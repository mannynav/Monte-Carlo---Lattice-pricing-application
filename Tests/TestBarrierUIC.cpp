#include "BarrierUIC.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterX.h"
#include "ParameterT.h"

#include "VecB.h"
#include "utility.h"

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Tests
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ComputeBarrierUICPayoff : public testing::Test
{
protected:
	void SetUp() override
	{
		strike = 95;
		S_ITM = 1000;
		S_ATM = strike;
		S_OTM = 0;
	}

	double strike{};
	double S_ITM{};
	double S_ATM{};
	double S_OTM{};
};
TEST_F(ComputeBarrierUICPayoff, ComputePayoff)
{
	BarrierUIC c(strike, 1, 100, false);

	double payoff_ITM = c.ComputePayoff(S_ITM);
	double payoff_ATM = c.ComputePayoff(S_ATM);
	double payoff_OTM = c.ComputePayoff(S_OTM);

	ASSERT_EQ(std::max(S_ITM - strike, 0.0), payoff_ITM);
	ASSERT_EQ(std::max(S_ATM - strike, 0.0), payoff_ATM);
	ASSERT_EQ(std::max(S_OTM - strike, 0.0), payoff_OTM);
}


class BarrierUICVariousInputs : public testing::TestWithParam<std::vector<double>>
{
public:
	bool hitBarrier(const std::vector<double>& path, double barrier)
	{
		bool result = false;

		for (auto i : path)
		{
			if (i >= barrier)
			{
				result = true;
				break;
			}
		}
		return result;
	}
};
TEST_P(BarrierUICVariousInputs,HitBarrierWithVariousInputs)
{
	auto in = GetParam();
	double barrier = 5;
	bool expected = hitBarrier(in, barrier);

	BarrierUIC c(100, 1, barrier, false);

	EXPECT_EQ(expected, c.hitBarrier(in, barrier));
}
INSTANTIATE_TEST_SUITE_P(HitBarrierWithVariousInputs, BarrierUICVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 3000},
	std::vector<double>{10, 20, 40},
	std::vector<double>{100, 120, 140}));



class BarrierUICPayoffPath : public testing::TestWithParam<std::vector<double>>{};
TEST_P(BarrierUICPayoffPath, ComputePayoffPathForVariousInputs)
{
	auto in = GetParam();
	double strike = 100;
	double barrier = 150;
	BarrierUIC c(strike, 1, barrier, false);


	double payoff = c.ComputePayoffPath(in);
	double expected = 0.0;
	if(c.hitBarrier(in, barrier))
	{
		expected = std::max(in.back() - strike, 0.0);
	}


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(ComputePayoffPathWithVariousInputs, BarrierUICPayoffPath, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 120},
	std::vector<double>{10, 20, 155,40},
	std::vector<double>{100, 120, 110}));


TEST(BarrierUIC, IsEarlyExercise)
{
	
	BarrierUIC c1(100, 1, 150,false);
	BarrierUIC c2(100, 1, 150,true);


	bool expected1 = false;
	bool expected2 = true;
	bool result1 = c1.IsEarlyExercise();
	bool result2 = c2.IsEarlyExercise();


	EXPECT_EQ(expected1, result1);
	EXPECT_EQ(expected2, result2);

}

TEST(BarrierUIC, IsExerciseTime)
{
	double expiry = 1;
	double early_time = 1 - expiry;
	BarrierUIC c(100, expiry, 150, false);


	bool result1 = c.IsExerciseTime(early_time);
	bool result2 = c.IsExerciseTime(expiry);


	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(result2, testing::IsTrue());
}