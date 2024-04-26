//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ProcessHSV.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef ProcessHSVH
#define ProcessHSVH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessBase.h"
#include <vector>
#include <boost/random/mersenne_twister.hpp>

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class ProcessHSV
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ProcessHSV : public ProcessBase
{
public:
    explicit ProcessHSV();

    double ExplicitSoln(const double T, const double z) const;
    double GetS0() const { return S_0_; }

    //MC stuff.
    double Next_S(double S) const;
    double Next_V(double V) const;
    void FillUpPath(std::vector<double>& path) const;

    //Lattice stuff but is not implemented in this model
    void FillUpSlice(const double T, VecB& S, const VecB& Z) const;
    double GetDtDiscount(double t) const;
    double GetSScale(double t) const;

    // CreatableBase stuff
    std::string GetName() const;
    std::string GetID() const;
    std::string GetBaseClassName() const;
    std::string GetStringify() const;

private:
    double S_0_;
    double V_0_;
    double r_;

    double corr_;
    double gamma;
    double kappa;
    double vbar;
    double expression_;

    double dt_;		
    double k0_;
    double k1_;
    double k2_;

    double delta_;
    double c_;

    mutable std::vector<double> FillCirPath_;
    double ExplicitSoln(const double S, const double T, const double z) const;
 
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


