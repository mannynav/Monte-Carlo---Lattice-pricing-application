
#include "CashOrNothingPut.h"

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

TEST(CashOrNothingPutMultiArgConstructor, CashOrNothingPut)
{
	CashOrNothingPut p(100, 1, 150, false);

	EXPECT_THAT(p.GetX(), testing::Eq(100));
	EXPECT_THAT(p.GetT(), testing::Eq(1));
	EXPECT_THAT(p.GetQ(), testing::Eq(150));
	EXPECT_THAT(p.GetEarly(), testing::IsFalse());

}

TEST(ComputeCashOrNothingPutPayoff, ComputePayoff)
{
	double notional = 120;
	CashOrNothingPut p(100, 1, notional, false);

	double payoff1 = p.ComputePayoff(90);
	double payoff2 = p.ComputePayoff(100);
	double payoff3 = p.ComputePayoff(160);

	ASSERT_EQ(notional, payoff1);
	ASSERT_EQ(notional, payoff2);
	ASSERT_EQ(notional, payoff3);
}

class CashOrNothingPutVariousInputs : public testing::TestWithParam<std::vector<double>> {};
TEST_P(CashOrNothingPutVariousInputs, ComputePayoffPathsWithVariousInputs)
{
	double notional = 110;
	auto in = GetParam();
	CashOrNothingPut p(100, 1, notional, false);


	double expected = 0;
	if (in.back() < p.GetX())
	{
		expected = notional;
	}
	double payoff = p.ComputePayoffPath(in);


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(HitBarrierWithVariousInputs, CashOrNothingPutVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 1500},
	std::vector<double>{40, 80, 120},
	std::vector<double>{100, 110, 140}));

TEST(CashOrNothingPutEarlyExercise, IsEarlyExercise)
{
	CashOrNothingPut p1(100, 1, 150, false);
	CashOrNothingPut p2(100, 1, 150, true);

	bool result1 = p1.IsEarlyExercise();
	bool result2 = p2.IsEarlyExercise();

	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(result2, testing::IsTrue());

}

TEST(CashOrNothingPutIsExerciseTime, IsExerciseTime)
{
	double expiry = 1;
	CashOrNothingPut p(100, expiry, 150, false);

	bool result1 = p.IsExerciseTime(expiry);
	bool result2 = p.IsExerciseTime(expiry - 0.5);

	EXPECT_THAT(result1, testing::IsTrue());
	EXPECT_THAT(result2, testing::IsFalse());
}

