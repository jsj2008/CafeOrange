#pragma once
#include "Shader.h"
#include "ShaderAlgorithm.h"
#include <string>
#include <map>
using namespace std;

//쉐이더 알고리즘은 스트링으로 관리한다. 
const LPCTSTR ShaderName[] =
{
	_T("Shadow Map"), 
};


class cShaderFactory
{
public:
	enum eShaderType{
		SHADER_TYPE_VERT,
		SHADER_TYPE_FRAG,
		SHADER_TYPE_UNKNOWN,
	};
		
	static cShaderFactory *pShaderFactory;
	static cShaderFactory* GetInstancePtr(){
		if(pShaderFactory == 0)
			pShaderFactory = new cShaderFactory();

		return pShaderFactory;
	}

	static cShaderFactory& GetInstance(){
		if(pShaderFactory == 0)
			pShaderFactory = new cShaderFactory();

		return *pShaderFactory;
	}

//	cShaderAlgorithm	CartoonShader;
	cShaderAlgorithm	ProjectedShadowMapShader;

	cShader* LoadShaderFromFile(const TCHAR * FileName);
	cShader* GetShaderFromName(std::string Name);

	typedef std::map<std::string,cShader*> ShaderFactory;

	bool LoadBasicShader(const TCHAR* Path, cRenderer& Renderer);

	cShader* LoadBasicShader(char* Buffer, LPCTSTR ShaderName, eShaderType Type);
	cShaderAlgorithm* GetShaderAlgorithm(eShaderAlgorithm Type);

	~cShaderFactory();

private:

	std::map<std::string,cShader*> VertexFactory;
	std::map<std::string,cShader*> FragmentFactory;

	cShaderFactory();

};

