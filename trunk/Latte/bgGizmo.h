#pragma once
#include <vector>
using namespace std;

class cRenderer;
class bgActor;
class bgData;
class bgShader;
class bgShaderFactory;
class cCamera;
//////////////////////////////////////////////////////////////////////////////////////
////bgGizmo
class bgGizmo
{
public:
	std::string name;
	bool enableMark;
	bool enableGuideLine;

	virtual void SetTargetActor( bgActor* pActor);
	virtual bgActor* GetTargetActor(){ return pTargetActor;  }
	virtual bool Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory, cRenderer* pRenderer);
	virtual void Draw(cRenderer& Renderer, cCamera* pCamera, bgActor* pSelectedActor, bgShader* pSelectedShader);
	virtual void MoveControlPoint(cRenderer& Renderer, glm::vec2& Differential, bgActor* pSelectedActor);
	
	virtual bgActor* SelectControlPoint(cRenderer& Renderer, cCamera& Camera, int x, int y){ return NULL;  };

	bgGizmo();
	virtual ~bgGizmo();


protected:	
	glm::mat4	 transformMat4;
	bgActor*	 pGuideLine;
	bgActor*	 pSelectedActor;
	bgActor*	 pTargetActor;
	bgShader*	 pMarkShader;
	bgShader*	 pLineShader;
	
	bgActor*	MakeTargetMark(glm::vec3& translation, cRenderer* pRenderer, bgSphereShapeInfo* pInfo = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////
////bgControlPointsGizmo
class bgControlPointsGizmo : public bgGizmo
{
public:
	typedef std::vector<bgActor*> _MarkList;
	typedef std::vector<glm::vec3> _ControlPoints;

	virtual bool Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory,cRenderer* pRenderer);
	virtual void Draw(cRenderer& Renderer, cCamera* pCamera,bgActor* pSelectedActor, bgShader* pSelectedShader);
	virtual void MoveControlPoint(cRenderer& Renderer, glm::vec2& Differential, bgActor* pSelectedActor);
	virtual bgActor* SelectControlPoint(cRenderer& Renderer, cCamera& Camera, int x, int y);

	void AddMark(glm::vec3& translation, cRenderer* pRenderer);

	bgControlPointsGizmo();
	virtual ~bgControlPointsGizmo();

protected:
	_MarkList		   MarkList;
	_ControlPoints  PickingPoints;

	bgSplineMapper* pMapper;

};

//////////////////////////////////////////////////////////////////////////////////////
////bgBezierPatchControlPointsGizmo
class bgBezierPatchControlPointsGizmo : public bgGizmo
{
public:
	typedef std::map<ControlPointIndex*, bgActor*> _MarkList;
	typedef std::vector<glm::vec3> _ControlPoints;

	virtual bool Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory, cRenderer* pRenderer);
	virtual void Draw(cRenderer& Renderer, cCamera* pCamera, bgActor* pSelectedActor, bgShader* pSelectedShader);
	virtual bgActor* SelectControlPoint(cRenderer& Renderer, cCamera& Camera, int x, int y);
	virtual void MoveControlPoint(cRenderer& Renderer, glm::vec2& Differential, bgActor* pSelectedActor);

	void AddMark(ControlPointIndex* indexInfo, glm::vec3& translation, cRenderer* pRenderer);
	void ResetMark(ControlPointIndex* indexInfo, glm::vec3& translation, cRenderer* pRenderer);

	bgBezierPatchControlPointsGizmo();
	virtual ~bgBezierPatchControlPointsGizmo();

protected:
	_MarkList	MarkList;
	_ControlPoints	ControlPoints;

	bgBezierPatchMapper*	pMapper;
	ControlPointIndex* GetMarkIndex(bgActor* pActor);

	void InitializeMarkList();
};



