//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// ProcessHSV.cpp - this class implements the Heston Stochastic Volatility model
// using the Almost Exact Scheme to sample from the variance process.
// Prices produced have been bumped against values in existing literature
// and produce similar results.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


#include "ProcessHSV.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterSigma.h"
#include "ParameterR.h"
#include "ParameterT.h"
#include "ParameterN.h"

#include "ParameterCorr.h"
#include "ParameterLTMean.h"
#include "ParameterMeanReversion.h"
#include "ParameterVolVol.h"
#include "ParameterV0.h"
#include "ParameterPsiC.h"

#include "rv_library.h"
#include "VecB.h"
#include "utility.h"

#include <cmath>
#include <vector>
#include <Boost/random.hpp>
#include <Boost/random/non_central_chi_squared_distribution.hpp>
#include <random>


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <>
std::string ClassName<ProcessHSV>()
{
	return "ProcessHSV";
}

template <>
std::string ClassID<ProcessHSV>()
{
	return "h";
}

template <>
std::string BaseClassName<ProcessHSV>()
{
	return ClassName<ProcessBase>();
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	FactoryRegistration<ProcessBase, ProcessHSV> RegisterProcessHSV;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ProcessHSV::ProcessHSV() :
	S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0())),
	V_0_(IO_Manager::Instance().GetValue<double>(ParameterV0())),
	r_(IO_Manager::Instance().GetValue<double>(ParameterR())),
	corr_(IO_Manager::Instance().GetValue<double>(ParameterCorr())),
	gamma(IO_Manager::Instance().GetValue<double>(ParameterVolVol())),
	kappa(IO_Manager::Instance().GetValue<double>(ParameterMeanReversion())),
	vbar(IO_Manager::Instance().GetValue<double>(ParameterLTMean()))
{
	long N = IO_Manager::Instance().GetValue<long>(ParameterN());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	FillCirPath_.resize(N + 1);

	//Everything that can be pre-computed is done here.

	dt_ = T / N;
	expression_ = std::exp(-kappa * dt_);

	k0_ = (r_ - (corr_ * kappa * vbar) / gamma) * dt_;
	k1_ = (((corr_ * kappa) / gamma) - 0.5) * dt_ - corr_ / gamma;
	k2_ = corr_ / gamma;

	delta_ = 4.0 * kappa * vbar / (gamma * gamma);
	c_ = (1.0 / (4.0 * kappa)) * gamma * gamma * (1.0 - expression_);
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHSV::Next_V(double V) const
{
	double kappaBar = (4.0 * kappa * V * expression_) / (gamma * gamma * (1.0 - expression_));
	double sample = c_ * rv::rand_ncchiSQ(delta_, kappaBar);
	return sample;
}


void ProcessHSV::FillUpPath(std::vector<double>& path) const
{
	long N = path.size() - 1;
	path[0] = S_0_;
	FillCirPath_[0] = V_0_;

	for (int i = 1; i <= N; ++i)
	{
		FillCirPath_[i] = Next_V(FillCirPath_[i - 1]);

		path[i] = path[i - 1] * std::exp(k0_ + k1_ * FillCirPath_[i - 1] + k2_ * FillCirPath_[i] +
			std::sqrt((1 - corr_ * corr_) * dt_ * FillCirPath_[i - 1]) * rv::GetNormalVariate());
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	The functions below are not used for the Monte Carlo simulation of this model
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHSV::Next_S(double S) const
{
	//Not used in this implementation
	return S;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Rollback stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHSV::GetDtDiscount(double t) const { return std::exp(-r_ * t); }

double ProcessHSV::GetSScale(double t) const
{
	return 0.0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ProcessHSV::FillUpSlice(const double T, VecB& S, const VecB& Z) const
{
	long lb = Z.LBound();
	long ub = Z.UBound();
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ExplicitSoln().  z is a Wiener increment over the period T
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessHSV::ExplicitSoln(const double T, const double z) const
{
	return 0.0;
}

double ProcessHSV::ExplicitSoln(const double S, const double T, const double z) const
{
	return 0.0;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


std::string ProcessHSV::GetName() const { return ClassName<ProcessHSV>(); }
std::string ProcessHSV::GetID() const { return ClassID<ProcessHSV>(); }
std::string ProcessHSV::GetBaseClassName() const { return BaseClassName<ProcessHSV>(); }

std::string ProcessHSV::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  S0 =  " + ut::ConvertToString<double>(S_0_) + '\n';
	nm += "  V0 =  " + ut::ConvertToString<double>(V_0_) + '\n';
	nm += "  Correlation = " + ut::ConvertToString<double>(corr_) + '\n';
	nm += "  Kappa =   " + ut::ConvertToString<double>(kappa) + '\n';
	nm += "  Gamma =   " + ut::ConvertToString<double>(gamma) + '\n';
	nm += "  Vbar =   " + ut::ConvertToString<double>(vbar) + '\n';
	return nm;
}
