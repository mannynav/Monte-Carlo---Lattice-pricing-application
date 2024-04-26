

//////// FloatingLookBackCall.cpp ////////////


#include "FloatingLookBackCall.h"


#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterX.h"
#include "ParameterT.h"

#include "VecB.h"
#include "utility.h"
#include "VectorStatistics.h"

#include <algorithm>
#include <vector>
#include <gtest/gtest.h>


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<FloatingLookBackCall>()
{
    return "FloatingLookBackCall";
}

template<> std::string ClassID<FloatingLookBackCall>()
{
    return "7";
}

template<> std::string BaseClassName<FloatingLookBackCall>()
{
    return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
    FactoryRegistration<OptionBase, FloatingLookBackCall> RegisterFloatingLookBackCall;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

FloatingLookBackCall::FloatingLookBackCall()
    : T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
    , minS(IO_Manager::Instance().GetValue<double>(ParameterS0()))
    , early_(false)
{}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoff()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double FloatingLookBackCall::ComputePayoff(const double S) const
{
    return std::max(0.0, (S-minS));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double FloatingLookBackCall::ComputePayoffPath(const std::vector<double>& path) const
{
    double temp = minS;

    minS = vs::VectorMinimum(path);

    double pay = ComputePayoff(path.back());
    
    minS = temp;                                //change back to minS for each path since minS is mutable.

    return pay;

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void FloatingLookBackCall::ComputePayoffSlice(const double T, const VecB& S, VecB& P) const
{
   throw std::runtime_error("FloatingLookBackCall::ComputePayoffSlice not implemented for this option");
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	IsExerciseTime()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool FloatingLookBackCall::IsExerciseTime(double t) const
{
    return t == T_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string FloatingLookBackCall::GetName() const { return ClassName<FloatingLookBackCall>(); }
std::string FloatingLookBackCall::GetID() const { return ClassID<FloatingLookBackCall>(); }
std::string FloatingLookBackCall::GetBaseClassName() const { return BaseClassName<FloatingLookBackCall>(); }
std::string FloatingLookBackCall::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
    nm += "  minS = " + ut::ConvertToString<double>(minS) + '\n';
 
    return nm;
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
