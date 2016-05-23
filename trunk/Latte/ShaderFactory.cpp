#include "StdAfx.h"
#include "FileHelper.h"
#include "ShaderFactory.h"

cShaderFactory* cShaderFactory::pShaderFactory = 0;
cShaderFactory::cShaderFactory()
{

}

cShaderFactory::~cShaderFactory()
{
	ShaderFactory::iterator iVShader = VertexFactory.begin();
	while(iVShader != VertexFactory.end()){
		delete (*iVShader).second;
		iVShader++;
	}

	VertexFactory.clear();

	ShaderFactory::iterator iFShader = FragmentFactory.begin();
	while(iFShader != FragmentFactory.end()){
		delete (*iFShader).second;
		iFShader++;
	}

	FragmentFactory.clear();
}
cShader* cShaderFactory::LoadShaderFromFile(const char *FileName)
{	
	char Path[MAX_PATH] = {0,};
	
	//WideCharToMultiByte(CP_ACP, 0, FileName, -1, Path, MAX_PATH, NULL, NULL);	

	GLint ShaderLength = 0;
	FILE *fp;
		
	fp = fopen(Path, "r");
	if(fp == NULL) return NULL;

	// 파일 확장자를 구한다.
	cFileHelper FileHelper;
	std::string FileExtent;
	{
		char pFileExtent[32] = {0, };
		if (FileHelper.GetFileExtent(FileName, pFileExtent, 32) < 0)
			return false; // 확장자가 없다.
		FileExtent = pFileExtent;
	}	

	cShader* pShader =0;
	std::string SourceFileName = FileHelper.GetFileName(FileName);

	/* 
	 * park sung jae 2009.3.9 
	 * 파일 확장자의 경우 소스 자체가 바이트 코드를 갖고 있으므로 glsl 문법에만 맞다면 어떤 확장자라도 상관없으나
	 * 버텍스와 프래그먼트 쉐이더를 구분 및 혼합하기위하여 VERT와 FRAG두개의 확장자를 지원한다.
	*/
	if(FileExtent.compare("VERT") == 0){
		pShader = new cVertexShader;	
		std::pair<std::string, cShader*> newItem(SourceFileName, pShader);
		VertexFactory.insert(newItem);
	}else if(FileExtent.compare("FRAG") == 0){
		pShader = new cFragmentShader;	
		std::pair<std::string, cShader*> newItem(SourceFileName, pShader);
		FragmentFactory.insert(newItem);
	}else{
		return NULL;
	}
	 
	pShader->ClearSource();
	while (fgetc(fp) != EOF){
		ShaderLength++;
	}      

	_ASSERT(ShaderLength < MAX_SHADER_LENGTH);

	if(ShaderLength > MAX_SHADER_LENGTH){
		fclose(fp);
		return NULL;
	}
				
	rewind(fp);
				
	if(pShader->ShaderSource != NULL){
		 fread(pShader->ShaderSource, 1, ShaderLength, fp);
	}
				
	pShader->ShaderSource[ShaderLength] = '\0';
	fclose(fp);

	return pShader;
}


cShader* cShaderFactory::GetShaderFromName(std::string Title)
{
	
	return NULL;
}


cShader* cShaderFactory::LoadBasicShader(char* Buffer, LPCTSTR ShaderName, eShaderType Type)
{
	cShader* pShader =0;
	switch(Type)
	{
	case SHADER_TYPE_VERT:
		{
		pShader = new cVertexShader;	
		std::pair<std::string, cShader*> newItem(ShaderName, pShader);
		VertexFactory.insert(newItem);
		}
		break;
	case SHADER_TYPE_FRAG:
		{
		pShader = new cFragmentShader;
		std::pair<std::string, cShader*> newItem(ShaderName, pShader);
		FragmentFactory.insert(newItem);		 			
		}
		break;
	}

	if(pShader == 0) 
		return NULL;

	pShader->ClearSource();
	memcpy(pShader->ShaderSource, Buffer, MAX_SHADER_LENGTH);
	
	return pShader;
}

cShaderAlgorithm* cShaderFactory::GetShaderAlgorithm(eShaderAlgorithm Type)
{
	cShaderAlgorithm* pShader = 0;
	
//	switch(Type){
//	case SHADER_ALGORITHM_CARTOON_SHADER:
//		 pShader = &CartoonShader;
//		 break;
//	}

	_ASSERT(pShader);
	return pShader;
}