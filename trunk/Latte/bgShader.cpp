#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Renderer.h"
#include "bgMaterial.h"
#include "bgShader.h"
#include "bgPhotoEffectAlgorithm.h"


bgShader::bgShader(void)
{
	pMaterial = 0;

	attributeList.clear();
	uniformLocationList.clear();
}

bgShader::~bgShader(void)
{
	attributeList.clear();
	uniformLocationList.clear();
}

void bgShader::DeleteShaderProgram() {
	glDeleteProgram(ShaderProgramId);
}


bool  bgShader::CreateAndLinkProgram() 
{
	ShaderProgramId = glCreateProgram();
	if (ShaderProgramId < 0)
		return false;

	_ShaderChain::iterator iShader = ShaderChain.begin();
	for (; iShader != ShaderChain.end(); iShader++)
	{
		if (bgShaderAlgorithm* pAlgo = *iShader){
			if (pAlgo->GetShaderId() >= 0)
			{
				glAttachShader(ShaderProgramId, pAlgo->GetShaderId());
			}
		}
	}

	GLint status;
	glLinkProgram(ShaderProgramId);
	glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) 
	{
		GLint infoLogLength;
		glGetProgramiv(ShaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(ShaderProgramId, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		//bgSceneObserver·Î ´ëÃ¼ 
		delete[] infoLog;
		return false;
	}

	iShader = ShaderChain.begin();
	for (; iShader != ShaderChain.end(); iShader++)
	{
		if (bgShaderAlgorithm* pAlgo = *iShader){
			if (pAlgo->GetShaderId() >= 0)
				glDeleteShader(pAlgo->GetShaderId());
		}
	}

	return true;
}

void bgShader::Use() 
{
	glUseProgram(ShaderProgramId);
}

void bgShader::UnUse() 
{
	glUseProgram(0);
}

void bgShader::InitializeProperties()
{
	if (ShaderChain.empty()) return;

	AddUniform(world_matrix);
	AddUniform(model_matrix);

	if (bgShaderAlgorithm* pAlgo = GetShaderAlgorithms(SHADER_TYPE_VERTEX))
	{
		if (pAlgo->GetShaderSpec() > SHADER_VERTEX_BEGIN && pAlgo->GetShaderSpec() < SHADER_VERTEX_BEGIN)
		{

		}
	}

	if (bgShaderAlgorithm* pAlgo = GetShaderAlgorithms(SHADER_TYPE_FRAGMENT))
	{
		if (pAlgo->GetShaderSpec() > SHADER_FRAGMENT_BEGIN && pAlgo->GetShaderSpec() <= SHADER_FRAGMENT_SELECTED)
		{
			AddUniform(texsize);
			AddUniform(tex0);
			AddUniform(tex1);
			AddUniform(tex2);
			AddUniform(tex3);

			AddUniform(tex1blend);
			AddUniform(tex2blend);
			AddUniform(tex3blend);
		}

		if (pAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_ALPHA)
		{
			AddUniform(alpha);
		}

		if (pAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_CONTRAST_BRIGHT)
		{
			AddUniform(brightness);
			AddUniform(contrast);
		}

		if (pAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_GRAY)
		{
			AddUniform(blendway);
		}
	}

}

void bgShader::BindProperties() 
{
	if (ShaderProgramId < 0) return;

	_ShaderChain::iterator iShader = ShaderChain.begin();
	for (; iShader != ShaderChain.end(); iShader++)
	{
		if (bgShaderAlgorithm* pAlgo = *iShader)
		{
			if (pAlgo->GetShaderId() >= 0)
			{
				pAlgo->AttribProperties.bindProperties(attributeList);
				pAlgo->UniformProperties.bindProperties(uniformLocationList);					
			}
		}
	}

}

bool		bgShader::ChangeTextureOperation(int stageIndex, eTextureOperation operation)
{
	if (bgShaderAlgorithm* pAlgo = GetShaderAlgorithms(SHADER_TYPE_FRAGMENT))
	{
		if (pAlgo->GetShaderSpec() > SHADER_FRAGMENT_BEGIN && pAlgo->GetShaderSpec() <= SHADER_FRAGMENT_SELECTED)
		{
			std::string texname = "tex";
			texname += std::to_string(stageIndex);
			texname += "blend";

			pAlgo->setProperty(texname, (int)operation);
			return true;
		}
	}

	return false;
}

void bgShader::AddShader(bgShaderAlgorithm* pAlgo)
{
	_ShaderChain::iterator iShader = std::find(ShaderChain.begin(), ShaderChain.end(), pAlgo);
	if (iShader == ShaderChain.end())
		ShaderChain.push_back(pAlgo);
}

//bool bgShader::setMaterial(bgMaterial* _pMaterial)
//{
//	pMaterial = _pMaterial;
//	return true;
//}

void bgShader::SetTextureProperty(bgMaterial* pMaterial, bgPhotoEffectAlgorithm* pPhotoEffect)
{
	if (pMaterial == 0 || ShaderProgramId < 0)
		return;

	if (pMaterial->TextureList.size() < 0)
		return;

	_ShaderChain::iterator iShader = ShaderChain.begin();
	for (; iShader != ShaderChain.end(); iShader++)
	{	
		if (bgShaderAlgorithm* pFragAlgo = dynamic_cast<bgShaderAlgorithm*>(*iShader))
		{
			if (pFragAlgo->GetShaderSpec() > SHADER_FRAGMENT_BEGIN && pFragAlgo->GetShaderSpec() < SHADER_FRAGMENT_END)
			{
				pFragAlgo->setProperty(texsize, (int)pMaterial->TextureList.size());
				pFragAlgo->setTextureProperty(pMaterial->TextureList);
			}

			if (pFragAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_ALPHA)
			{
				bgAlphaPhotoEffectAlgorithm* pAlgo = dynamic_cast<bgAlphaPhotoEffectAlgorithm*>(pPhotoEffect);
				pFragAlgo->setProperty(alpha, pAlgo->GetAlpha());
			}
			else if (pFragAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_CONTRAST_BRIGHT)
			{
				bgBrightContrastPhotoEffectAlgorithm* pAlgo = dynamic_cast<bgBrightContrastPhotoEffectAlgorithm*>(pPhotoEffect);
				pFragAlgo->setProperty(brightness, pAlgo->GetBrightness());
				pFragAlgo->setProperty(contrast, pAlgo->GetContrast());
			}
			else if (pFragAlgo->GetShaderSpec() == SHADER_FRAGMENT_PP_GRAY)
			{
				bgGrayPhotoEffectAlgorithm* pAlgo = dynamic_cast<bgGrayPhotoEffectAlgorithm*>(pPhotoEffect);
				pFragAlgo->setProperty(blendway, pAlgo->GetBlendWay());
			}
		}		
	}

}

void bgShader::AddAttribute(const string& attribute) {
	attributeList[attribute] = glGetAttribLocation(ShaderProgramId, attribute.c_str());
}

GLuint bgShader::getAttribute(const string& attribute) {
	return attributeList[attribute];	
}

void bgShader::AddUniform(const string& uniform) {
	uniformLocationList[uniform] = glGetUniformLocation(ShaderProgramId, uniform.c_str());
}

GLuint bgShader::getUniform(const string& uniform){
	return uniformLocationList[uniform];
}

bgShaderAlgorithm* bgShader::GetShaderAlgorithms(eGLSLShaderType whichShader)
{
	_ShaderChain::iterator iShader = ShaderChain.begin();
	for (; iShader != ShaderChain.end(); iShader++)
	{
		if (bgShaderAlgorithm* pAlgo = *iShader){
			if (pAlgo->GetShaderType() == whichShader)
				return pAlgo;
		}
	}

	return 0;
}
