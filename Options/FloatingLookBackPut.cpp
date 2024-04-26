
/////////// Floating LookBack Put.cpp  //////////////

#include "FloatingLookBackPut.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterX.h"
#include "ParameterT.h"

#include "VecB.h"
#include "VectorStatistics.h"
#include "utility.h"

#include <algorithm>
#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<FloatingLookBackPut>()
{
    return "FloatingLookBackPut";
}

template<> std::string ClassID<FloatingLookBackPut>()
{
    return "8";
}

template<> std::string BaseClassName<FloatingLookBackPut>()
{
    return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
    FactoryRegistration<OptionBase, FloatingLookBackPut> RegisterFloatingLookBackPut;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

FloatingLookBackPut::FloatingLookBackPut()
    : T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
    , maxS(IO_Manager::Instance().GetValue<double>(ParameterS0()))
    , early_(false)
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoff()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double FloatingLookBackPut::ComputePayoff(const double S) const
{
    return std::max(0.0, (maxS - S));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double FloatingLookBackPut::ComputePayoffPath(const std::vector<double>& path) const
{
    double temp = maxS;

    maxS = vs::VectorMaximum(path);

    double pay = ComputePayoff(path.back());

    maxS = temp;

    return pay;

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void FloatingLookBackPut::ComputePayoffSlice(const double T, const VecB& S, VecB& P) const
{
    throw std::runtime_error("FloatingLookBackPut::ComputePayoffSlice not implemented");
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	IsExerciseTime()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool FloatingLookBackPut::IsExerciseTime(double t) const
{
    return t == T_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string FloatingLookBackPut::GetName() const { return ClassName<FloatingLookBackPut>(); }
std::string FloatingLookBackPut::GetID() const { return ClassID<FloatingLookBackPut>(); }
std::string FloatingLookBackPut::GetBaseClassName() const { return BaseClassName<FloatingLookBackPut>(); }
std::string FloatingLookBackPut::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
    nm += "  maxS = " + ut::ConvertToString<double>(maxS) + '\n';

    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
