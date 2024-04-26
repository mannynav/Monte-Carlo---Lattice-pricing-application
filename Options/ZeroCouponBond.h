// ZeroCouponBond.h

#include <cmath>
#include "ZeroCouponBondBase.h"

#ifndef ZeroCouponBondH
#define ZeroCouponBondH

class ZeroCouponBond : public ZeroCouponBondBase {

public:

	explicit ZeroCouponBond();
	~ZeroCouponBond();

	//double GetData() const;
	//void Interpolate function. What is the return type?

	double ZCBCurve(double t) const {
		return std::exp(-0.05 * t);
	}
	double f0(double) const; 
	double theta(double) const;
	double f(double grid) const;
	double integrateThetaTrapz(double lower, double upper, double subInterval) const;
	double A_r(double lambda_, double eta_, double T1_, double T2_) const;
	double B_r(double grid) const;

	double P_T1_T2(double RT_, double T1_) const;

	// CreatableBase stuff
	std::string GetName() const;
	std::string GetID() const;
	std::string GetBaseClassName() const;
	std::string GetStringify() const;


private:
	
	double T1_;
	double T2_;
	double lambda_;
	double eta_;
};

#endif
