#include "testscript.h"

bool TestScriptApp1::DoScript()
{
	unsigned int nWriteValue = 0x0;

	FullWrite(nWriteValue);
	return FullReadVerify(nWriteValue);
}

bool TestScriptApp1::FullReadVerify(unsigned int nWriteValue)
{
	for (int i = 0; i < ssd->GetSSDSize(); i++)
	{
		if (ssd->Read(i) != nWriteValue)
		{
			return false;
		}
	}
	return true;
}

void TestScriptApp1::FullWrite(unsigned int nWriteValue)
{
	for (int i = 0; i < ssd->GetSSDSize(); i++)
	{
		ssd->Write(i, nWriteValue);
	}
}

bool TestScriptApp2::DoScript()
{
	FirstWrite();
	OverWrite();
	if (Verify() != true)
	{
		return false;
	}
	return true;
}
void TestScriptApp2::FirstWrite()
{
	unsigned int nWriteValue = 0xAAAABBBB;
	for (int nLoop = 0; nLoop < 30; nLoop++)
	{
		for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
		{
			ssd->Write(nLba, nWriteValue);
			pTestData[nLba] = nWriteValue;
		}
	}
}
void TestScriptApp2::OverWrite()
{
	unsigned int nWriteValue = 0x12345678;
	for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
	{
		ssd->Write(nLba, nWriteValue);
		pTestData[nLba] = nWriteValue;
	}
}
bool TestScriptApp2::Verify()
{
	for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
	{
		if (pTestData[nLba] != ssd->Read(nLba))
		{
			return false;
		}
	}
	return true;
}

bool FullWriteReadCompare::DoScript()
{
	for (register int nandFileOffset = 0; nandFileOffset < ssd->GetSSDSize(); nandFileOffset++)
	{
		ssd->Write(nandFileOffset, nRefDataForTestScenario);
	}

	for (register int nandFileOffset = 0; nandFileOffset < ssd->GetSSDSize(); nandFileOffset++)
	{
		if (ssd->Read(nandFileOffset) != nRefDataForTestScenario)
		{
			return false;
		}
	}

	return true;
}

bool FullRead10AndCompare::DoScript()
{
	for (register int nandFileOffset = 0; nandFileOffset < ssd->GetSSDSize(); nandFileOffset++)
	{
		ssd->Write(nandFileOffset, nRefDataForTestScenario);
	}

	for (int fullReadCount = 0; nTestScenarioLoopCount; fullReadCount++)
	{
		for (register int nandFileOffset = 0; nandFileOffset < ssd->GetSSDSize(); nandFileOffset++)
		{
			ssd->Read(nandFileOffset);
		}
	}

	for (register int nandFileOffset = 0; nandFileOffset < ssd->GetSSDSize(); nandFileOffset++)
	{
		if (ssd->Read(nandFileOffset) != nRefDataForTestScenario)
		{
			return false;
		}
	}
	return true;
}

bool Write10AndCompare::DoScript()
{
	for (int testCount = 0; testCount < nTestScenarioLoopCount; testCount++)
	{
		ssd->Write(nTargetAddrForTestScenario, nRefDataForTestScenario);
	}
	if (nRefDataForTestScenario != ssd->Read(nTargetAddrForTestScenario))
	{
		return false;
	}
	return true;
}

bool Loop_WriteAndReadCompare::DoScript()
{
	for (int testCount = 0; testCount < nTestScenarioLoopCount; testCount++)
	{
		ssd->Write(nTargetAddrForTestScenario, nRefDataForTestScenario);
		if (nRefDataForTestScenario != ssd->Read(nTargetAddrForTestScenario))
		{
			return false;
		}
	}
	return true;
}

TestScript* TestScriptFactory::createScript(string sScriptName, SSD& ssd)
{
	if (sScriptName.compare("testscriptapp2") == 0)
	{
		return new TestScriptApp2(&ssd);
	}
	else if (sScriptName.compare("testscriptapp1") == 0)
	{
		return new TestScriptApp1(&ssd);
	}
	else if (sScriptName.compare("FullWriteReadCompare") == 0)
	{
		return new FullWriteReadCompare(&ssd);
	}
	else if (sScriptName.compare("FullRead10AndCompare") == 0)
	{
		return new FullRead10AndCompare(&ssd);
	}
	else if (sScriptName.compare("Write10AndCompare") == 0)
	{
		return new Write10AndCompare(&ssd);
	}
	else if (sScriptName.compare("Loop_WriteAndReadCompare") == 0)
	{
		return new Loop_WriteAndReadCompare(&ssd);
	}
	return nullptr;
}
