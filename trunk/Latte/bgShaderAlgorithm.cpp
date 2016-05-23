#include "stdafx.h"
#include "bgMaterial.h"
#include "bgShaderAlgorithm.h"


GLenum GetGLSLShaderType(eGLSLShaderType type)
{
	switch (type)
	{
	case SHADER_TYPE_VERTEX:				return GL_VERTEX_SHADER; break;
	case SHADER_TYPE_FRAGMENT:		return GL_FRAGMENT_SHADER; break;
	case SHADER_TYPE_TESSELLATION: return GL_GEOMETRY_SHADER; break;
	}


	ASSERT(0);
	return GL_VERTEX_SHADER;
}

bgShaderAlgorithm::bgShaderAlgorithm(eGLSLShaderType type, eShaderFactory spec) :ShaderType(type), ShaderSpec(spec)
{
	ShaderProgramId = -1;
}


bgShaderAlgorithm::~bgShaderAlgorithm()
{

}

bool  bgShaderAlgorithm::Initialize()
{
	AttribProperties.initialize();
	UniformProperties.initialize();
	
	ShaderProgramId = -1;

	return true;
}

bool  bgShaderAlgorithm::MakeShaderLocations()
{


	return true;
}


bool bgShaderAlgorithm::LoadFromString(eGLSLShaderType whichShader, const string& source)
{
	ShaderProgramId = glCreateShader(GetGLSLShaderType(whichShader));

	const char * ptmp = source.c_str();
	glShaderSource(ShaderProgramId, 1, &ptmp, NULL);
	
	GLint status;
	glCompileShader(ShaderProgramId);
	glGetShaderiv(ShaderProgramId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) 
	{
		GLint infoLogLength;
		glGetShaderiv(ShaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(ShaderProgramId, infoLogLength, NULL, infoLog);
		
		//bgSceneObserver 사용으로 대체
		delete[] infoLog;

		return false;
	}

	return true;
}

bool bgShaderAlgorithm::LoadFromFile(eGLSLShaderType whichShader, const string& filename)
{
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp) 
	{
		string line, buffer;
		while (getline(fp, line)) 
		{
			buffer.append(line);
			buffer.append("\r\n");
		}
		return LoadFromString(whichShader, buffer);
	}
	else 
	{
	//cerr << "Error loading shader: " << filename << endl;
	//추후 bgSceneObserver 반영 
		return false;
	}

	return false;
}


void bgShaderAlgorithm::setProperty(const std::string& name, int intValue)
{
	UniformProperties.setProperty(name, intValue);
}

void bgShaderAlgorithm::setProperty(const std::string& name, float floatValue)
{
	UniformProperties.setProperty(name, floatValue);
}


void bgShaderAlgorithm::setTextureProperty(bgMaterial::_TextureList& TextureList)
{
	for (int iTexture = 0; iTexture < TextureList.size(); iTexture++){
		bgTexture* pTexture = TextureList[iTexture];
		std::string texname = "tex";
		if (pTexture->Enabled){
			texname += std::to_string(iTexture);
			UniformProperties.setProperty(texname, iTexture);
		}
	}
}

////////////////////////////////////////////////////////////////////////////
///bgShaderFragmentAlgorithm
bgShaderFragmentAlgorithm::bgShaderFragmentAlgorithm(eGLSLShaderType type, eShaderFactory spec) :bgShaderAlgorithm(type,spec)
{

}

bgShaderFragmentAlgorithm::~bgShaderFragmentAlgorithm()
{

}

bool  bgShaderFragmentAlgorithm::Initialize()
{
	bgShaderAlgorithm::Initialize();
	return true;
}

bool  bgShaderFragmentAlgorithm::MakeShaderLocations()
{
	bgShaderAlgorithm::MakeShaderLocations();
	return true;
}
