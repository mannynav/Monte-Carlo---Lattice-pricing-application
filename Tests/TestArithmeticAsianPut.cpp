
#include "ArithmeticAsianPut.h"

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

class ComputeAsianPutPayoff :public testing::Test
{
public:

    void SetUp() {

        strike = 95;

        //These represent averages of the asset price
        S_ITM = 10;
        S_ATM = strike;
        S_OTM = 150;
    }

    double strike{};
    double S_ITM{};
    double S_ATM{};
    double S_OTM{};

};

TEST_F(ComputeAsianPutPayoff, ComputePayoff)
{
    ArithmeticAsianPut p(strike, 1, false);

    double payoff_ITM = p.ComputePayoff(S_ITM);
    double payoff_ATM = p.ComputePayoff(S_ATM);
    double payoff_OTM = p.ComputePayoff(S_OTM);

    ASSERT_EQ(std::max(strike - S_ITM, 0.0), payoff_ITM);
    ASSERT_EQ(std::max(strike - S_ATM, 0.0), payoff_ATM);
    ASSERT_EQ(std::max(strike - S_OTM, 0.0), payoff_OTM);

}

class ComputeAsianPutPayoffPath : public testing::Test
{

public:
    void SetUp()
    {
        strike = 95;
        path_ITM = { 30,30,30};
        path_ATM = { strike,strike,strike };
        path_OTM = { 150,150,150 };

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

TEST_F(ComputeAsianPutPayoffPath, ComputePayoffPath)
{
    ArithmeticAsianPut p(strike, 1, false);

    double payoff_ITM = p.ComputePayoffPath(path_ITM);
    double payoff_ATM = p.ComputePayoffPath(path_ATM);
    double payoff_OTM = p.ComputePayoffPath(path_OTM);

    ASSERT_EQ(std::max(strike-avg_ITM, 0.0), payoff_ITM);
    ASSERT_EQ(std::max(strike-avg_ATM, 0.0), payoff_ATM);
    ASSERT_EQ(std::max(strike-avg_OTM, 0.0), payoff_OTM);

}

TEST(ComputeAsianPutEarlyExercise, IsEarlyExerciseTime)
{
    ArithmeticAsianPut p(95, 1, false);

    bool isEarlyExercise = p.IsEarlyExercise();

    ASSERT_EQ(false, isEarlyExercise);

}

TEST(ComputeAsianPutExerciseTime, IsExerciseTime)
{
    double Time = 10;
    ArithmeticAsianPut p(95, Time, false);
    double early = Time - 9;

    bool resultEarly = p.IsExerciseTime(early);
    bool resultT = p.IsExerciseTime(Time);

    ASSERT_EQ(false, resultEarly);
    ASSERT_EQ(true, resultT);
}