//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionEuroCall.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionEuroCallH
#define OptionEuroCallH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>
#include "OptionBase.h"
#include "CreateableBase.h"

class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionEuroCall
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionEuroCall : public OptionBase
{
    public:
        explicit OptionEuroCall();
        OptionEuroCall(double X, double T, bool early);

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
