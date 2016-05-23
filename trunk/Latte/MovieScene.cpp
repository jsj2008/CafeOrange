#include "stdafx.h"
#include "MovieScene.h"
#include"bgMovieAlgorithm.h"

MovieScene::MovieScene(eSceneType	_SceneType) : cScene(_SceneType)
{
	pMovieMaterial = NULL;
	pMovieShader = NULL;
	pMovieData = NULL;
	pMovieActor = NULL;
}

MovieScene::~MovieScene()
{

}

void MovieScene::Initialize()
{
	cScene::Initialize();

	Background.SetGrid(false);
	Background.BackgroundInfo.Color[(int)cBackground::cBackgroundInfo::POSITION_LEFTTOP]				= glm::vec3(0.6, 0, 0);
	Background.BackgroundInfo.Color[(int)cBackground::cBackgroundInfo::POSITION_RIGHTTOP]			= glm::vec3(0.6, 0, 0);
	Background.BackgroundInfo.Color[(int)cBackground::cBackgroundInfo::POSITION_LEFTBOTTOM]	= glm::vec3(0.2, 0.1, 0);
	Background.BackgroundInfo.Color[(int)cBackground::cBackgroundInfo::POSITION_RIGHTBOTTOM] = glm::vec3(0.2, 0.1, 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////Initialize Movie Actor
	pMovieMaterial = new bgMaterial();
	pMovieShader	= new bgShader();
	pMovieData		= new bgData();
	pMovieActor		= new bgActor();

	pMovieActor->setShader(pMovieShader);
	bgMapper* pMapper = new bgMapper();
	pMapper->setInput(pMovieData);
	pMovieActor->setInput(pMapper);
	pMovieActor->setMaterial(pMovieMaterial);

	ActorList.push_back(pMovieActor);
}

void MovieScene::SetMovieTexture(bgTexture* pTexture, cRenderer& Renderer)
{
	if (pMovieActor == 0)
		return;

	bgMaterial*pMaterial = pMovieActor->getMaterial();
	pMaterial->ChangeTexture(0, pTexture);
	pMovieActor->build(Renderer);
}

void	MovieScene::AddMovieEffect(eMovieEffect movieEffect)
{
	//bgActor* actor = FindActorByName(std::string("Movie_Rectangle_PTC"));
	//bgMapper* pMapper = actor->getInput();
	//bgMovieAlgorithm* pAlgo = new  bgMovieAlgorithm();	
	//pMapper->setAlgorithm(0, pAlgo);	
}

void MovieScene::InitializeActor(cRenderer& Renderer)
{
	pMovieMaterial->LoadTexture(std::string("../Media/Texture/ghost05.jpg"));
	
	bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory.requestVertexShader(VERTEX_XYZ_COLOR_TEX0);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory.requestFragmentShader(VERTEX_XYZ_COLOR_TEX0);

	ASSERT(pVertexShaderAlgorithm != 0);
	ASSERT(pFragmentShaderAlgorithm != 0);

	pMovieShader->AddShader(pVertexShaderAlgorithm);
	pMovieShader->AddShader(pFragmentShaderAlgorithm);
	pMovieData->generate(SOURCE_2D_MOVIE_RECTANGLE_PTC);

	pMovieActor->initialize();
	pMovieActor->build(Renderer);
}

void MovieScene::Update()
{
	cScene::Update();
}

void MovieScene::Build()
{
	cScene::Build();
}

void MovieScene::PreRender(cRenderer& Renderer, cCamera* pCamera)
{
	Renderer.SetFillMode(FILLMODE_SOLID);
	Renderer.SetShadingModel(SHADING_MODEL_SMOOTH);
	if (pMovieActor)
	{
		pMovieActor->build(Renderer);
		Background.RenderActor(Renderer, *pCamera, *pMovieActor);
	}
	else
		Background.Render(Renderer, pCamera);
	Renderer.SetFillMode(fillMode);
	Renderer.SetShadingModel(shadingModel);

}
void MovieScene::Render(cRenderer& Renderer, cCamera* pCamera)
{
	//cScene::Render(Renderer, pCamera);
}
void MovieScene::PostRender(cRenderer& Renderer, cCamera* pCamera)
{

}
