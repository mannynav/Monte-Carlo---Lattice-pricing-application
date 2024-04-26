//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  EuroCallOptionZCB.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef EuroCallOptionZCBH
#define EuroCallOptionZCBH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>
#include "OptionBase.h"
#include "ZeroCouponBond.h"

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class EuroCallOptionZCB
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class EuroCallOptionZCB : public OptionBase
{
public:
    explicit EuroCallOptionZCB();
    ~EuroCallOptionZCB();

    double GetT() const { return T_; }
    double GetP_T1_T2(double RT) const {

        double price_ = ZCB_.P_T1_T2(RT,T_);
    
        return price_;

    };

    double ComputePayoffPath(const std::vector<double>&) const;
    double ComputePayoff(const double) const;
    void ComputePayoffSlice(const double, const VecB&, VecB&) const;

    bool IsEarlyExercise() const { return early_; }
    bool IsExerciseTime(double t) const;

    // CreatableBase stuff
    std::string GetName() const;
    std::string GetID() const;
    std::string GetBaseClassName() const;
    std::string GetStringify() const;

private:
    double X_;
    double T_;
    bool early_;

    ZeroCouponBond ZCB_;

};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX