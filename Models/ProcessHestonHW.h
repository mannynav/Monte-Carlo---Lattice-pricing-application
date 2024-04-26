//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ProcessHestonHW.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef ProcessHestonHWH
#define ProcessHestonHWH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessBase.h"
#include <vector>
#include <complex>

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class ProcessHestonHW
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ProcessHestonHW : public ProcessBase
{
public:
	explicit ProcessHestonHW();

	double ExplicitSoln(double T, double z) const override;
	double GetS0() const override { return S_0_; }

	//Fast exponential function. Not the most accurate
	double fastExp(double x) const
	{
		constexpr double a = (1ll << 52) / 0.6931471805599453;
		constexpr double b = (1ll << 52) * (1023 - 0.04367744890362246);
		x = a * x + b;

		constexpr double c = (1ll << 52);
		constexpr double d = (1ll << 52) * 2047;
		if (x < c || x > d)
			x = (x < c) ? 0.0 : d;

		uint64_t n = static_cast<uint64_t>(x);
		memcpy(&x, &n, 8);
		return x;
	}

	//MC stuff.
	double Next_S(double S) const override;
	double Next_V(double V) const;
	void FillUpPath(std::vector<double>& path) const override;


	//HW stuff
	double ZCB(double t) const
	{
		return std::exp(-0.1*t);
	}
	double f0(double t) const;
	double theta(double t) const;
	double GetM_T() const override
	{
		return M_t.back();
	};


	//Trinomial stuff. Not used in this model
	void FillUpSlice(double T, VecB& S, const VecB& Z) const override;
	double GetDtDiscount(double t) const override;
	double GetSScale(double t) const override;

	// CreatableBase stuff
	std::string GetName() const override;
	std::string GetID() const override;
	std::string GetBaseClassName() const override;
	std::string GetStringify() const override;

private:
	double S_0_; //Initilized from input manager.
	double V_0_;
	double r_;
	double drift_{};

	double corrXR_;
	double corrXV_;
	double volvol_;
	double meanreversion_;
	double ltmean_;
	double expression_;
	double PsiC_;

	double eta_;
	double lambda_;
	double differentiationStep_;

	double dt_; //Computed and assigned in constructor
	double k0_;
	double k1_;
	double k2_;
	double k3_;

	double L1_;
	double L2_;

	mutable std::vector<double> pathV_;
	mutable std::vector<double> pathR_;
	mutable std::vector<double> M_t; //Vector for numeraire
	mutable std::vector<double> timeDt_;

	double ExplicitSoln(double S, double T, double z) const;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
