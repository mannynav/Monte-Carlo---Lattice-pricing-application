#include "BarrierUIC.h"

#include "FactoryRegistration.h"	//need to register with factory
#include "IO_Manager.h"				//Used to get input
#include "CreateableBase.h"			//Has template specializations for Class Name, ID and Base class 	


#include "ParameterX.h"				//Strike
#include "ParameterT.h"				//Time to expiry
#include "ParameterK.h"				//Barrier

#include "VecB.h"
#include "utility.h"

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<BarrierUIC>() {
	return "BarrierUIC";
}

template<> std::string ClassID<BarrierUIC>() {
	return "1";
}

template<> std::string BaseClassName<BarrierUIC>() {
	return ClassName<OptionBase>();
}



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace {

	FactoryRegistration<OptionBase, BarrierUIC> RegisterBarrierUIC;

}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

BarrierUIC::BarrierUIC() : 
  X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
, T_(IO_Manager::Instance().GetValue<double>(ParameterT())), 
  K_(IO_Manager::Instance().GetValue<double>(ParameterK()))
, early_(false){}

double BarrierUIC::ComputePayoff(const double S) const {

	return std::max(S - X_, 0.0);

}

double BarrierUIC::ComputePayoffPath(const std::vector<double>& vec) const {

    double payoff = 0.0;

    bool isHit = hitBarrier(vec, K_);

    if(isHit)
    {
        payoff = ComputePayoff(vec.back());
    }

	return payoff;
}

void BarrierUIC::ComputePayoffSlice(const double, const VecB& S, VecB& P) const
{

	long lb = S.LBound();
	long ub = S.UBound();

	for (long i = lb; i <= ub; ++i) {
		P[i] = ComputePayoff(S[i]);
	}
}

bool BarrierUIC::IsEarlyExercise() const {
	return early_;
}

bool BarrierUIC::IsExerciseTime(double t) const
{
	return t == T_;
}

bool BarrierUIC::hitBarrier(const std::vector<double>& vec, double b) const {

    auto iter = std::find_if(vec.begin(), vec.end(), [b](double s) {return s >= b; });

    if(iter != vec.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Creatable base stuff
std::string BarrierUIC::GetName() const { return ClassName<BarrierUIC>(); }
std::string BarrierUIC::GetID() const { return ClassID<BarrierUIC>(); }
std::string BarrierUIC::GetBaseClassName() const { return BaseClassName<BarrierUIC>(); }
std::string BarrierUIC::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
