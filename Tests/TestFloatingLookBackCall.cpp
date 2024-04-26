
#include "FloatingLookBackCall.h"

#include "IO_Manager.h"
#include "FactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterX.h"
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

TEST(FloatingLBCallMultiArgConstructor, TestConstructorFloatingLBCall)
{
	double expiry = 1;
	double sMin = 90;
	bool early = false;
	FloatingLookBackCall c(expiry, sMin, early);

	EXPECT_THAT(c.GetT(), testing::Eq(expiry));
	EXPECT_THAT(c.GetMinS(), testing::Eq(sMin));
	EXPECT_THAT(c.GetEarly(), early);
}

TEST(FloatingLBCallPayoff, ComputePayoff)
{
	double minS = 120;
	FloatingLookBackCall c(1, minS, false);

	double payoff1 = c.ComputePayoff(90);
	double payoff2 = c.ComputePayoff(100);
	double payoff3 = c.ComputePayoff(160);

	ASSERT_EQ(std::max(90-minS,0.0), payoff1);
	ASSERT_EQ(std::max(100-minS,0.0), payoff2);
	ASSERT_EQ(std::max(160-minS,0.0), payoff3);
	
}

class FloatingLBCallVariousInputs : public testing::TestWithParam<std::vector<double>> {};
TEST_P(FloatingLBCallVariousInputs, ComputePayoffPathsWithVariousInputs)
{
	double minS = 110;
	auto in = GetParam();
	FloatingLookBackCall c(1, minS, false);


	double vectorMin = vs::VectorMinimum(in);
	double expected = std::max(in.back() - vectorMin, 0.0);
	double payoff = c.ComputePayoffPath(in);


	EXPECT_EQ(expected, payoff);

}
INSTANTIATE_TEST_SUITE_P(FloatingLBCallPayoffPath, FloatingLBCallVariousInputs, testing::Values(
	std::vector<double>{0, 0, 0, 0},
	std::vector<double>{1000, 2000, 1500},
	std::vector<double>{40, 80, 120},
	std::vector<double>{100, 110, 140}));

TEST(FloatingLBCallEarlyExercise, IsEarlyExercise)
{
	FloatingLookBackCall c(1, 150, false);
	FloatingLookBackCall c2(1, 150, true);

	bool result1 = c.IsEarlyExercise();
	bool result2 = c2.IsEarlyExercise();

	EXPECT_THAT(result1, testing::IsFalse());
	EXPECT_THAT(result2, testing::IsTrue());

}

TEST(FloatingLBCallIsExerciseTime, IsExerciseTime)
{
	double expiry = 1;
	double pre_expiry = 1 - 0.5;
	FloatingLookBackCall c(expiry, 150, false);

	bool result1 = c.IsExerciseTime(expiry);
	bool result2 = c.IsExerciseTime(expiry - 0.5);

	EXPECT_THAT(result1, testing::IsTrue());
	EXPECT_THAT(result2, pre_expiry == expiry);
}

