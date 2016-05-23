#pragma once
#include "bgTexture.h"
#include "bgPhotoEffectAlgorithm.h"

class bgCanvasActor;

class bgSpecialEffectAlgorithm
{
public:

	void	 BindScreenFBO(glm::vec2 ScreenSize);
	void	 UnBindScreenFBO();

	virtual bool Initialize(glm::vec2& FBOSize, cRenderer& Renderer,bgShaderFactory& ShaderFactory);
	virtual void Render(std::vector<bgActor*>& ActorList, cRenderer& Renderer, cCamera& Camera) = 0;
	virtual bool Execute(cRenderer& Renderer, bgActor* pTargetActor) = 0;
	bgSpecialEffectAlgorithm();
	virtual ~bgSpecialEffectAlgorithm();

	eSpecialEffect GetEffectType() { return EffectType;  }

protected:
	eSpecialEffect		EffectType;
	glm::vec2			RenderSize;
	bgFBOTexture	FBO;

	bgCanvasActor*				 pCanvasActor;
	bgPhotoEffectAlgorithm* pEffectAlgo;

	bool CreateScreenFBO(glm::vec2 ScreenSize);
	virtual void RenderEffect(cRenderer& Renderer, cCamera& Camera) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
////bg2Dto3DSpecialEffectAlgorithm
class bg2Dto3DSpecialEffectAlgorithm : public bgSpecialEffectAlgorithm
{
public:
	virtual bool Initialize(glm::vec2& FBOSize,cRenderer& Renderer,bgShaderFactory& ShaderFactory);
	virtual void Render(std::vector<bgActor*>& ActorList, cRenderer& Renderer, cCamera& Camera);
	virtual bool Execute(cRenderer& Renderer, bgActor* pTargetActor);

	bg2Dto3DSpecialEffectAlgorithm();
	virtual ~bg2Dto3DSpecialEffectAlgorithm();

protected:

	std::vector<float> HeightMap;
	virtual void RenderEffect(cRenderer& Renderer, cCamera& Camera);

};













