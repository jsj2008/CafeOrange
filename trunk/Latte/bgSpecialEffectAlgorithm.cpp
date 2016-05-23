#include "stdafx.h"
#include "bgSpecialEffectAlgorithm.h"
#include "bgActor.h"
#include "bgMaterial.h"
#include "bgShaderFactory.h"
#include "Camera.h"

bgSpecialEffectAlgorithm::bgSpecialEffectAlgorithm()
{
	pCanvasActor = 0;
	pEffectAlgo = 0;
}

bgSpecialEffectAlgorithm::~bgSpecialEffectAlgorithm()
{
	delete pEffectAlgo;
}

bool bgSpecialEffectAlgorithm::CreateScreenFBO(glm::vec2 ScreenSize)
{
	if (FBO.FrameBufferID != -1 || FBO.RenderBufferID != -1)
		FBO.ReleaseFrameBuffer();

	RenderSize = ScreenSize;
	return FBO.CreateFrameBuffer(bgFBOTexture::FBO_TYPE_COLOR, (int)RenderSize.x, (int)RenderSize.y);
}

void	 bgSpecialEffectAlgorithm::BindScreenFBO(glm::vec2 ScreenSize)
{
	if (ScreenSize.x != RenderSize.x || ScreenSize.y != RenderSize.y){
		CreateScreenFBO(ScreenSize);
	}

	FBO.BindFrameBuffer();
	ASSERT_GL_CALL();
}

void	 bgSpecialEffectAlgorithm::UnBindScreenFBO()
{
	FBO.UnBindFrameBuffer();
	ASSERT_GL_CALL();
}

bool bgSpecialEffectAlgorithm::Initialize(glm::vec2& FBOSize, cRenderer& Renderer, bgShaderFactory& ShaderFactory)
{
	RenderSize = FBOSize;
	if (!FBO.CreateFrameBuffer(bgFBOTexture::FBO_TYPE_COLOR, FBOSize.x, FBOSize.y))
		return false;

	pCanvasActor = new bgCanvasActor();
	bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory.requestFragmentShader(VERTEX_XYZ_COLOR_TEX0);

	bgShader* pShader = new bgShader();
	pShader->AddShader(pVertexShaderAlgorithm);
	pShader->AddShader(pFragmentShaderAlgorithm);
	bgMapper* pMapper = new bgMapper();
	bgData* pData = new bgData();
	pData->generate(CANVAS_TYPE_FULL);
	pMapper->setInput(pData);

	bgMaterial*pMaterial = new bgMaterial();
	pMaterial->TextureList.push_back(new bgTexture());

	pCanvasActor->setShader(pShader);
	pCanvasActor->setMaterial(pMaterial);
	pCanvasActor->setTranslation(glm::vec3(.0f, 0.f, 0.f));
	pCanvasActor->setInput(pMapper);
	pCanvasActor->initialize();
	pCanvasActor->name = std::string("BackgroundMesh");
	pCanvasActor->build(Renderer);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////bg2Dto3DSpecialEffectAlgorithm
bg2Dto3DSpecialEffectAlgorithm::bg2Dto3DSpecialEffectAlgorithm()
{

}

bg2Dto3DSpecialEffectAlgorithm::~bg2Dto3DSpecialEffectAlgorithm()
{

}

bool bg2Dto3DSpecialEffectAlgorithm::Initialize(glm::vec2& FBOSize, cRenderer& Renderer, bgShaderFactory& ShaderFactory)
{
	if (!bgSpecialEffectAlgorithm::Initialize(FBOSize, Renderer, ShaderFactory))
		return false;

	pEffectAlgo = new bgGrayPhotoEffectAlgorithm();
	pEffectAlgo->CreateShader(ShaderFactory);
}

void bg2Dto3DSpecialEffectAlgorithm::Render(std::vector<bgActor*>& ActorList, cRenderer& Renderer, cCamera& Camera)
{
	BindScreenFBO(RenderSize);
		Camera.SetView();
		glm::mat4  cameraProjectMat4 = Camera.GetProjectionMatrix() * Camera.GetWorldViewMatrix();

		std::vector<bgActor*>::iterator iActor = ActorList.begin();
		for (; iActor != ActorList.end(); iActor++)
		{
			bgActor* pActor = *iActor;
			bgShader* pShader = pActor->getShader();
			bgMaterial* pMaterial = pActor->getMaterial();

			if (pShader) Renderer.BindShader(pShader, pMaterial, cameraProjectMat4, pActor->GetTransformMat4());
			if (pMaterial) Renderer.BindMaterial(pMaterial);
				Renderer.RenderActor(pActor);
			if (pShader) Renderer.UnbindShader(pShader);
			if (pMaterial) Renderer.UnBindMaterial(pMaterial);
		}
	UnBindScreenFBO();

	RenderEffect(Renderer, Camera);

}

void bg2Dto3DSpecialEffectAlgorithm::RenderEffect(cRenderer& Renderer, cCamera& Camera)
{
	bgMaterial* pMaterial = pCanvasActor->getMaterial();
	pMaterial->TextureList[0]->TextureID = FBO.TextureID;

	FBO.BindFrameBuffer();
		pEffectAlgo->RenderActor(Renderer, Camera, *pCanvasActor);

	HeightMap.clear();

	int StepX = RenderSize.x / 21;
	int StepY = RenderSize.y / 21;

	for (int col = 0; col < StepY; col++)
	{
		for (int row = 0; row < StepX; row ++)
		{
			int xpos = StepX*row;
			int ypos = StepY*col;

			unsigned char pickColor[4] = { 0, };
			glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickColor);
			HeightMap.push_back(pickColor[0] / 255.0f);
		}

	}

	FBO.UnBindFrameBuffer();

}

bool bg2Dto3DSpecialEffectAlgorithm::Execute(cRenderer& Renderer, bgActor* pTargetActor)
{

	//CString message;
	bgData* pData  = pTargetActor->getInput()->getInput();
	vector<float>& PositionList = pData->GetPositionList();
	int heightIndex = 0;
	for (int index = 2; index < PositionList.size(); index += 3, heightIndex++)
	{
		if (index < PositionList.size())
		{
			PositionList[index] = HeightMap[heightIndex] ;
			//message.Format("index%d:%f \n\r", index, PositionList[index]);
			//OutputDebugString(message);
		}
	}	
	pTargetActor->build(Renderer);

	return true;
}

