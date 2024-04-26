#pragma once


#include <vector>
#include "OptionBase.h"
#include "CreateableBase.h"

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class CashOrNothingCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class CashOrNothingCall : public OptionBase
{
public:
    explicit CashOrNothingCall();

    CashOrNothingCall(double X, double T, double Q, bool early)
		: X_(X), T_(T), Q_(Q), early_(early) {}

    double ComputePayoff(const double) const;
    double ComputePayoffPath(const std::vector<double>&) const;
    void ComputePayoffSlice(const double, const VecB&, VecB&) const;

    bool IsEarlyExercise() const { return early_; }
    bool IsExerciseTime(double t) const;

    double GetX() const { return X_; }
    double GetT() const { return T_; }
    double GetQ() const { return Q_; }
    double GetEarly() const { return early_; }

    // CreatableBase stuff
    std::string GetName() const;
    std::string GetID() const;
    std::string GetBaseClassName() const;
    std::string GetStringify() const;

private:
    double X_;
    double T_;
    double Q_{};            //Fixed cash amount Q.

    bool early_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
