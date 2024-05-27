#include "RunnerFileHandler.h"

vector<string>& RunnerFileHandler::GetCommandListFromTheRunnerFile()
{
	string sIndex;
	ifstream fin(runnerFileName);

	while (!fin.eof())
	{
		fin >> sIndex;
		if (fin.eof()) break;
		commandList.push_back(sIndex);
	}
	fin.close();
	return commandList;
}

void RunnerFileHandler::MakeFilePath(string fileName)
{
	const string currentPath = "./";
	runnerFileName = currentPath + fileName + ".txt";
}

bool RunnerFileHandler::IsRunnerListFileExist(string fileName)
{
	MakeFilePath(fileName);
	return _access(&*runnerFileName.begin(), 0) == 0;
}