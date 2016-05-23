#include "StdAfx.h"
#include "Shader.h"
#include "ShaderAlgorithm.h"



////////////////////////////////////////////////////////////////////////////
cProgramShader::cProgramShader()
{
	ShaderHandle = 0;
	pActiveVertexShader = 0;
	pActiveFragmentShader = 0;		
}

cProgramShader::~cProgramShader()
{
	if(ShaderHandle != 0) glDeleteProgram(ShaderHandle);
}

void cProgramShader::InsertVertexShader(cShader* pShader)
{
	if(ShaderHandle == 0 || pShader == 0) return;

	if(pActiveVertexShader && GetObjectStatus(STATUS_VERTEX_OBJECT_ATTACHED)){
		glDetachShader(ShaderHandle, pActiveVertexShader->ShaderHandle);
	}

	pActiveVertexShader = pShader;
}

void cProgramShader::InsertFragmentShader(cShader* pShader)
{
	if(ShaderHandle == 0 || pShader == 0)  return;

	if(pActiveFragmentShader && GetObjectStatus(STATUS_VERTEX_OBJECT_ATTACHED)){
		glDetachShader(ShaderHandle, pActiveFragmentShader->ShaderHandle);
	}

	pActiveFragmentShader = pShader;
}

void cProgramShader::Build(cRenderer& Renderer)
{
	if(ShaderHandle != 0) UnBuild(Renderer);

	if(Renderer.MakeCurrent())
	{
		ShaderHandle = glCreateProgram();
		GLenum error;
		if((error = glGetError()) != GL_NO_ERROR){
			_ASSERT(ShaderHandle);
		}

		if(pActiveVertexShader)		glAttachShader(ShaderHandle, pActiveVertexShader->ShaderHandle);
		if(pActiveFragmentShader)	glAttachShader(ShaderHandle, pActiveFragmentShader->ShaderHandle);

		glLinkProgram(ShaderHandle);		
		
#ifdef _DEBUG
		GetErrorLog();
#endif
	}
}

void cProgramShader::UnBuild(cRenderer& Renderer)
{
	if(Renderer.MakeCurrent())
	{
		if(pActiveVertexShader)		glDetachShader(ShaderHandle, pActiveVertexShader->ShaderHandle);
		if(pActiveFragmentShader)	glDetachShader(ShaderHandle, pActiveFragmentShader->ShaderHandle);	

		ShaderHandle = 0;
		
#ifdef _DEBUG
		GetErrorLog();
#endif
	
	}
}

void cProgramShader::Render(cRenderer& Renderer)
{	
	glUseProgram(ShaderHandle);	
#ifdef _DEBUG
//	GetErrorLog();
#endif
}


int  cProgramShader::GetObjectStatus(eStatus Status)
{
	GLint Param = 0;

	switch(Status){
	case STATUS_OBJECT_COMPILED: glGetObjectParameterivARB(ShaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &Param); 	break;
	case STATUS_OBJECT_LINKED:   glGetObjectParameterivARB(ShaderHandle, GL_OBJECT_LINK_STATUS_ARB, &Param); 		break;	
	case STATUS_OBJECT_VALID:	 glGetObjectParameterivARB(ShaderHandle, GL_OBJECT_VALIDATE_STATUS_ARB, &Param); 	break;	
	case STATUS_VERTEX_OBJECT_ATTACHED:		glGetObjectParameterivARB(pActiveVertexShader->ShaderHandle  , GL_OBJECT_ATTACHED_OBJECTS_ARB, &Param); 		break;
	case STATUS_FRAGMENT_OBJECT_ATTACHED:	glGetObjectParameterivARB(pActiveFragmentShader->ShaderHandle, GL_OBJECT_ATTACHED_OBJECTS_ARB, &Param); 		break;
	}

	if(Param == 0) GetErrorLog();

	return Param;
}

void cProgramShader::operator = (const cProgramShader& Other)
{
	pActiveVertexShader   = Other.pActiveVertexShader;
	pActiveFragmentShader = Other.pActiveFragmentShader;
}

void cProgramShader::GetErrorLog()
{
	if(ShaderHandle == 0 )  
		return;

	GLint   infologLength = 0;
    GLint   charsWritten  = 0;
	GLchar  infoLog[MAX_PATH] = {0,};

	glGetInfoLogARB(ShaderHandle,MAX_PATH,&charsWritten,infoLog);
	if(charsWritten != 0)
	{
		ATLTRACE(_T("-------------OpenGL Shader Error 발생------------\r\n")); 
			ATLTRACE(infoLog); ATLTRACE(_T("\r\n"));
		ATLTRACE(_T("-------------------End of Log-------------------\r\n"));
	}
}

////////////////////////////////////////////////////////////////////////////

cShaderAlgorithm::cShaderAlgorithm()
{
	ShaderOn   = false;
}

cShaderAlgorithm::~cShaderAlgorithm()
{

}

GLint cShaderAlgorithm::GetUniformLocation(const char* name)
{
	// 쉐이더에 설정된 변수는 쉐이더가 링크된 상태이어야 접근할 수 있다.
	if(ProgramShader.GetObjectStatus(cProgramShader::STATUS_OBJECT_LINKED)){
		return -1;
	}

	return glGetUniformLocationARB(ProgramShader.ShaderHandle, name);
}

GLint cShaderAlgorithm::GetAttributeLocation(const char* name)
{
	// 쉐이더에 설정된 변수는 쉐이더가 링크된 상태이어야 접근할 수 있다.
	if(ProgramShader.GetObjectStatus(cProgramShader::STATUS_OBJECT_LINKED)){
		return -1;
	}

	return glGetAttribLocationARB(ProgramShader.ShaderHandle, name);
}

void cShaderAlgorithm::SetFixedPipeline(){
	//고정 파이프라인으로 리셋, 리셋하지 않으면 설정된 쉐이더가 계속 적용된다.
	if(ProgramShader.ShaderHandle != 0){
		glUseProgram(0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

