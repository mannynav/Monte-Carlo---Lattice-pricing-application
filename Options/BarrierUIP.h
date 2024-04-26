#pragma once

#include <vector>
#include "OptionBase.h"

class BarrierUIP : public OptionBase {

public:

	explicit BarrierUIP();

	BarrierUIP(double X, double T, double K, bool early) : X_(X), T_(T), K_(K), early_(early) {}

	double ComputePayoff(const double) const;

	bool hitBarrier(const std::vector<double>& vec, double b) const;

	double ComputePayoffPath(const std::vector<double>&) const;

	void ComputePayoffSlice(const double, const VecB&, VecB&) const;

	bool IsEarlyExercise() const;

	bool IsExerciseTime(double t) const;

	double GetX() const { return X_; }
	double GetT() const { return T_; }
	double GetK() const { return K_; }
	double GetEarly() const { return early_; }
	

	//Creatable base stuff/ Inherited from creatable base, an indirect base class
	std::string GetName() const;
	std::string GetID() const;
	std::string GetBaseClassName() const;
	std::string GetStringify() const;

private:

	double X_{};
	double T_{};
	double K_{};	//Barrier

	bool early_{};

};
