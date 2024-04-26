#pragma once



#include <vector>
#include "OptionBase.h"
#include "CreateableBase.h"

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class FloatingLookBackCall.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class FloatingLookBackCall : public OptionBase
{
public:
    explicit FloatingLookBackCall();
    FloatingLookBackCall (double T, double Smin, bool early)
		: T_(T), minS(Smin), early_(early) {}

    double ComputePayoff(const double) const;
    double ComputePayoffPath(const std::vector<double>&) const;
    void ComputePayoffSlice(const double, const VecB&, VecB&) const;

    bool IsEarlyExercise() const { return early_; }
    bool IsExerciseTime(double t) const;


    double GetT() const { return T_; }
    double GetMinS() const { return minS; }
    bool GetEarly() const { return early_; }

    // CreatableBase stuff
    std::string GetName() const;
    std::string GetID() const;
    std::string GetBaseClassName() const;
    std::string GetStringify() const;

private:
    double T_;
    mutable double minS; //Will be modified inside ComputePayoffPath
    bool early_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
