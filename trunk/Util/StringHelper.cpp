#include "StdAfx.h"
#include "StringHelper.h"


cStringHelper::cStringHelper()
{

}

cStringHelper::~cStringHelper()
{

}

std::vector<std::string> cStringHelper::GetTokens(const std::string& Str, const std::string& Delimiters)
{
	std::string::size_type LastPos = Str.find_first_not_of(Delimiters, 0);
	std::string::size_type Pos = Str.find_first_of(Delimiters, LastPos);
	std::vector<std::string> tokens;
	while (std::string::npos != Pos || std::string::npos != LastPos){
		std::string data;
		int Count = Pos - LastPos;
		if (Count == -1)
			data = Str.substr(LastPos, Str.size() - LastPos);
		else
			data = Str.substr(LastPos, Count);

		tokens.push_back(data);
		LastPos = Str.find_first_not_of(Delimiters, Pos);
		Pos = Str.find_first_of(Delimiters, LastPos);
	}

	return tokens;
}



TCHAR* cStringHelper::StringToTCHAR(std::string& s)
{
	tstring tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

std::string cStringHelper::TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////StringTokenizer
//cStringTokenizer::cStringTokenizer(CString str,CString sep)
//{
//	index=0;
//	count=0;
//	CString str1="";
//	for(int i=0;i<str.GetLength() && sep.GetLength()==1;i++)
//	{
//		if(str.GetAt(i)==sep.GetAt(0))
//		{
//			elements.Add(str1);
//			
//			str1="";
//		}
//		else
//		{
//			str1+=str.GetAt(i); 
//		}
//	}
//	elements.Add(str1);
//	count=elements.GetSize ();
//
//}
//
//CString cStringTokenizer::getNextElement()
//{
//	index++;
//	if(index==count)
//	{
//		throw CString("Index out of Bounds");
//	}
//	return elements.GetAt(index-1);	
//}
//int cStringTokenizer::countElements()
//{
//	return count;
//}
//
//CString cStringTokenizer::elementAt(int index)
//{
//	if(index>=count ||index<0)
//	{
//		throw CString("Index out of Bounds");
//	}
//	else
//		return elements.GetAt(index);	
//}