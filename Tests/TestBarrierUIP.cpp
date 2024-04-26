
#include "BarrierUIP.h"

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

TEST(TestBarrierUIPMultiArgConstructor, BarrierUIP)
{
	BarrierUIP p(100, 1, 150, false);

	EXPECT_THAT(p.GetX(),testing::Eq(100));
	EXPECT_THAT(p.GetT(),testing::Eq(1));
	EXPECT_THAT(p.GetK(),testing::Eq(150));
	EXPECT_THAT(p.GetEarly(),testing::IsFalse());

}


class ComputeBarrierUIPPayoff : public testing::Test
{
protected:
	void SetUp() override
	{
		strike = 110;
		S_ITM = 90;
		S_ATM = strike;
		S_OTM = 160;
	}

	double strike{};
	double S_ITM{};
	double S_ATM{};
	double S_OTM{};
};
TEST_F(ComputeBarrierUIPPayoff, ComputePayoff)
{
	BarrierUIP p(strike, 1, 100, false);

	double payoff_ITM = p.ComputePayoff(S_ITM);
	double payoff_ATM = p.ComputePayoff(S_ATM);
	double payoff_OTM = p.ComputePayoff(S_OTM);

	ASSERT_EQ(std::max(strike-S_ITM, 0.0), payoff_ITM);
	ASSERT_EQ(std::max(strike-S_ATM, 0.0), payoff_ATM);
	ASSERT_EQ(std::max(strike-S_OTM, 0.0), payoff_OTM);
}


class BarrierUIPVariousInputs : public testing::TestWithParam<std::vector<double>>
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
TEST_P(BarrierUIPVariousInputs, HitBarrierWithVariousInputs)
{
	auto in = GetParam();
	double barrier = 120;
	bool expected = hitBarrier(in, barrier);


	BarrierUIP p(100, 1, barrier, false);


	EXPECT_EQ(expected, p.hitBarrier(in, barrier));
}
INSTANTIATE_TEST_SUITE_P(HitBarrierWithVariousInputs, BarrierUIPVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 3000},
	std::vector<double>{10, 20, 40},
	std::vector<double>{100, 110, 140}));



class BarrierUIPPayoffPath : public testing::TestWithParam<std::vector<double>> {};
TEST_P(BarrierUIPPayoffPath, ComputePayoffPathForVariousInputs)
{
	double strike = 100;
	double barrier = 150;
	auto in = GetParam();
	BarrierUIP p(strike, 1, barrier, false);


	double expected = 0.0;
	double payoff = p.ComputePayoffPath(in);
	if (p.hitBarrier(in, barrier))
	{
		expected = std::max(strike - in.back(), 0.0);
	}


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(ComputePayoffPathWithVariousInputs, BarrierUIPPayoffPath, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 120},
	std::vector<double>{10, 20, 155, 90},
	std::vector<double>{100, 120, 110}));


TEST(BarrierUIP, IsEarlyExercise)
{

	BarrierUIP p1(100, 1, 150, false);
	BarrierUIP p2(100, 1, 150, true);


	bool result1 = p1.IsEarlyExercise();
	bool result2 = p2.IsEarlyExercise();


	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(!result1,testing::IsTrue());


	EXPECT_THAT(result2, testing::IsTrue());
	EXPECT_THAT(!result2,testing::IsFalse());
}

TEST(BarrierUIP, IsExerciseTime)
{
	double expiry = 1;
	double early_time = 1 - expiry;
	BarrierUIP p(100, expiry, 150, false);


	bool result1 = p.IsExerciseTime(early_time);
	bool result2 = p.IsExerciseTime(expiry);


	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(result2, testing::IsTrue());
}