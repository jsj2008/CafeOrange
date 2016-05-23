#pragma once
#include "bgPhotoEffectAlgorithm.h"

class bgActor;
class bgFBOTexture;
class cRenderer;
class cCamera;
class bgShaderFactory;

class bgPostProcess
{
public:
	typedef std::map<int, bgPhotoEffectAlgorithm*> _PhotoEffectList;
	
	bool IsEnable();
	void	SetIsEnable(bool enable){ IsEnabled = enable;  }

	void	 BindScreenFBO(glm::vec2 ScreenSize);
	void	 UnBindScreenFBO();

	bool		AddPostEffect(bgShaderFactory& ShaderFactory, ePhotoEffect effect,bgActor* pTargetActor);
	bool		DeletePostEffect(int  algorithmIndex);
	bgPhotoEffectAlgorithm* GetPostEffect(int algorithmIndex);

	virtual bool Initialize(bgShaderFactory& ShaderFactory);
	virtual void Render(cRenderer& Renderer, cCamera& Camera);

	bgPostProcess();
	virtual ~bgPostProcess();

protected:
	bool		IsEnabled;
	bgActor*  pScreenActor;	
	glm::vec2 RenderSize;
	bgFBOTexture ScreenFBO;
	_PhotoEffectList PhotoEffectList;

	virtual void	 RenderScreen(cRenderer& Renderer, cCamera& Camera);
	bool CreateScreenFBO(glm::vec2 ScreenSize);
};