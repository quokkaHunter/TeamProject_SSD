#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <fstream>
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/SSDFileHandler.cpp"
#include "../TeamProject_SSD/testscript.cpp"
#include "../TeamProject_SSD/ShellTestApp.cpp"
#include "../TeamProject_SSD/RunnerFileHandler.cpp"
#include "../TeamProject_SSD/Command.cpp"
#include "../TeamProject_SSD/CommandFactory.cpp"
#include "../TeamProject_SSD/Runner.cpp"
#include "../TeamProject_SSD/Logger.cpp"
#include "../TeamProject_SSD/SSDCommandBuffer.cpp"

using namespace std;
using namespace testing;

class MockSSD : public SSD {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
	MOCK_METHOD(void, Erase, (unsigned int, unsigned int), (override));
	MOCK_METHOD(int, GetSSDSize, (), (override));
};

class MockTestScript : public ITestScript {
public:
	MOCK_METHOD(bool, DoScript, (), (override));
};

class ShellTestAppFixture : public testing::Test {
public:
	void SetUp() override {
		pApp = new ShellTestApp(&mSsd);
	}

	ShellTestApp* pApp;
	MockSSD mSsd;
	MockTestScript mTestScript;

	const unsigned int LBA = 0;
	const unsigned int SIZE = 3;
	const unsigned int MAX_LBA_NUM = 100; //todo get MAX_LBA_NUM from SSD
	const unsigned int DATA = 0xFFFFFFFF;
	const unsigned int INVALID_DATA = 0X00000000;
	const std::exception ERREXCEPTION;
	const std::string TESTSCRIPT1 = "testscriptapp2";
	const std::string TESTSCRIPT2 = "testscriptapp1";
};

class TestScriptAppFixture : public Test
{
public:
	const string SCRIPT_APP1 = "testscriptapp1";
	const string SCRIPT_APP2 = "testscriptapp2";
	const string SCRIPT_INVALID = "invalid_script";

	NiceMock<MockSSD> mockSSD;
	TestScriptFactory fTestScriptFactory;
	TestScript* pTestScript;
protected:
	void MakeScript(string scriptName)
	{
		pTestScript = fTestScriptFactory.createScript(scriptName, mockSSD);
	}

};

class SSDFixture : public testing::Test
{
public:
	void SetUp() override
	{
		remove(sResultFileName.c_str());
		remove(sNANDFileName.c_str());
		remove(sCommandBufferFileName.c_str());
	}

	CMD_BUFFER_MAP LoadCmdBuffer()
	{
		CMD_BUFFER_MAP nCmdBuffer;
		ifstream fin(sCommandBufferFileName);
		string sCmdType, sLBA, sValue;

		if (fin.is_open())
		{
			while (!fin.eof())
			{
				fin >> sCmdType >> sLBA >> sValue;

				int nCmdType = sCmdType == "W" ? W : E;
				int nLBA = stoi(sLBA);
				unsigned int nValue = stoul(sValue, nullptr, 16);

				nCmdBuffer[{ nCmdType, nLBA }] = nValue;
			}
		}
		return nCmdBuffer;
	}

	SSD ssd;
	CMD_BUFFER_MAP nCmdBuffer;

	const unsigned int DEFAULT_READ_VALUE = 0x00000000;
	const unsigned int INVALID_LBA = 0xFF;
	const unsigned int LBA_0 = 0;

	string sResultFileName = "result.txt";
	string sNANDFileName = "nand.txt";
	string sCommandBufferFileName = "buffer.txt";
};