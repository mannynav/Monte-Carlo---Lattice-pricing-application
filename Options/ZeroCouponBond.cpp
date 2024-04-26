// ZeroCouponBond.cpp

#include "ZeroCouponBond.h"

#include "FactoryRegistration.h"

#include "AccumulatorBase.h"
#include "OptionBase.h"
#include "ProcessBase.h"

#include "ParameterLambda.h"
#include "ParameterEta.h"
#include "ParameterT.h"
#include "ParameterT2.h"

#include "utility.h"
#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<ZeroCouponBond>()
{
	return "ZeroCouponBond";
}

template<> std::string ClassID<ZeroCouponBond>()
{
	return "zcb";
}

template<> std::string BaseClassName<ZeroCouponBond>()
{
	return ClassName<ZeroCouponBondBase>();
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	FactoryRegistration<ZeroCouponBondBase, ZeroCouponBond> RegisterZeroCouponBond;
}


ZeroCouponBond::ZeroCouponBond() :

	lambda_(IO_Manager::Instance().GetValue<double>(ParameterLambda())),
	eta_(IO_Manager::Instance().GetValue<double>(ParameterEta())),
	T2_(IO_Manager::Instance().GetValue<double>(ParameterT2())),
	T1_(IO_Manager::Instance().GetValue<double>(ParameterT()))

{
	std::cout << "ZCB constructed: " << std::endl;

}

ZeroCouponBond::~ZeroCouponBond() {
	//delete HW_;
}

double ZeroCouponBond::B_r(double grid) const {

	double B_r_ = 1 / lambda_ * (std::exp(-lambda_ * grid) - 1);

	return B_r_;

};
double ZeroCouponBond::f0(double t) const {

	double fwdBond = ZCBCurve(t + 0.00001);

	double backwardBond = ZCBCurve(t - 0.00001);

	double result = -(std::log(fwdBond) - std::log(backwardBond)) / (2 * 0.00001);

	return result;

}
double ZeroCouponBond::theta(double t) const {


	double result = (1 / lambda_) * (f0(t + 0.00001) - f0(t - 0.00001)) / (2 * 0.00001) +
		f0(t) + (eta_ * eta_) * (1 - std::exp(-2 * lambda_ * t)) / (2 * lambda_ * lambda_);

	return result;


}
double ZeroCouponBond::f(double grid) const
{
	//Get T1_ is expiry of the option
	double tenor = T2_ - T1_;

	//HW_Theta comes from Hull White Process
	double theta_ = theta(tenor - grid);

	double B_r_ = B_r(grid);

	return theta_ * B_r_;
}
double ZeroCouponBond::integrateThetaTrapz(double lower, double upper, double subInterval) const
{
	double stepSize = (upper - lower) / subInterval;

	/* Finding Integration Value */
	double integration = f(lower) + f(upper);

	for (int i = 1; i <= subInterval - 1; i++)
	{
		double k = lower + i * stepSize;
		integration = integration + 2.0 * (f(k));
	}

	integration = integration * stepSize / 2.0;

	return integration;
}
double ZeroCouponBond::A_r(double lambda_, double eta_, double T1_, double T2_) const {

	double tenor = T2_ - T1_;

	double integrationRes = integrateThetaTrapz(0, tenor, 250);

	double temp1 = lambda_ * integrationRes;

	double temp2 = ((eta_ * eta_) / (4 * std::pow(lambda_, 3))) * (std::exp(-2 * lambda_ * tenor) * (4 * std::exp(0.02 * tenor) - 1) - 3)
		+ (eta_ * eta_ * tenor) / (2 * lambda_ * lambda_);

	return temp1 + temp2;
}
double ZeroCouponBond::P_T1_T2(double RT_, double T1_) const {

	//Bound price under the Hull-White model with affine form
	double A_r_ = A_r(lambda_, eta_, T1_, T2_);

	auto B_R_{ [](double lambda_,double T1_, double T2_) -> double {

		return (1 / lambda_) * (std::exp(-lambda_ * (T2_ - T1_)) - 1);

	} };

	double B_r_ = B_R_(lambda_, T1_, T2_);

	//R_T comes from Hull White Process to evolve R
	return std::exp(A_r_ + B_r_ * RT_);

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string ZeroCouponBond::GetName() const { return ClassName<ZeroCouponBond>(); }
std::string ZeroCouponBond::GetID() const { return ClassID<ZeroCouponBond>(); }
std::string ZeroCouponBond::GetBaseClassName() const { return BaseClassName<ZeroCouponBond>(); }
std::string ZeroCouponBond::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  T1 = " + ut::ConvertToString<double>(T1_) + '\n';
	nm += "  T2 = " + ut::ConvertToString<double>(T2_) + '\n';
	return nm;
}