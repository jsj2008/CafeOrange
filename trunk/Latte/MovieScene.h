#pragma once
#include "Scene.h"


class MovieScene : public cScene
{
public:
	virtual void	Initialize();
	virtual void Update();
	virtual void Build();

	virtual void PreRender(cRenderer& Renderer, cCamera* pCamera = 0);
	virtual void Render(cRenderer& Renderer, cCamera* pCamera = 0);
	virtual void PostRender(cRenderer& Renderer, cCamera* pCamera = 0);

	virtual void SetMovieTexture(bgTexture* pTexture, cRenderer& Renderer);
	virtual void InitializeActor(cRenderer& Renderer);

	void MakeTestScene(cRenderer& Renderer);
	void	AddMovieEffect(eMovieEffect movieEffect);

	MovieScene(eSceneType	_SceneType);
	virtual ~MovieScene();

private:
	class bgActor* pMovieActor;
	class bgData*	 pMovieData;
	class bgShader* pMovieShader;
	class bgMaterial*pMovieMaterial;

};