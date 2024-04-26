//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ProcessGBM.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef ProcessGBMH
#define ProcessGBMH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessBase.h"
#include <vector>

class BrownianPathBase;
class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class ProcessGBM
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ProcessGBM : public ProcessBase
{
    public:
        explicit ProcessGBM();
        ~ProcessGBM();

        double ExplicitSoln(const double T, const double z) const;
        double GetS0() const {return S_0_;}

		//MC stuff.
        double Next_S(double S) const;
        void FillUpPath(std::vector<double> & path) const;
        
        void FillUpSlice(const double T, VecB & S, const VecB & Z) const;
        double GetDtDiscount(double t) const;
        double GetSScale(double t) const;
        
	   // CreatableBase stuff
	   std::string GetName() const;
	   std::string GetID() const;
	   std::string GetBaseClassName() const;
	   std::string GetStringify() const;

    private:
        double S_0_;	//Initilized with parameters from input manager.
        double sig_;      
        double r_;      

        double dt_;		//Assigned in constructor
        double drift_;
        double rt_;

        BrownianPathBase* BrownianPath_;

        double ExplicitSoln(const double S, const double T, const double z) const;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
