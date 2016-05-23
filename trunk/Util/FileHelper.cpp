#include "StdAfx.h"
#include <assert.h>
#include <tchar.h>
#include "FileHelper.h"


cFileHelper::cFileHelper()
{

}


cFileHelper::~cFileHelper()
{

}


int cFileHelper::GetFileName(const char* pFileName, char* pTitle, int MaxTitleSize)
{
	// Extract filetitle.extent from the filename. 
	// result string does NOT contain '.' character but contains terminating null character.
	// return value:
	//		if this function succeeded, it returns file name extent's length.
	//		or if it fails, returns -1(minus number)
	const char*	pSlash;

	if (MaxTitleSize <= 1){
		assert("insufficent buffer memory");
		return -1;
	}

	pSlash = _tcsrchr(pFileName, '\\');

	if (!pSlash){
		// the file name has no extent.
		pSlash = pFileName - 1;
	}

	int	NameSize;

	NameSize = (int)_tcslen(pSlash + 1);
	if (NameSize >= MaxTitleSize){
		assert("insufficent buffer memory");
		return -1;
	}

	//_tcscpy(pTitle, pSlash + 1);
	_tcscpy_s(pTitle, NameSize + 1, pSlash + 1);
	return NameSize;
}


int  cFileHelper::GetFileNameTitle(const char* pFileName, char* pTitle, int MaxBufferSize)
{
	assert(pFileName);
	if (!pFileName)
		return 0;
	intptr_t	BeginIndex, EndIndex;
	TCHAR*	pChar;

	pChar = const_cast<TCHAR*>(_tcsrchr(pFileName, '\\'));
	if (pChar){
		BeginIndex = pChar - pFileName + 1;
	}
	else{
		BeginIndex = 0;
	}

	pChar = const_cast<TCHAR*>(_tcsrchr(pFileName, '.'));
	if (pChar){
		EndIndex = pChar - pFileName;
		if (EndIndex < BeginIndex){
			EndIndex = _tcslen(pFileName);
		}
	}
	else{
		EndIndex = _tcslen(pFileName);
	}

	if (EndIndex - BeginIndex + 1 > MaxBufferSize)
		return (int)(EndIndex - BeginIndex + 1);

	_tcsncpy(pTitle, pFileName + BeginIndex, EndIndex - BeginIndex);
	pTitle[EndIndex - BeginIndex] = 0;


	return (int)(EndIndex - BeginIndex);
}


int cFileHelper::GetFileExtent(const char* pFileName, char* pExtent, int MaxExtentSize){
	// Extract extent from the filename. 
	// result string does NOT contain '.' character but contains terminating null character.
	// return value:
	//		if this function succeeded, it returns file name extent's length.
	//		or if it fails, returns -1(minus number)
	const char*	pDot;

	if (MaxExtentSize <= 1){
		return -1;
	}

	pDot = _tcsrchr(pFileName, '.');

	if (!pDot){
		// the file name has no extent.
		pExtent[0] = 0;
		return 0;
	}

	int	ExtentSize;

	ExtentSize = (int)_tcslen(pDot + 1);
	if (ExtentSize >= MaxExtentSize){
		return -1;
	}

	//_tcscpy(pExtent, pDot + 1);
	_tcscpy_s(pExtent, ExtentSize + 1, pDot + 1);
	return ExtentSize;
}


std::string cFileHelper::GetFileName(const char* Path){
	char Title[MAX_PATH] = { 0, };
	if (GetFileName(Path, Title, MAX_PATH) < 0){
		return std::string(_T(""));
	}

	return std::string(Title);
}