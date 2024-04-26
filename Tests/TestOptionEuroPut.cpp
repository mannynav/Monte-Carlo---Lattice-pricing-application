

#include "OptionEuroPut.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterX.h"
#include "ParameterT.h"

#include "VecB.h"
#include "utility.h"

#include <algorithm>
#include <vector>

#include <gtest\gtest.h>
#include <gmock\gmock.h>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Tests
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


class ComputePutPayoff :public testing::Test
{
protected:

    void SetUp() {

        strike = 95;
        S_ITM = 10;
        S_ATM = strike;
        S_OTM = 200;
    }

    double strike{};
    double S_ITM{};
    double S_ATM{};
    double S_OTM{};
};
TEST_F(ComputePutPayoff, ComputePutPayoff)
{
    OptionEuroPut p(strike, 1, false);

    double payoff_ITM = p.ComputePayoff(S_ITM);
    double payoff_ATM = p.ComputePayoff(S_ATM);
    double payoff_OTM = p.ComputePayoff(S_OTM);

    ASSERT_EQ(std::max(strike - S_ITM, 0.0), payoff_ITM);
    ASSERT_EQ(std::max(strike - S_ATM, 0.0), payoff_ATM);
    ASSERT_EQ(std::max(strike - S_OTM, 0.0), payoff_OTM);
}

class ComputePutPayoffPath : public testing::Test
{
public:
    void SetUp() override
    {
        double strike = 95;

        vector_OTM.push_back(1000);
        vector_OTM.push_back(2000);

        vector_ATM.push_back(100);
        vector_ATM.push_back(strike);

        vector_0.push_back(100);
        vector_0.push_back(0);

    }

    double strike;
    std::vector<double> vector_OTM;
    std::vector<double> vector_ATM;
    std::vector<double> vector_0;
};
TEST_F(ComputePutPayoffPath, ComputePutPayoffPath)
{
    OptionEuroPut p(strike, 1, false);

    double payoff_OTM = p.ComputePayoffPath(vector_OTM);
    double payoff_ATM = p.ComputePayoffPath(vector_ATM);
    double payoff_0 = p.ComputePayoffPath(vector_0);

    EXPECT_EQ(std::max(strike - vector_OTM.back(), 0.0), payoff_OTM);
    EXPECT_EQ(std::max(strike - vector_ATM.back(), 0.0), payoff_ATM);
    EXPECT_EQ(std::max(strike - vector_0.back(), 0.0), payoff_0);

}

class ComputePutPayoffSlice : public testing::Test
{
public:
    void SetUp() override
    {
       
        strike = 95;
        S_OTM = 1000;
        S_ATM = strike;
        S_0 = 0;

        v1.push_back(S_OTM);
        v1.push_back(S_ATM);
        v1.push_back(S_0);
    }

    VecB v1;
    double strike;
    double S_OTM;
    double S_ATM;
    double S_0;
};
TEST_F(ComputePutPayoffSlice, ComputePutPayoffSlice)
{
    OptionEuroPut p(strike, 1, false);
    VecB v2(0, 3, 0.0);

    p.ComputePayoffSlice(0, v1, v2);

    //v2 should now be filled with the payoffs using s values in v1
    ASSERT_EQ(std::max(strike - S_OTM, 0.0), v2[0]);
    ASSERT_EQ(std::max(strike - S_ATM, 0.0), v2[1]);
    ASSERT_EQ(std::max(strike - S_0, 0.0), v2[2]);
}


TEST(IsExerciseTimeForPut, IsExerciseTimeForPut)
{
    OptionEuroPut p(100, 1, false);
    double expiry = 1.0;
    double pre_expiry_ = expiry - 0.5;

    bool result = p.IsExerciseTime(pre_expiry_);

    ASSERT_EQ(false, result);
}
