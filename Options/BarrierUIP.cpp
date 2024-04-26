
#include "BarrierUIP.h"

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

template<> std::string ClassName<BarrierUIP>() {
	return "BarrierUIP";
}

template<> std::string ClassID<BarrierUIP>() {

	return "2";
}

template<> std::string BaseClassName<BarrierUIP>() {
	return ClassName<OptionBase>();
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


namespace {

	FactoryRegistration<OptionBase, BarrierUIP> RegisterBarrierUIP;

}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//Get input paramters from the IO_manager instance! which is a singleton
BarrierUIP::BarrierUIP() :
	X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
	, T_(IO_Manager::Instance().GetValue<double>(ParameterT())),
	K_(IO_Manager::Instance().GetValue<double>(ParameterK()))
	, early_(false){}


double BarrierUIP::ComputePayoff(const double S) const {

	return std::max(X_ - S, 0.0);

}

double BarrierUIP::ComputePayoffPath(const std::vector<double>& vec) const {

	double payoff{};

	bool isHit = hitBarrier(vec, K_);

	if(isHit)
	{
		payoff = ComputePayoff(vec.back());
	}

	return payoff;

}

void BarrierUIP::ComputePayoffSlice(const double , const VecB& S, VecB& P) const
{

	long lb = S.LBound();
	long ub = S.UBound();

	for (long i = lb; i <= ub; ++i) {
		P[i] = ComputePayoff(S[i]);
	}
}

bool BarrierUIP::IsEarlyExercise() const {
	return early_;
}

bool BarrierUIP::IsExerciseTime(double t) const
{
	return t == T_;
}

bool BarrierUIP::hitBarrier(const std::vector<double>& vec, double b) const {

	auto iter = std::find_if(vec.begin(), vec.end(), [b](double s) {return s >= b; });

	if (iter != vec.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Creatable base stuff
std::string BarrierUIP::GetName() const { return ClassName<BarrierUIP>(); }
std::string BarrierUIP::GetID() const { return ClassID<BarrierUIP>(); }
std::string BarrierUIP::GetBaseClassName() const { return BaseClassName<BarrierUIP>(); }
std::string BarrierUIP::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	nm += "  K = " + ut::ConvertToString<double>(K_) + '\n';
	return nm;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
