#include "ssd.h"

unsigned int SSD::Read(unsigned int nLBA)
{
	ValidateParameter(nLBA);

	unsigned int nReadValue = DEFAULT_READ_VALUE;
	
	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		nReadValue = umDataSet[nLBA];
	}

	ssdFileHandler.WriteHexReadValueToResultFile(nReadValue);

	return nReadValue;
}

void SSD::Write(unsigned int nLBA, unsigned int nValue)
{
	ValidateParameter(nLBA);

	AddCommandToBuffer(W, nLBA, nValue);

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		umDataSet[nLBA] = nValue;
	}
	else
	{
		umDataSet.insert({ nLBA, nValue });
	}

	ssdFileHandler.WriteNANDFile(umDataSet);
}

void SSD::Erase(unsigned int nLBA, unsigned int nSize)
{
	if (nLBA < 0 || nLBA + nSize - 1 > SSD_MAX_LBA)
	{
		throw exception("INVALID COMMAND");
	}

	AddCommandToBuffer(E, nLBA, nSize);

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	for (int i = 0; i < nSize; i++)
	{
		umDataSet.erase(nLBA + i);
	}

	if (umDataSet.empty())
	{
		remove("nand.txt");
		return;
	}

	ssdFileHandler.WriteNANDFile(umDataSet);
}

int SSD::GetSSDSize()
{
	return SSD_MAX_LBA + 1; // LBA is 0 base.
}

void SSD::ValidateParameter(unsigned int nLBA)
{
	if (nLBA < 0 || nLBA > SSD_MAX_LBA)
	{
		throw exception("INVALID COMMAND");
	}
}

bool SSD::IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet)
{
	return umDataSet.find(nLBA) != umDataSet.end();
}

void SSD::AddCommandToBuffer(int nCmdType, int nLBA, unsigned int nData)
{
	unordered_map<MyKey, unsigned int> nCmdBuffer;

	ssdFileHandler.LoadCommandBufferFile(nCmdBuffer);

	nCmdBuffer[{ nCmdType, nLBA }] = nData;

	ssdFileHandler.WriteCommandBufferFile(nCmdBuffer);
}