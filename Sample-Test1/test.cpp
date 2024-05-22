#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/testscript.cpp"

using namespace std;
using namespace testing;

class MockSSD : public SSD {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

class TestScriptApp1Fixture : public Test
{
public:
	NiceMock<MockSSD> mockSsd;
};

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullWrite) {
	TestScriptApp1 testScriptApp1(&mockSsd);
	
	EXPECT_CALL(mockSsd, Write)
		.Times(testScriptApp1.nSSDSize);
	testScriptApp1.DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullRead) {
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.Times(testScriptApp1.nSSDSize);
	testScriptApp1.DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_FailReadVerify) {
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(testScriptApp1.DoScript(), Eq(false));
}

class SSDFixture : public testing::Test {
public:
	SSD ssd;

	const int INVALID_DATA = 0x00000000;
};

TEST_F(SSDFixture, ReadLBANeverBeenWritten) {
	unsigned int nAddr = 0x0;

	EXPECT_EQ(INVALID_DATA, ssd.Read(nAddr));
}