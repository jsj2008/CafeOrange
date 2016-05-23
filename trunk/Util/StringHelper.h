#pragma once



#include <tchar.h>
#include <string>
#include <vector>
using namespace std;

class cStringHelper
{
public:
	typedef std::basic_string<TCHAR> tstring;
	std::vector<std::string> GetTokens(const std::string& Str, const std::string& Delimiters = std::string(" "));

	TCHAR*		StringToTCHAR(std::string& s);
	std::string		TCHARToString(const TCHAR* ptsz);

	cStringHelper();
	~cStringHelper();
};

