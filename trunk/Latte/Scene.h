#pragma once
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "bgActor.h"
#include "bgShaderFactory.h"
#include "bgPhotoEffectAlgorithm.h"
#include "bgPostProcess.h"
#include <string>
using namespace std;

class bgActor;
class cBackground
{
public:
	class  cBackgroundInfo
	{
	public:
		enum ePosition{
			POSITION_LEFTTOP,
			POSITION_RIGHTTOP,
			POSITION_RIGHTBOTTOM,
			POSITION_LEFTBOTTOM,			
		};
		int 	TileWidth;
		int 	TileHeight;
		glm::vec3		Color[4];
		std::string		TextureFileName;
		cBackgroundInfo()
		{
			TileWidth = TileHeight = 0;
		}
	};

	enum eBackType{
		TYPE_BACK_GRID,
		TYPE_BACK_COLOR,
		TYPE_BACK_IMAGE,

		TYPE_BACK_ANIMATION,

		TYPE_BACK_ACTOR,

		TYPE_BACK_COUNT,
	};

	eBackType BackType;
	cBackgroundInfo BackgroundInfo;

	void SetGrid(bool bUse);
	void DrawGrid(float Size,float Step);
	void Render(cRenderer& Renderer,cCamera* pCamera=0);
	void RenderGrid(cRenderer& Renderer,cCamera& Camera);
	void RenderGradient(cRenderer& Renderer);
	void RenderTexture(cRenderer& Renderer);
	void RenderAnimation(cRenderer& Renderer);

	void RenderActor(cRenderer& Renderer, cCamera& Camera, bgActor& BackgroundActor);

	cBackground();
	
protected:
	bool UseGrid;
	float GridSize, GridStep;
		
};

class bgObject;
class cShaderEffect;
class cRenderer;
class cScene
{
public:
	eSceneType	SceneType;
	cBackground Background;

	//////////////////////////////////////////////////
	//Rendering Option
	eFillMode			fillMode;
	eShadingModel	shadingModel;
	//////////////////////////////////////////////////

	typedef std::vector<cObject*> _ObjectList;
	_ObjectList ObjectList;

	typedef std::vector<cCamera*> _CameraList;
	_CameraList CameraList;

	typedef std::vector<cLight*> _LightList;
	_LightList LightList;

	typedef std::vector<bgActor*> _ActorList;
	_ActorList ActorList;

	typedef std::map<int, bgPhotoEffectAlgorithm*> _PhotoEffectList;
	_PhotoEffectList PhotoEffectList;

	virtual void Update();
	virtual void Build();
	virtual void Build(cRenderer& Renderer);
	virtual void DoInit(cRenderer& Renderer){ Initialize(); }
	virtual void PreRender(cRenderer& Renderer,cCamera* pCamera=0);
	virtual void Render(cRenderer& Renderer,cCamera* pCamera= 0);
	virtual void PostRender(cRenderer& Renderer,cCamera* pCamera=0);

	void InitializePostProcess();

	virtual void	Initialize();
	virtual void	Release();
	virtual void	Clear(cRenderer& Renderer) { }
	virtual void	ChangeSize(cRenderer& Renderer,int width, int height);

	virtual void InitializeActor(cRenderer& Renderer);
	void	DrawLight(cRenderer& Renderer);
	void	MakeTestScene(cRenderer& Renderer);
	void SetCVFrameToTexture(cv::Mat* cvFrame, cRenderer& Renderer);
	void GetTextureMemory();

	///////////////////////////////////////////////////
	//Management
	bgActor*	FindActorByName(std::string& name);
	bgShaderFactory*	GetShaderFactory() { return &ShaderFactory;  }
	///////////////////////////////////////////////////

	//picking 
	bgActor* SelectModel(cRenderer& Renderer, cCamera& Camera, int x, int y,bool drawHelper=false);
	bgActor* TestIntersction(bgRay& ray);
	bool		IsIntersectedOn2D(eSourceType type, bgRay& ray,int primitiveCount, glm::mat4& transformedMat, bgData* pbgData);
	bool		IsIntersectedOn3D(eSourceType type, bgRay& ray, int primitiveCount,glm::mat4& transformedMat, bgData* pbgData);

	///////////////////////////////////////////////////
	//selection
	void		SetSelectedActor(bgActor* _pActor);
	bgActor*	GetSelectedActor(){ return pSelectedActor;  }
	bgShader*	GetSelectedShader(){ return pSelectedShader;  }
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//texture
	void		ChangeTexture(const std::string& filename);
	void		ChangeTexture(bgTexture* pTexture, cRenderer& Renderer);
	void		AddTexture(const std::string& filename,cRenderer& Renderer);
	void		DeleteTexture(int index, const std::string& filename, cRenderer& Renderer);
	void		ChangeTextureOperation(int stageIndex, eTextureOperation operation);
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//canvas
	void		ChangeCanvasSize(eCanvasSize size, cRenderer& Renderer);
	class bgCanvasActor* GetCanvasActor() { return pImageActor; }
	const class bgCanvasActor* GetCanvasActor() const { return pImageActor; }
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//PhotoEffect
	void		AddPhotoEffect(ePhotoEffect effect);
	void		DeletePhotoEffect(int algorithmIndex);
	bgPhotoEffectAlgorithm*	GetPhotoEffect(int algorithmIndex);
	///////////////////////////////////////////////////

	cScene(eSceneType	_SceneType);
	virtual ~cScene();

protected:
	//////////////////////////////////////////////////
	//Post Processing
	bgPostProcess PostProcess;
	//////////////////////////////////////////////////
	glm::vec2				WindowSize;
	bgShaderFactory	ShaderFactory;
	//////////////////////////////////////////////////
	//Selection
	bgActor*		pSelectedActor;
	bgShader*		pSelectedShader;
	bool					HasSelectedActor;
	//////////////////////////////////////////////////
	//Propoerty

private:
	class bgCanvasActor*		pImageActor;
	class bgData*			pImageData;
	class bgShader*		pImageShader;
	class bgMaterial*	pImageMaterial;
	
};

