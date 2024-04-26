//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionEuroPut.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionEuroPutH
#define OptionEuroPutH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionBase.h"
#include <vector>

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionEuroPut
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionEuroPut : public OptionBase
{
    public:
        explicit OptionEuroPut();
        OptionEuroPut(double X, double T, bool early);
        
		double GetT() const {return T_;}

        double ComputePayoffPath(const std::vector<double> &) const;
        double ComputePayoff(const double) const;
        void ComputePayoffSlice(const double, const VecB &, VecB &) const;

        bool IsEarlyExercise() const {return early_;}
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
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
