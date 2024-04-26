#pragma once

#include <vector>
#include "OptionBase.h"


class BarrierUIC : public OptionBase {

public:

	explicit BarrierUIC();

	BarrierUIC(double X,double T, double K, bool early) : X_(X),T_(T), K_(K), early_(early) {}

	double GetT() const { return T_; }

	double ComputePayoff(const double) const;

	bool hitBarrier(const std::vector<double>& vec, double b) const;

	double ComputePayoffPath(const std::vector<double>&) const;

	void ComputePayoffSlice(const double, const VecB&, VecB&) const;

	bool IsEarlyExercise() const;

	bool IsExerciseTime(double t) const;

	//Creatable base stuff/ Inherited from creatable base, an indirect base class
	std::string GetName() const;
	std::string GetID() const;
	std::string GetBaseClassName() const;
	std::string GetStringify() const;

private:

	double T_{};
	double X_{};
	double K_{};	//will need class for K. This is the barrier.

	bool early_{};

};
