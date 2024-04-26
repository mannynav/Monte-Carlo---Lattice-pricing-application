//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ProcessHestonHWcpp - this class implements the Heston Hull-White model.
//  The Heston dynamics are simulated using the Quadratic Exponential scheme for the variance process, see Anderson 2008.
//  The Hull-White dynamics are simulated using the Euler scheme, see Brigo and Mercurio 2006 and Oosterlee and Grzelak, 2019.
//	Prices for the model were compared against those in Oosterlee and Grzelak, 2019 and were found to be consistent, where they used the COS method
//  and an approximate to the Heston Hull-White model to compare prices for long maturity European options
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessHestonHW.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterSigma.h"
#include "ParameterR.h"
#include "ParameterT.h"
#include "ParameterN.h"

#include "ParameterCorrXR.h"
#include "ParameterCorrXV.h"
#include "ParameterLTMean.h"
#include "ParameterMeanReversion.h"
#include "ParameterVolVol.h"
#include "ParameterV0.h"
#include "ParameterPsiC.h"

#include "ParameterEta.h"
#include "ParameterLambda.h"

#include "rv_library.h"
#include "VecB.h"
#include "utility.h"

#include <cmath>
#include <vector>
#include <Boost/random.hpp>
//#include <Boost/random/non_central_chi_squared_distribution.hpp>
#include <random>


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <>
std::string ClassName<ProcessHestonHW>()
{
	return "ProcessHestonHW";
}

template <>
std::string ClassID<ProcessHestonHW>()
{
	return "hhw";
}

template <>
std::string BaseClassName<ProcessHestonHW>()
{
	return ClassName<ProcessBase>();
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	FactoryRegistration<ProcessBase, ProcessHestonHW> RegisterProcessHestonHW;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ProcessHestonHW::ProcessHestonHW() :
	S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0())),
	V_0_(IO_Manager::Instance().GetValue<double>(ParameterV0())),
	r_(IO_Manager::Instance().GetValue<double>(ParameterR())),
	corrXR_(IO_Manager::Instance().GetValue<double>(ParameterCorrXR())),
	corrXV_(IO_Manager::Instance().GetValue<double>(ParameterCorrXV())),
	volvol_(IO_Manager::Instance().GetValue<double>(ParameterVolVol())),
	meanreversion_(IO_Manager::Instance().GetValue<double>(ParameterMeanReversion())),
	ltmean_(IO_Manager::Instance().GetValue<double>(ParameterLTMean())),
	PsiC_(IO_Manager::Instance().GetValue<double>(ParameterPsiC())),
	eta_(IO_Manager::Instance().GetValue<double>(ParameterEta())),
	lambda_(IO_Manager::Instance().GetValue<double>(ParameterLambda()))
{
	long N = IO_Manager::Instance().GetValue<long>(ParameterN());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());

	pathV_.resize(N + 1);
	pathR_.resize(N + 1);
	M_t.resize(N + 1);
	timeDt_.resize(N + 1);
	M_t[0] = 1;

	dt_ = T / N;
	differentiationStep_ = 0.0001;
	expression_ = std::exp(-meanreversion_ * dt_);

	k0_ = -(meanreversion_ * corrXV_ * ltmean_ * dt_) / volvol_;

	k1_ = ((meanreversion_ * corrXV_ / volvol_) - 0.5) * dt_ - corrXV_ / volvol_;

	k2_ = corrXV_ / volvol_;

	k3_ = (1 - corrXV_ * corrXV_) * dt_;

	L1_ = (eta_ * std::sqrt(dt_) * corrXR_) / std::sqrt(1 - corrXV_ * corrXV_);

	L2_ = (eta_ * std::sqrt(dt_)) * std::sqrt(1 - ((corrXR_ * corrXR_) / (1 - corrXV_ * corrXV_)));
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHestonHW::Next_V(double V) const
{
	double m_ = ltmean_ + (V - ltmean_) * expression_;


	double s2_ = (V * volvol_ * volvol_ * expression_) * (1 - expression_) / meanreversion_
		+ ((ltmean_ * volvol_ * volvol_) * (1 - expression_) * (1 - expression_)) / (2 * meanreversion_);

	double psi_ = s2_ / (m_ * m_);


	if (psi_ <= PsiC_)
	{
		double b2_ = (2 / psi_) - 1 + std::sqrt((2 / psi_)) * std::sqrt((2 / psi_) - 1);
		double a_ = m_ / (1 + b2_);

		double z = rv::GetNormalVariate();

		return a_ * (std::sqrt(b2_) + z) * (std::sqrt(b2_) + z);
	}
	double p_ = (psi_ - 1) / (psi_ + 1);
	double beta_ = (1 - p_) / m_;
	double u = rv::rand_uniform();

	if (u <= p_)
	{
		return 0;
	}

	if (p_ < u <= 1)
	{
		double ret = (std::log((1 - p_) / (1 - u))) / beta_;

		return ret;
	}
}

double ProcessHestonHW::f0(double t) const
{
	double fwdBond = ZCB(t + differentiationStep_);
	double backwardBond = ZCB(t - differentiationStep_);

	double result = -(std::log(fwdBond) - std::log(backwardBond)) / (2 * differentiationStep_);

	return result;
}

double ProcessHestonHW::theta(double t) const
{
	double result = (1 / lambda_) * (f0(t + dt_) - f0(t - dt_)) / (2 * dt_) +
		f0(t) + (eta_ * eta_) / (2 * lambda_ * lambda_) * (1 - std::exp(-2.0 * lambda_ * t));

	return result;
}


void ProcessHestonHW::FillUpPath(std::vector<double>& path) const
{
	long N = path.size() - 1;

	path[0] = S_0_;
	pathV_[0] = V_0_;
	pathR_[0] = f0(0.00001);
	timeDt_[0] = 0;

	for (int i = 1; i <= N; ++i)
	{
		double z1 = rv::GetNormalVariate();
		double z2 = rv::GetNormalVariate();

		pathR_[i] = pathR_[i - 1] + lambda_ * (theta(timeDt_[i - 1]) - pathR_[i - 1]) * dt_ + L1_ * z1 + L2_ * z2;

		M_t[i] = M_t[i - 1] * std::exp(0.5 * (pathR_[i] + pathR_[i - 1]) * dt_);

		pathV_[i] = Next_V(pathV_[i - 1]);

		path[i] = path[i - 1] * std::exp(
			pathR_[i - 1] * dt_ + k0_ + k1_ * pathV_[i - 1] + k2_ * pathV_[i] + std::sqrt(
				k3_ * pathV_[i - 1]) * z1);

		timeDt_[i] = timeDt_[i - 1] + dt_;
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	The functions below are not used for the implentation of the Heston Hull White model
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHestonHW::Next_S(double S) const
{
	return 0.0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Rollback stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHestonHW::GetDtDiscount(double t) const { return std::exp(-r_ * t); }

double ProcessHestonHW::GetSScale(double t) const
{
	return 0.0;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ProcessHestonHW::FillUpSlice(const double T, VecB& S, const VecB& Z) const
{
	long lb = Z.LBound();
	long ub = Z.UBound();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ExplicitSoln().  z is a Wiener increment over the period T
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHestonHW::ExplicitSoln(const double T, const double z) const
{
	return 0.0;
}

double ProcessHestonHW::ExplicitSoln(const double S, const double T, const double z) const
{
	return 0.0;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


std::string ProcessHestonHW::GetName() const { return ClassName<ProcessHestonHW>(); }
std::string ProcessHestonHW::GetID() const { return ClassID<ProcessHestonHW>(); }
std::string ProcessHestonHW::GetBaseClassName() const { return BaseClassName<ProcessHestonHW>(); }

std::string ProcessHestonHW::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	if (2 * meanreversion_ * ltmean_ > volvol_ * volvol_)
	{
		nm += "  Feller condition is satisfied: " + ut::ConvertToString<double>(2 * meanreversion_ * ltmean_) + " > " +
			ut::ConvertToString<double>(volvol_ * volvol_) + '\n';
	}
	else
	{
		nm += "  Feller condition not satisfied: " + ut::ConvertToString<double>(volvol_ * volvol_) + " >= " +
			ut::ConvertToString<double>(2 * meanreversion_ * ltmean_) + '\n';
	}

	nm += "  S0 =  " + ut::ConvertToString<double>(S_0_) + '\n';
	nm += "  r =   " + ut::ConvertToString<double>(r_) + '\n';
	nm += "  V0 =  " + ut::ConvertToString<double>(V_0_) + '\n';
	nm += "  Correlation XR = " + ut::ConvertToString<double>(corrXR_) + '\n';
	nm += "  Correlation XV = " + ut::ConvertToString<double>(corrXV_) + '\n';
	return nm;
}
