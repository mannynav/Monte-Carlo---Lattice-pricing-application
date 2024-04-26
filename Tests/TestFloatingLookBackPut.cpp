
#include "FloatingLookBackPut.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterT.h"

#include "VecB.h"
#include "VectorStatistics.h"
#include "utility.h"

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Tests
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

TEST(FloatingLBPutMultiArgConstructor, TestConstructorFloatingLBPut)
{
	double expiry = 1;
	double sMax = 90;
	bool early = false;
	FloatingLookBackPut p(expiry, sMax, early);

	EXPECT_THAT(p.GetT(), testing::Eq(expiry));
	EXPECT_THAT(p.GetMaxS(), testing::Eq(sMax));
	EXPECT_THAT(p.GetEarly(), early);
}

TEST(FloatingLBPutPayoff, ComputePayoff)
{
	double maxS = 120;
	FloatingLookBackPut p(1, maxS, false);

	double payoff1 = p.ComputePayoff(90);
	double payoff2 = p.ComputePayoff(100);
	double payoff3 = p.ComputePayoff(160);

	ASSERT_EQ(std::max(maxS-90, 0.0), payoff1);
	ASSERT_EQ(std::max(maxS-100, 0.0), payoff2);
	ASSERT_EQ(std::max(maxS-160, 0.0), payoff3);

}

class FloatingLBPutVariousInputs : public testing::TestWithParam<std::vector<double>> {};
TEST_P(FloatingLBPutVariousInputs, ComputePayoffPathsWithVariousInputs)
{
	double maxS = 110;
	auto in = GetParam();
	FloatingLookBackPut p(1, maxS, false);


	double vectorMax = vs::VectorMaximum(in);
	double expected = std::max(vectorMax-in.back(), 0.0);
	double payoff = p.ComputePayoffPath(in);


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(FloatingLBPutPayoffPath, FloatingLBPutVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1500,110,1200},
	std::vector<double>{60, 120, 134},
	std::vector<double>{100, 110, 140}));

TEST(FloatingLBPutEarlyExercise, IsEarlyExercise)
{
	bool earlyF = false;
	bool earlyT = true;
	FloatingLookBackPut p(1, 150, earlyF);
	FloatingLookBackPut p2(1, 150, earlyT);

	bool result1 = p.IsEarlyExercise();
	bool result2 = p2.IsEarlyExercise();

	EXPECT_THAT(result1, testing::Eq(earlyF));
	EXPECT_THAT(result2, testing::Eq(earlyT));

}

TEST(FloatingLBPutIsExerciseTime, IsExerciseTimeFalse)
{
	double expiry = 1;
	double pre_expiry = 1 - 0.5;
	FloatingLookBackPut p(expiry, 150, false);

	bool result = p.IsExerciseTime(pre_expiry);

	EXPECT_THAT(result, testing::IsFalse());
}

TEST(FloatingLBPutIsExerciseTime, IsExerciseTimeTrue)

{
	double expiry = 1;
	double input_expiry = 1;
	FloatingLookBackPut p(expiry, 150, false);

	bool result = p.IsExerciseTime(input_expiry);

	EXPECT_THAT(result, input_expiry == expiry);
}

