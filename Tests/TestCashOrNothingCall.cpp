
#include "CashOrNothingCall.h"

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

TEST(CashOrNothingCallMultiArgConstructor, CashOrNothingCall)
{
	CashOrNothingCall c(100, 1, 150, false);

	EXPECT_THAT(c.GetX(),testing::Eq(100));
	EXPECT_THAT(c.GetT(),testing::Eq(1));
	EXPECT_THAT(c.GetQ(),testing::Eq(150));
	EXPECT_THAT(c.GetEarly(),testing::IsFalse());

}

TEST(ComputeCashOrNothingCallPayoff, ComputePayoff)
{
	double notional = 120;
	CashOrNothingCall c(100, 1, notional, false);

	double payoff1 = c.ComputePayoff(90);
	double payoff2 = c.ComputePayoff(100);
	double payoff3 = c.ComputePayoff(160);

	ASSERT_EQ(notional, payoff1);
	ASSERT_EQ(notional, payoff2);
	ASSERT_EQ(notional, payoff3);
}

class CashOrNothingCallVariousInputs : public testing::TestWithParam<std::vector<double>>{};
TEST_P(CashOrNothingCallVariousInputs, ComputePayoffPathsWithVariousInputs)
{
	double notional = 110;
	auto in = GetParam();
	CashOrNothingCall c(100, 1, notional, false);


	double expected = 0;
	if(in.back()>c.GetX())
	{
		expected = notional;
	}
	double payoff = c.ComputePayoffPath(in);


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(HitBarrierWithVariousInputs, CashOrNothingCallVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000,1500},
	std::vector<double>{40,80,120},
	std::vector<double>{100, 110, 140}));

TEST(CashOrNothingCallEarlyExercise, IsEarlyExercise)
{
	CashOrNothingCall c1(100, 1, 150, false);
	CashOrNothingCall c2(100, 1, 150, true);

	bool result1 = c1.IsEarlyExercise();
	bool result2 = c2.IsEarlyExercise();

	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(!result1, testing::IsTrue());

	EXPECT_THAT(result2, testing::IsTrue());
	EXPECT_THAT(!result2, testing::IsFalse());

}

TEST(CashOrNothingCallIsExerciseTime, IsExerciseTime)
{
	double expiry = 1;
	CashOrNothingCall c(100, 1, 150, false);

	bool result1 = c.IsExerciseTime(1);
	bool result2 = c.IsExerciseTime(0.4);

	EXPECT_THAT(result1, testing::IsTrue());
	EXPECT_THAT(!result1, testing::IsFalse());

	EXPECT_THAT(result2, testing::IsFalse());
	EXPECT_THAT(!result2, testing::IsTrue());
}


