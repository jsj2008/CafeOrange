#pragma once
#include "GL/glew.h"
#include "Renderer.h"

#define MAX_SHADER_LENGTH   400000
/*
Shadow Algorithms
 //http://www.slideshare.net/MoonLightMS/1023-1
Projected Shadow Mapping
Stencil Shadow Volume
Depth Buffer Shadow
Perspective Shadow Map(PSM)
Light Space Perspective Shadow Map(LSPSM)
Cascaded LSPSM
Soft Shadow
Variance Shadow Map(VSM)
*/

enum eShaderAlgorithm{  // ���� ���� 
	SHADER_ALGORITHM_NONE,
	SHADER_ALGORITHM_SHADOW_BEGIN,
		SHADER_FIXED_PIPE_SHADOW_MAP,
		SHADER_PROJECTED_SHADOW,
		SHADER_STENCIL_SHADOW,
		SHADER_DEPTH_BUFFER_SHADOW,
		SHADER_PSM,
		SHADER_LSPSM,
		SHADER_CASCADED_LSPSM,
		SHADER_SOFT_SHADOW,
		SHADER_VSM,
	SHADER_ALGORITHM_SHADOW_END,
		
	SHADER_ALGORITHM_COUNT,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//class cCartoon{
//public:
//	glm::vec4	Color;
//	float		ThreshHold;
//
//
//	cCartoon();
//};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

class cShaderAlgorithm;
class cModel;
class cMaterial;
class cShaderEffect{
public:
	bool	ShaderOn;
	bool	ShaderObject;
	bool	ShaderScene;
	bool	ShaderPreview;
	
	//��鳻�� �ִ� ������ �ش� ���̴��� ���� ����
	glm::vec3 LightPosition;
	int		    LightIndex;
	float	    LightAttenuation;

	eShaderAlgorithm  eType;
	cShaderAlgorithm* pShaderAlgorithm;
	
	virtual void Build(cRenderer& Renderer);
	virtual void Render(cRenderer& Renderer);
	virtual void Render(cRenderer& Renderer, cModel& Model);

	void	SetFixedPipeline();
	bool	MakeDefaultTexture(cMaterial& Material);
	bool	IsShaderEffect(cModel *pModel);


	cShaderEffect();
	virtual ~cShaderEffect();
	//virtual cShaderEffect* Clone() const = 0;
};


class cProjectedShadow{

public:
	float Lightness;
	float Accessibility;
	cProjectedShadow();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cProjectedShadowMapEffect
class cProjectedShadowMapEffect : public cShaderEffect
{
public:
	cProjectedShadow ProjectedShadow;

	cProjectedShadowMapEffect();
	virtual ~cProjectedShadowMapEffect();

	virtual void Build(cRenderer& Renderer);
	virtual void Render(cRenderer& Renderer);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class cShader
{
public:
	GLuint ShaderHandle;
	GLchar ShaderSource[MAX_SHADER_LENGTH];

	virtual bool Build(cRenderer& Renderer);
	void ClearSource();

	cShader();
	virtual ~cShader();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/****** ���ؽ� ���̴� ���� *********
 *  1.���ؽ� ��ġ ��ȯ
 *  2.���ؽ� ���� ������
 *  3.�ؽ��� ��ǥ ������ ��ȯ 
 ***********************************
*/
class cVertexShader : public cShader
{
public:
	virtual bool Build(cRenderer& Renderer);

	cVertexShader();
   ~cVertexShader();

private:
};

/****** �����׸�Ʈ ���̴� ���� *********
 *  1.�ȼ����� ���� �� �ؽ��� ��ǥ ���
 *  2.�ؽ��� ����
 *  3.�Ȱ� ���
 *	4.������ ���� �ȼ� ���� �븻�� ��� 
 ***********************************
*/
class cFragmentShader : public cShader
{
public:
	virtual bool Build(cRenderer& Renderer);

	cFragmentShader();
   ~cFragmentShader();

private:
};

