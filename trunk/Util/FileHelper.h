#pragma once
#include <string>
using namespace std;

class cFileHelper
{
public:
	int GetFileName(const char* pFileName, char* pTitle, int MaxTitleSize);
	int GetFileNameTitle(const char* pFileName, char* pTitle, int MaxTitleSize);
	int GetFileExtent(const char* pFileName, char* pExtent, int MaxExtentSize);

	std::string GetFileName(const char* Path);

	cFileHelper();
	~cFileHelper();
};

