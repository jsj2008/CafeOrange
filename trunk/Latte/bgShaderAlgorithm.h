#pragma once
#include "bgShaderProperty.h"
#include <map>
using namespace std;

class bgMaterial;
class bgTexture;


class bgShaderAlgorithm
{
public:
	bgShaderAlgorithm(eGLSLShaderType type,eShaderFactory spec);
	virtual ~bgShaderAlgorithm();

	virtual bool  Initialize();
	virtual bool  MakeShaderLocations();
	
	bool LoadFromString(eGLSLShaderType whichShader, const string& source);
	bool LoadFromFile(eGLSLShaderType whichShader, const string& filename);

	GLuint GetShaderId() { return ShaderProgramId;  }
	eGLSLShaderType	GetShaderType(){ return ShaderType; }
	eShaderFactory		GetShaderSpec(){ return ShaderSpec; }

	bgUniformShaderProperty			UniformProperties;
	bgAttributeShaderProperty			AttribProperties;

	void setProperty(const std::string& name, int intValue);
	void setProperty(const std::string& name, float floatValue);
	void setTextureProperty(std::vector<bgTexture*>& TextureList);

protected:
	GLuint		ShaderProgramId;
	eGLSLShaderType	ShaderType;
	eShaderFactory		ShaderSpec;
};


////////////////////////////////////////////////////////////////////////////
///bgShaderVertexAlgorithm
class bgShaderVertexAlgorithm : public bgShaderAlgorithm
{
public:
	bgShaderVertexAlgorithm(eGLSLShaderType type, eShaderFactory spec);
	virtual ~bgShaderVertexAlgorithm();

	virtual bool  Initialize();
	virtual bool  MakeShaderLocations();

};


////////////////////////////////////////////////////////////////////////////
///bgShaderTessllationAlgorithm
//class bgShaderTessllationAlgorithm : public bgShaderAlgorithm
//{
//
//
//};


////////////////////////////////////////////////////////////////////////////
///bgShaderGeometryAlgorithm
//class bgShaderGeometryAlgorithm : public bgShaderAlgorithm
//{
//
//
//};

////////////////////////////////////////////////////////////////////////////
///bgShaderFragmentAlgorithm
class bgShaderFragmentAlgorithm : public bgShaderAlgorithm
{
public:
	bgShaderFragmentAlgorithm(eGLSLShaderType type, eShaderFactory spec);
	virtual ~bgShaderFragmentAlgorithm();

	virtual bool  Initialize();
	virtual bool  MakeShaderLocations();
	//void setProperty(const std::string& name, int intValue);
	//void setTextureProperty(std::vector<bgTexture*>& TextureList);

};


////////////////////////////////////////////////////////////////////////////
///bgShaderComputeAlgorithm
//class bgShaderComputeAlgorithm : public bgShaderAlgorithm
//{
//
//
//};


