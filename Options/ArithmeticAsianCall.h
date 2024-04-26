#pragma once

#include <vector>
#include "OptionBase.h"
#include "CreateableBase.h"

class VecB;

////////// Arithmetic Asian Call ///////////

class ArithmeticAsianCall : public OptionBase {


public:

	explicit ArithmeticAsianCall();
	ArithmeticAsianCall(double X, double T, bool early);

	//Must define all 6 virtual functions from OptionBase class

	double GetT() const { return T_; }

	double ComputePayoff(const double) const;

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

	double X_{};
	double T_{};
	bool early_{};

};