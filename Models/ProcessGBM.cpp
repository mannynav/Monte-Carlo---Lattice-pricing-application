//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ProcessGBM.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessGBM.h"
#include "BrownianPathBase.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterSigma.h"
#include "ParameterR.h"
#include "ParameterT.h"
#include "ParameterN.h"

#include "rv_library.h"
#include "VecB.h"
#include "utility.h"

#include <cmath>
#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<ProcessGBM>()
{
    return "ProcessGBM";
}

template<> std::string ClassID<ProcessGBM>()
{
    return "g";
}

template<> std::string BaseClassName<ProcessGBM>()
{
    return ClassName<ProcessBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	FactoryRegistration<ProcessBase, ProcessGBM> RegisterProcessGBM;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ProcessGBM::ProcessGBM()
:   S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,   sig_(IO_Manager::Instance().GetValue<double>(ParameterSigma()))
,   r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
{

    BrownianPath_ = Factory<BrownianPathBase>::Instance().CreateObject();

    long N = IO_Manager::Instance().GetValue<long>(ParameterN());
    double T = IO_Manager::Instance().GetValue<double>(ParameterT());

	dt_ = T/N;
    drift_ = r_ - 0.5*sig_*sig_;
    rt_ = std::sqrt(dt_);
}

ProcessGBM::~ProcessGBM() {
    delete BrownianPath_;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::Next_S(double S) const
{
    double w = rv::GetNormalVariate();
    return ExplicitSoln(S, dt_, rt_*w);  // rt_*w is a Wiener increment
}

void ProcessGBM::FillUpPath(std::vector<double> & path) const
{
    path[0] = S_0_;
    double z = 0;

    long N = path.size()-1;
    VecB Bpath(0,N);

    //Fills up Bpath with increments of brownian motion - either regular, antithetic or from a stratified sample
    BrownianPath_->FillUpPath(Bpath);

   
    for(long i = 1;  i <= N; ++i)
    {

        z = Bpath[i] - Bpath[i - 1];
    
        path[i] = ExplicitSoln(path[i - 1], dt_, z);

    }

}
 
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Rollback stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::GetDtDiscount(double t) const {return std::exp(-r_*t);}
double ProcessGBM::GetSScale(double t) const {return std::exp(-drift_*t);}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ProcessGBM::FillUpSlice(const double T, VecB & S, const VecB & Z) const
{
    long lb = Z.LBound();
    long ub = Z.UBound();

    for(long i = lb;  i <= ub; ++i)
    {
        S[i] = ExplicitSoln(S_0_, T, Z[i]);
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ExplicitSoln().  z is a Wiener increment over the period T
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::ExplicitSoln(const double T, const double z) const
{
    return ExplicitSoln(S_0_, T, z);
}

double ProcessGBM::ExplicitSoln(const double S, const double T, const double z) const
{
    return S * std::exp(drift_ * T + sig_*z);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string ProcessGBM::GetName() const {return ClassName<ProcessGBM>();}
std::string ProcessGBM::GetID() const {return ClassID<ProcessGBM>();}
std::string ProcessGBM::GetBaseClassName() const {return BaseClassName<ProcessGBM>();}
std::string ProcessGBM::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  r =   " + ut::ConvertToString<double>(r_) + '\n';
    nm += "  sig = " + ut::ConvertToString<double>(sig_) + '\n';
    nm += "  S0 =  " + ut::ConvertToString<double>(S_0_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
