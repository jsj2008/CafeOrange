#pragma once
#include <map>
#include "bgObject.h"
#include "bgMaterial.h"
#include "bgShaderAlgorithm.h"
using namespace std;

class bgMaterial;
class bgPhotoEffectAlgorithm;


//////////////////////////////////////////////////////////////////////////////////
///bgShader
class bgShader : public bgObject
{
public:
	typedef std::vector<bgShaderAlgorithm*> _ShaderChain;

	bgShader();
	virtual ~bgShader();

	void SetShaderType(eShaderType type){ ShaderType = type; }
	eShaderType	GetShaderType(){ return ShaderType; }

	bool CreateAndLinkProgram();

	void Use();
	void UnUse();
	void BindProperties();

	void InitializeProperties();

	void AddShader(bgShaderAlgorithm* pAlgo);
	bgShaderAlgorithm*	GetShaderAlgorithms(eGLSLShaderType whichShader);

	void SetTextureProperty(bgMaterial* pMaterial, bgPhotoEffectAlgorithm* pPhotoEffect = 0);
	void AddAttribute(const string& attribute);
	void AddUniform(const string& uniform);

	GLuint getAttribute(const string& attribute);
	GLuint getUniform(const string& uniform);
	
	void DeleteShaderProgram();
	bool	 ChangeTextureOperation(int stageIndex, eTextureOperation operation);

private:
	eShaderType	ShaderType;
	GLuint	 ShaderProgramId;
	map<string, GLuint> attributeList;
	map<string, GLuint> uniformLocationList;

	bgMaterial*		pMaterial;
	_ShaderChain		ShaderChain;
};

