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

enum eShaderAlgorithm{  // 순서 유지 
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
	
	//장면내에 있는 조명중 해당 쉐이더에 사용될 조명
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
/****** 버텍스 쉐이더 역할 *********
 *  1.버텍스 위치 변환
 *  2.버텍스 단위 조명계산
 *  3.텍스쳐 좌표 생성과 변환 
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

/****** 프레그먼트 쉐이더 역할 *********
 *  1.픽셀단위 색상 및 텍스쳐 좌표 계산
 *  2.텍스쳐 응용
 *  3.안개 계산
 *	4.조명을 위한 픽셀 단위 노말값 계산 
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

