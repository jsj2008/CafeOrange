#include  "stdafx.h"
#include "Camera.h"
#include "bgShaderFactory.h"
#include "bgActor.h"
#include "bgPhotoEffectAlgorithm.h"


bgPhotoEffectAlgorithm::bgPhotoEffectAlgorithm()
{
	pShader = 0;
}

bgPhotoEffectAlgorithm::~bgPhotoEffectAlgorithm()
{

}

GLuint		bgPhotoEffectAlgorithm::RenderActor(cRenderer& Renderer, cCamera& Camera, bgActor& Actor)
{
	if (pShader == NULL) return 0;

	glm::mat4  cameraProjectMat4 = Camera.GetProjectionMatrix() * Camera.GetWorldViewMatrix();		
	bgMaterial* pMaterial = Actor.getMaterial();

	if (pShader) Renderer.BindShader(pShader, pMaterial, cameraProjectMat4, Actor.GetTransformMat4(), this);
	if (pMaterial) Renderer.BindMaterial(pMaterial);
		Renderer.RenderActor(&Actor);
	if (pShader) Renderer.UnbindShader(pShader);
	if (pMaterial) Renderer.UnBindMaterial(pMaterial);

}

/////////////////////////////////////////////////////////////////////////////////////////////
////bgAlphaPhotoEffectAlgorithm
bgAlphaPhotoEffectAlgorithm::bgAlphaPhotoEffectAlgorithm()
{
	effectType = PHOTO_EFFECT_ALPHA;
	Alpha = 0.5f;
}

bgAlphaPhotoEffectAlgorithm::~bgAlphaPhotoEffectAlgorithm()
{

}

bool bgAlphaPhotoEffectAlgorithm::CreateShader(bgShaderFactory& ShaderFactory)
{
	pShader = new bgShader();
	bgShaderAlgorithm* pVertexShaderAlgorithm		  = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory.getFragmentShader(SHADER_FRAGMENT_PP_ALPHA);

	ASSERT(pVertexShaderAlgorithm != 0);
	ASSERT(pFragmentShaderAlgorithm != 0);

	pShader->AddShader(pVertexShaderAlgorithm);
	pShader->AddShader(pFragmentShaderAlgorithm);
	
	pShader->CreateAndLinkProgram();
	pShader->Use();	
		pShader->InitializeProperties();
	pShader->UnUse();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
////bgBrightContrastPhotoEffectAlgorithm
bgBrightContrastPhotoEffectAlgorithm::bgBrightContrastPhotoEffectAlgorithm()
{
	Brightness = 0.5f;
	Contrast = 0.5f;
	effectType = PHOTO_EFFECT_BRIGHT_CONTRAST;
}

bgBrightContrastPhotoEffectAlgorithm::~bgBrightContrastPhotoEffectAlgorithm()
{

}

bool bgBrightContrastPhotoEffectAlgorithm::CreateShader(bgShaderFactory& ShaderFactory)
{
	pShader = new bgShader();
	bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory.getFragmentShader(SHADER_FRAGMENT_PP_CONTRAST_BRIGHT);

	ASSERT(pVertexShaderAlgorithm != 0);
	ASSERT(pFragmentShaderAlgorithm != 0);

	pShader->AddShader(pVertexShaderAlgorithm);
	pShader->AddShader(pFragmentShaderAlgorithm);

	pShader->CreateAndLinkProgram();
	pShader->Use();
	pShader->InitializeProperties();
	pShader->UnUse();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
////bgGrayPhotoEffectAlgorithm
bgGrayPhotoEffectAlgorithm::bgGrayPhotoEffectAlgorithm()
{
	BlendWay = 0;
	effectType = PHOTO_EFFECT_GRAY;
}

bgGrayPhotoEffectAlgorithm::~bgGrayPhotoEffectAlgorithm()
{

}

bool bgGrayPhotoEffectAlgorithm::CreateShader(bgShaderFactory& ShaderFactory)
{
	pShader = new bgShader();
	bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory.getFragmentShader(SHADER_FRAGMENT_PP_GRAY);

	ASSERT(pVertexShaderAlgorithm != 0);
	ASSERT(pFragmentShaderAlgorithm != 0);

	pShader->AddShader(pVertexShaderAlgorithm);
	pShader->AddShader(pFragmentShaderAlgorithm);

	pShader->CreateAndLinkProgram();
	pShader->Use();
	pShader->InitializeProperties();
	pShader->UnUse();


	return true;
}

