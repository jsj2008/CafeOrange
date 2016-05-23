#pragma once
#include <map>
#include <string>

using namespace std;

class bgProperty
{
public:
	bgProperty();
	virtual ~bgProperty();

	std::map<string, string> messageMap;


protected:
};

