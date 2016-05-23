#include "StdAfx.h"
#include "Model.h"
#include "ShaderFactory.h"
#include "Shader.h"

///////////////////////////////////////////////////////////////////////
cShaderEffect::cShaderEffect()
{
	pShaderAlgorithm = 0;
	eType			 = SHADER_ALGORITHM_COUNT;
	ShaderOn		 = false;
	ShaderObject	 = true;
	ShaderScene		 = false;
	ShaderPreview	 = false;

	LightIndex = 0;
	LightAttenuation = 0.0f;
}

cShaderEffect::~cShaderEffect()
{
}

void cShaderEffect::Build(cRenderer& Renderer)
{
}


void cShaderEffect::Render(cRenderer& Renderer)
{

}

void cShaderEffect::Render(cRenderer& Renderer, cModel& Model)
{

}

void cShaderEffect::SetFixedPipeline(){
	//고정 파이프라인으로 리셋, 리셋하지 않으면 설정된 쉐이더가 계속 적용된다.
	glUseProgram(0);
}

bool cShaderEffect::IsShaderEffect(cModel *pModel)
{
	
	return true;
}

bool cShaderEffect::MakeDefaultTexture(cMaterial& Material)
{	
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectedShadow::cProjectedShadow()
{
	Lightness = 0.5f;
	Accessibility = 0.5f;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectedShadowMapEffect::cProjectedShadowMapEffect()
{
	pShaderAlgorithm = &cShaderFactory::GetInstance().ProjectedShadowMapShader;
}

cProjectedShadowMapEffect::~cProjectedShadowMapEffect()
{

}

void cProjectedShadowMapEffect::Build(cRenderer& Renderer)
{

}

void cProjectedShadowMapEffect::Render(cRenderer& Renderer) 
{	
	cProgramShader& ProgramShader = pShaderAlgorithm->ProgramShader;
	if(!ProgramShader.GetObjectStatus(cProgramShader::STATUS_OBJECT_LINKED)){
		return;
	}

	GLint Accessibility = glGetAttribLocationARB(ProgramShader.ShaderHandle, "Accessibility");
	_ASSERT(Accessibility >= 0);
	glVertexAttrib1fvARB(Accessibility,	&(ProjectedShadow.Accessibility));	

	GLint TextureObjectAttrib  = glGetUniformLocation(ProgramShader.ShaderHandle,"shadow");
	_ASSERT(TextureObjectAttrib >= 0);
	glUniform1i(TextureObjectAttrib, (GLint)Renderer.DepthTexture.Stage);	

	GLint ShadowLightnessAttrib  = glGetUniformLocation(ProgramShader.ShaderHandle,"ShadowLightness");
	_ASSERT(ShadowLightnessAttrib >= 0);
	glUniform1f(ShadowLightnessAttrib, (GLfloat)ProjectedShadow.Lightness);
	 
	ProgramShader.Render(Renderer);	

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cShader::cShader()
{

}

cShader::~cShader()
{

}

void cShader::ClearSource()
{
	ShaderHandle = 0;
	memset(ShaderSource, 0, sizeof(ShaderSource));
}

bool cShader::Build(cRenderer& Renderer)
{
	return false;
}

///////////////////////////////////////////////////////////////////////
cVertexShader::cVertexShader()
{
}

cVertexShader::~cVertexShader()
{
}

bool cVertexShader::Build(cRenderer& Renderer)
{
	if(Renderer.MakeCurrent())
	{
		GLhandleARB Handle = glCreateShader(GL_VERTEX_SHADER);
		if(Handle == 0) return false;

		ShaderHandle = Handle;  
		GLcharARB  *text[1];
		text[0] = (GLcharARB *)ShaderSource;
		glShaderSource(ShaderHandle, 1,(const GLcharARB**)text, NULL);
		
		GLenum error;
		if((error = glGetError()) != GL_NO_ERROR){
			_ASSERT(0);
		}		
		glCompileShader(ShaderHandle);
		if((error = glGetError()) != GL_NO_ERROR){
			_ASSERT(0);
		}		
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
cFragmentShader::cFragmentShader()
{
}

cFragmentShader::~cFragmentShader()
{
}

bool cFragmentShader::Build(cRenderer& Renderer)
{
	if(Renderer.MakeCurrent())
	{
		GLhandleARB Handle = glCreateShader(GL_FRAGMENT_SHADER);
		if(Handle == 0) return false;

		ShaderHandle = Handle;
		GLcharARB *text[1];
		text[0] = (GLcharARB *)ShaderSource;

		glShaderSource(ShaderHandle, 1, (const GLcharARB**)text, NULL);
		GLenum error;
		if((error = glGetError()) != GL_NO_ERROR){
			_ASSERT(0);
		}
		glCompileShader(ShaderHandle);
		if((error = glGetError()) != GL_NO_ERROR){
			_ASSERT(0);
		}
		
	}

	return true;
}



