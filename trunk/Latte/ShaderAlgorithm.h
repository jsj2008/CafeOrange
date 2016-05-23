#pragma once
#include "Renderer.h"
#include "Shader.h"

/*
Overview of creating and using OpenGL shaders:
 1.Create one or more (empty) shader objects with glCreateShader
 2.Provide source code for these shaders with glShaderSource
 3.Compile each of the shaders with glCompileShader
 4.Create a program object with glCreateProgram
 5.Attach all the shader objects to the program object with glAttahShader
 6.Link the program object with glLinkProgram
 7.Install the executable program as part of OpenGL's current state with glUseProgram
 8.If the shaders use user-defined uniform variables,query the locations of these variables with 
   glGetUniformLocation and then set their values with glUniform.
*/

class cShader;
class cProgramShader
{
public:
	enum eStatus{
		STATUS_OBJECT_COMPILED,
		STATUS_OBJECT_LINKED,
		STATUS_OBJECT_VALID,
		STATUS_VERTEX_OBJECT_ATTACHED,
		STATUS_FRAGMENT_OBJECT_ATTACHED,
	};

	cShader* pActiveVertexShader;
	cShader* pActiveFragmentShader;

	GLuint ShaderHandle;

	virtual void Render(cRenderer& Renderer);

	void Build(cRenderer& Renderer);
	void UnBuild(cRenderer& Renderer);

	void InsertVertexShader(cShader* pShader);
	void InsertFragmentShader(cShader* pShader);

	int  GetObjectStatus(eStatus Status);
	void GetErrorLog();

	void operator = (const cProgramShader& Other);

	cProgramShader();
   ~cProgramShader();

private:

};
class cShaderAlgorithm
{
public:
	eShaderAlgorithm eType;
	cProgramShader	 ProgramShader;	

	//uinform 변수에 대한 메모리 주소 획득
	GLint GetUniformLocation(const char* name);
	//attribute 변수에 대한 메모리 주소 획득
	GLint GetAttributeLocation(const char* name);

	cShaderAlgorithm();
	virtual ~cShaderAlgorithm();

	void	SetFixedPipeline();
	void	SetShader(bool _ShaderOn) { ShaderOn = _ShaderOn; }
			
	bool	ShaderOn;

};

