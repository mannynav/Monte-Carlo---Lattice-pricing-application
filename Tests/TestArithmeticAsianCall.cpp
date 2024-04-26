#include "ArithmeticAsianCall.h"

#include "ParameterX.h"
#include "ParameterT.h"

#include "VecB.h"
#include "utility.h"

#include <algorithm>
#include <vector>

#include <gtest\gtest.h>
#include <gmock\gmock.h>

#include "VectorStatistics.h"


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Tests
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ComputeAsianCallPayoff :public testing::Test
{
public:

    void SetUp() {

        strike = 95;

        //These represent averages of the asset price
        S_ITM = 200;
        S_ATM = strike;
        S_OTM = 30;
    }

    double strike{};
    double S_ITM{};
    double S_ATM{};
    double S_OTM{};

};

TEST_F(ComputeAsianCallPayoff, ComputePayoff)
{
    ArithmeticAsianCall c(strike, 1, false);

    double payoff_ITM = c.ComputePayoff(S_ITM);
    double payoff_ATM = c.ComputePayoff(S_ATM);
    double payoff_OTM = c.ComputePayoff(S_OTM);

    ASSERT_EQ(std::max(S_ITM - strike, 0.0), payoff_ITM);
    ASSERT_EQ(std::max(S_ATM - strike, 0.0), payoff_ATM);
    ASSERT_EQ(std::max(S_OTM - strike, 0.0), payoff_OTM);

}

class ComputeAsianCallPayoffPath: public testing::Test
{

public:
    void SetUp()
    {

    	strike = 95;
		path_ITM = { 200,200,200 };
		path_ATM = {strike,strike,strike };
		path_OTM = { 30,30,30 };

        avg_ITM = VectorStatisticalFunctions::VectorMean(path_ITM);
        avg_ATM = VectorStatisticalFunctions::VectorMean(path_ATM);
        avg_OTM = VectorStatisticalFunctions::VectorMean(path_OTM);
	    
    }

    double strike{};
    double avg_ITM{};
    double avg_ATM{};
    double avg_OTM{};
    std::vector<double> path_ITM{};
    std::vector<double> path_ATM{};
    std::vector<double> path_OTM{};

};

TEST_F(ComputeAsianCallPayoffPath,ComputePayoffPath)
{
    ArithmeticAsianCall c(strike, 1, false);

    double payoff_ITM = c.ComputePayoffPath(path_ITM);

    double payoff_ATM = c.ComputePayoffPath(path_ATM);

    double payoff_OTM = c.ComputePayoffPath(path_OTM);

    ASSERT_EQ(std::max(avg_ITM - strike, 0.0), payoff_ITM);
    ASSERT_EQ(std::max(avg_ATM - strike, 0.0), payoff_ATM);
    ASSERT_EQ(std::max(avg_OTM - strike, 0.0), payoff_OTM);

}

TEST(ComputeAsianCallEarlyExercise, IsEarlyExerciseTime)
{
    ArithmeticAsianCall c(95, 1, false);

    bool isEarlyExercise = c.IsEarlyExercise();

    ASSERT_EQ(false, isEarlyExercise);

}

TEST(ComputeAsianCallExerciseTime,IsExerciseTime)
{
    double Time = 10;
    ArithmeticAsianCall c(95, Time ,false);
    double early = Time - 9;

    bool resultEarly = c.IsExerciseTime(early);
    bool resultT = c.IsExerciseTime(Time);

    ASSERT_EQ(false, resultEarly);
    ASSERT_EQ(true, resultT);
}