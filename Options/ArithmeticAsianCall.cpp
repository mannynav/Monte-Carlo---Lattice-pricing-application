//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ArithmeticAsianCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ArithmeticAsianCall.h"

#include "IO_Manager.h"				//Used to get input
#include "FactoryRegistration.h"	//need to register with factory
#include "CreateableBase.h"			//Has template specializations for Class Name, ID and Base class 	


#include "ParameterX.h"				//Strike
#include "ParameterT.h"				//Time to expiry

#include "VecB.h"
#include "utility.h"
#include "VectorStatistics.h"

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


template<> std::string ClassName<ArithmeticAsianCall>() {

	return "ArithmeticAsianCall";
}

template<> std::string ClassID<ArithmeticAsianCall>() {
	return "3";
}

template<> std::string BaseClassName<ArithmeticAsianCall>() {
	return ClassName<OptionBase>();
}


namespace {

	//Calls default constructor of Factory registration, which in turn calls Instance() from the Factory<BaseCLass>
	//to register the object
	FactoryRegistration<OptionBase, ArithmeticAsianCall> RegisterArithmeticAsianCall;

}



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ArithmeticAsianCall::ArithmeticAsianCall() :
	T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
  , X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
  , early_(false) {

}

ArithmeticAsianCall::ArithmeticAsianCall(double X, double T, bool early) : X_(X),T_(T), early_(early){}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoff()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ArithmeticAsianCall::ComputePayoff(const double S) const {

	return std::max(S-X_ ,0.0);

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ArithmeticAsianCall::ComputePayoffPath(const std::vector<double>& vec) const {

	double payoff{};
	double avg{};

	avg = VectorStatisticalFunctions::VectorMean(vec);

	payoff = ComputePayoff(avg);

	return payoff;

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ArithmeticAsianCall::ComputePayoffSlice(const double, const VecB& S, VecB& P) const {

    throw std::runtime_error("ArithmeticAsianCall::ComputePayoffSlice:  cannot compute");
}

bool ArithmeticAsianCall::IsEarlyExercise() const {

	return early_;

}

bool ArithmeticAsianCall::IsExerciseTime(double t) const {

	return t == T_;

}

//Creatable base stuff
std::string ArithmeticAsianCall::GetName() const { return ClassName<ArithmeticAsianCall>(); }
std::string ArithmeticAsianCall::GetID() const { return ClassID<ArithmeticAsianCall>(); }
std::string ArithmeticAsianCall::GetBaseClassName() const { return BaseClassName<ArithmeticAsianCall>(); }
std::string ArithmeticAsianCall::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX