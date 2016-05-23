#include "stdafx.h"
#include "bgPostProcess.h"
#include "bgActor.h"
#include "bgShader.h"
#include "bgShaderFactory.h"
#include "Camera.h"
#include "bgTexture.h"

bgPostProcess::bgPostProcess()
{
	pScreenActor = 0;
}

bgPostProcess::~bgPostProcess()
{
	delete pScreenActor;
}

bool bgPostProcess::IsEnable()
{
	if (PhotoEffectList.size() <= 0)
		return false;

	return IsEnabled;
}

bool bgPostProcess::CreateScreenFBO(glm::vec2 ScreenSize)
{
	if (ScreenFBO.FrameBufferID != -1 || ScreenFBO.RenderBufferID != -1)
		ScreenFBO.ReleaseFrameBuffer();

	RenderSize = ScreenSize;
	return ScreenFBO.CreateFrameBuffer(bgFBOTexture::FBO_TYPE_COLOR, (int)RenderSize.x, (int)RenderSize.y);
}

void	 bgPostProcess::BindScreenFBO(glm::vec2 ScreenSize)
{
	if (ScreenSize.x != RenderSize.x || ScreenSize.y != RenderSize.y){
		CreateScreenFBO(ScreenSize);
	}

	ScreenFBO.BindFrameBuffer();
	ASSERT_GL_CALL();
}

void bgPostProcess::UnBindScreenFBO()
{	
	ScreenFBO.UnBindFrameBuffer();
	ASSERT_GL_CALL();
}


void bgPostProcess::Render(cRenderer& Renderer, cCamera& Camera)
{	
	pScreenActor->build(Renderer);
	ScreenFBO.BindFrameBuffer();	
	
	bgMaterial* pMaterial = pScreenActor->getMaterial();
	pMaterial->TextureList[0]->TextureID = ScreenFBO.TextureID;

	for (int index = 0; index < PhotoEffectList.size(); index++)
	{
		bgPhotoEffectAlgorithm* pAlgo = PhotoEffectList[index];		
		pAlgo->RenderActor(Renderer, Camera, *pScreenActor);
	}
	
	ScreenFBO.UnBindFrameBuffer();
	RenderScreen( Renderer, Camera);
}

void bgPostProcess::RenderScreen(cRenderer& Renderer, cCamera& Camera)
{
	bgMaterial*pMaterial = pScreenActor->getMaterial();
	pMaterial->TextureList[0]->TextureID =  ScreenFBO.GetTextureID();

	pScreenActor->build(Renderer);

	bgShader* pShader = pScreenActor->getShader();
	glm::mat4  cameraProjectMat4 = Camera.GetProjectionMatrix() * Camera.GetWorldViewMatrix();

	if (pShader) Renderer.BindShader(pShader, pMaterial, cameraProjectMat4, pScreenActor->GetTransformMat4());
	if (pMaterial) Renderer.BindMaterial(pMaterial);
		Renderer.RenderActor(pScreenActor);
	if (pShader) Renderer.UnbindShader(pShader);
	if (pMaterial) Renderer.UnBindMaterial(pMaterial);

}

bool bgPostProcess::Initialize(bgShaderFactory& ShaderFactory)
{
	bgMapper* pMapper = new bgMapper();
		bgData* pData = new bgData();
		pData->generate(SOURCE_2D_RECTANGLE_PTC);
		pMapper->setInput(pData);

	bgShader* pShader = new bgShader();
		bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
		bgShaderAlgorithm* 	pFragmentShaderAlgorithm = ShaderFactory.requestFragmentShader(VERTEX_XYZ_COLOR_TEX0);
		pShader->AddShader(pVertexShaderAlgorithm);
		pShader->AddShader(pFragmentShaderAlgorithm);

	bgMaterial* pMaterial = new bgMaterial();
		bgTexture* pTextrue = new bgTexture();
		pMaterial->TextureList.push_back(pTextrue);

	pScreenActor = new bgActor();
	pScreenActor->setInput(pMapper);
	pScreenActor->setShader(pShader);
	pScreenActor->setMaterial(pMaterial);
	pScreenActor->initialize();

	IsEnabled = false;
	ASSERT_GL_CALL();

	return true;
}

bool		bgPostProcess::AddPostEffect(bgShaderFactory& ShaderFactory, ePhotoEffect effect, bgActor* pTargetActor)
{
	bgPhotoEffectAlgorithm* pAlgo = NULL;
	switch (effect)
	{
	case PHOTO_EFFECT_ALPHA:							 pAlgo = new bgAlphaPhotoEffectAlgorithm();  break;
	case PHOTO_EFFECT_BRIGHT_CONTRAST:   pAlgo = new bgBrightContrastPhotoEffectAlgorithm(); break;
	case PHOTO_EFFECT_GRAY:							 pAlgo = new bgGrayPhotoEffectAlgorithm(); break;
	default: return false; break;
	}

	pAlgo->CreateShader(ShaderFactory);	
	PhotoEffectList.insert(std::make_pair(PhotoEffectList.size(), pAlgo));
	
	IsEnabled = true;
	return true;
}
bool		bgPostProcess::DeletePostEffect(int  algorithmIndex)
{
	_PhotoEffectList::iterator iEffect = PhotoEffectList.find(algorithmIndex);
	if (iEffect == PhotoEffectList.end())
		return false;

	PhotoEffectList.erase(iEffect);
	PhotoEffectList.size() > 0 ? IsEnabled = true : IsEnabled = false;

	return true;
}
bgPhotoEffectAlgorithm* bgPostProcess::GetPostEffect(int algorithmIndex)
{
	_PhotoEffectList::iterator iEffect = PhotoEffectList.find(algorithmIndex);
	if (iEffect == PhotoEffectList.end())
		return NULL;

	return iEffect->second;
}