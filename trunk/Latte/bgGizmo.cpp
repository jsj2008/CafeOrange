#include "stdafx.h"
#include "bgActor.h"
#include "bgShaderFactory.h"
#include "bgGizmo.h"
#include "Camera.h"



//////////////////////////////////////////////////////////////////////////////////////
////bgGizmo
bgGizmo::bgGizmo()
{
	pSelectedActor = 0;
	
	enableMark  = true;
	enableGuideLine = true;

	bgData* pData = new bgData();
	pData->sourceType	= SOURCE_2D_LINE_PC;
	pData->vertexType		= VERTEX_XYZ_COLOR;
	pData->drawElementType = ELEMENT_TYPE_LINES;
	bgMapper* pMapper = new bgMapper();
	pMapper->setInput(pData);
	
	pGuideLine = new bgActor();
	pMarkShader = new bgShader();	
	pMarkShader->SetShaderType(SHADER_TYPE_PHONG);

	pLineShader  = new bgShader();
	pGuideLine->setInput(pMapper);

	pTargetActor = 0;
}

bgGizmo::~bgGizmo()
{
	if (pGuideLine) delete pGuideLine;
}

void bgGizmo::SetTargetActor(bgActor* pActor)
{
	pTargetActor = pActor;
	transformMat4 = pTargetActor->GetTransformMat4();
}

bool bgGizmo::Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory, cRenderer* pRenderer)
{
	pSelectedActor = pActor;
	//MarkList.clear();

	SetTargetActor(pSelectedActor);

	bgShaderAlgorithm* pVertexShaderAlgorithm = ShaderFactory->requestVertexShader(VERTEX_XYZ_NORMAL_DIFFUSE);
	bgShaderAlgorithm* pFragmentShaderAlgorithm = ShaderFactory->requestFragmentShader(VERTEX_XYZ_NORMAL_DIFFUSE);

	pMarkShader->AddShader(pVertexShaderAlgorithm);
	pMarkShader->AddShader(pFragmentShaderAlgorithm);

	pVertexShaderAlgorithm = ShaderFactory->requestVertexShader(VERTEX_XYZ_COLOR);
	pFragmentShaderAlgorithm = ShaderFactory->requestFragmentShader(VERTEX_XYZ_COLOR);

	pLineShader->AddShader(pVertexShaderAlgorithm);
	pLineShader->AddShader(pFragmentShaderAlgorithm);

	pGuideLine->setShader(pLineShader);

	return true;
}

bgActor*	bgGizmo::MakeTargetMark(glm::vec3& translation, cRenderer* pRenderer, bgSphereShapeInfo* pInfo)
{
	bgActor* targetActor = new bgActor();
	targetActor->setShader(pMarkShader);

	bgSphereShapeInfo* pSphereInfo = pInfo;
	if (pSphereInfo == NULL){
		pSphereInfo = new bgSphereShapeInfo();
		pSphereInfo->Radius = 0.1f;
		pSphereInfo->Sectors = 10.0f;
		pSphereInfo->Rings = 10.0f;
	}
	bgData* pData = new bgData();
	pData->generate(SOURCE_3D_SPHERE_PC, pSphereInfo);

	bgMapper* pMapper = new bgMapper();
	pMapper->setInput(pData);

	targetActor->setTranslation(translation);
	targetActor->setInput(pMapper);
	targetActor->initialize();
	targetActor->build(*pRenderer);

	return targetActor;
}
//////////////////////////////////////////////////////////////////////////////////////
////bgControlPointsGizmo
void bgControlPointsGizmo::AddMark(glm::vec3& translation, cRenderer* pRenderer)
{
	if (bgActor* targetActor = MakeTargetMark(translation, pRenderer))
		MarkList.push_back(targetActor);
}

bgActor* bgControlPointsGizmo::SelectControlPoint(cRenderer& Renderer, cCamera& Camera, int x, int y)
{
	glm::vec2 WindowSize = Camera.GetWindowSize();
	if (MarkList.size() <=0 || x < 0 || x >= WindowSize.x || y < 0 || y >= WindowSize.y)
		return 0;

	glm::vec4 origin = glm::vec4(
		(2.0f*x) / WindowSize.x - 1.0f,
		1 - (2.0f*y) / WindowSize.y,
		-1, 1
		);


	glm::mat4 InverseMVP = glm::inverse(Camera.GetWorldViewMatrix())* glm::inverse(Camera.GetProjectionMatrix());
	origin = InverseMVP *origin;
	glm::vec3 RayOrigin = glm::vec3(origin.x, origin.y, origin.z);

	bgRay ray(RayOrigin, Camera.GetEyePos());

	glm::vec3 IntersectedPoint;
	_MarkList::iterator iMark = MarkList.begin();
	for (int index = 0; iMark != MarkList.end(); iMark++,index++)
	{
		bgActor* pActor = *iMark;
		bgData*	 pbgData = pActor->getInput()->getInput();
		const bgSphereShapeInfo * pInfo = dynamic_cast<bgSphereShapeInfo*>(pbgData->GetShapeInfo());
		glm::mat4 transformedMat = pActor->GetTransformMat4();
		bool IsIntersected = ray.CalculateRayOnSphere(pInfo, transformedMat, IntersectedPoint);

		if (IsIntersected) {
			//CString message;
			//CString data;
			//data.Format("Mark Index:%d is selected. \n", index);

			//message += data;
			//OutputDebugString(message);
			return pActor;
		}
	}

	return NULL;
}

void bgGizmo::MoveControlPoint(cRenderer& Renderer ,glm::vec2& Differential, bgActor* pSelectedActor)
{

}

void bgGizmo::Draw(cRenderer& Renderer, cCamera* pCamera,bgActor* pSelectedActor, bgShader* pSelectedShader)
{

}


//////////////////////////////////////////////////////////////////////////////////////
////bgControlPointsGizmo
bgControlPointsGizmo::bgControlPointsGizmo()
{
	pMapper = 0;
}

bgControlPointsGizmo::~bgControlPointsGizmo()
{

}

bool bgControlPointsGizmo::Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory,cRenderer* pRenderer)
{
	ASSERT(pActor != 0);
	bgGizmo::Initialize(pActor, ShaderFactory,pRenderer);

	pMapper = dynamic_cast<bgSplineMapper*>(pActor->getInput());
	if (pMapper == 0)
		return false;
	
	PickingPoints = pMapper->GetControlPoints();
	if (PickingPoints.size() <= 0)
		return false;

	pGuideLine->getInput()->getInput()->initialize();
	pGuideLine->initialize();

	bgData* pData = pGuideLine->getInput()->getInput();
	bgData::_PositionList& PositionList = pData->GetPositionList();
	bgData::_ColorList&		ColorList		= pData->GetColorList();
	bgData::_IndexList&		IndexList		= pData->GetIndexList();

	std::vector<glm::vec3>::iterator iControl = PickingPoints.begin();
	for (; iControl != PickingPoints.end(); iControl++)
	{
		PositionList.push_back((*iControl).x);
		PositionList.push_back((*iControl).y);
		PositionList.push_back((*iControl).z);

		ColorList.push_back(0.0f);
		ColorList.push_back(1.0f);
		ColorList.push_back(0.0f);

		AddMark(*iControl, pRenderer);
	}

	for (int index = 0; index < PositionList.size() / 3-1; index++){
		IndexList.push_back(index); IndexList.push_back(index+1);
	}

	pGuideLine->build(*pRenderer);

	return true;
}

void bgControlPointsGizmo::MoveControlPoint(cRenderer& Renderer,glm::vec2& Differential, bgActor* pSelectedActor)
{
	if (pSelectedActor == NULL)
		return;
	
	int index = 0;
	_MarkList::iterator iMark = MarkList.begin();
	for (; iMark != MarkList.end(); iMark++){
		bgActor* pActor = *iMark;
		if (pActor == pSelectedActor){			
			break;
		}
		index++;
	}

	if (index <0 || index >= MarkList.size())
		return;

	pMapper->TranslateControlPoint(index, Differential);
	pMapper->execute();

	bgData* pData = pGuideLine->getInput()->getInput();
	bgData::_PositionList& PositionList = pData->GetPositionList();

	PositionList[3 * index + 0] += Differential.x;
	PositionList[3 * index + 1] += Differential.y;
	pGuideLine->build(Renderer);

	pSelectedActor->setTranslation(glm::vec3(PositionList[3 * index + 0], PositionList[3 * index + 1], PositionList[3 * index + 2]));
	pSelectedActor->build(Renderer);

}

void bgControlPointsGizmo::Draw(cRenderer& Renderer, cCamera* pCamera,bgActor* pSelectedActor,bgShader* pSelectedShader)
{
	glm::mat4  cameraProjectMat4 = pCamera->GetProjectionMatrix() * pCamera->GetWorldViewMatrix();

	Renderer.BindShader(pLineShader, 0, cameraProjectMat4, transformMat4);
		Renderer.RenderActor(pGuideLine);
	Renderer.UnbindShader(pLineShader);

	_MarkList::iterator iMark = MarkList.begin();
	for (; iMark != MarkList.end(); iMark++)
	{
		bgActor* pActor = *iMark;
		bgShader* pShader = pMarkShader;

		if (pActor == pSelectedActor){
	/*		pShader = pSelectedShader;
			pActor->setShader(pShader);
			pActor->initialize();
			pActor->build(Renderer);*/
		}

		Renderer.BindShader(pShader, 0, cameraProjectMat4, pActor->GetTransformMat4());
			Renderer.RenderActor(pActor);
		Renderer.UnbindShader(pShader);

	}

}


//////////////////////////////////////////////////////////////////////////////////////
////bgBezierPatchControlPointsGizmo
bgBezierPatchControlPointsGizmo::bgBezierPatchControlPointsGizmo()
{
	pMapper = 0;
}

bgBezierPatchControlPointsGizmo::~bgBezierPatchControlPointsGizmo()
{

}

bool bgBezierPatchControlPointsGizmo::Initialize(bgActor* pActor, bgShaderFactory* ShaderFactory, cRenderer* pRenderer)
{
	ASSERT(pActor != 0);
	bgGizmo::Initialize(pActor, ShaderFactory, pRenderer);

	pMapper = dynamic_cast<bgBezierPatchMapper*>(pActor->getInput());
	if (pMapper == 0) return false;
	pMapper->Reset();

	InitializeMarkList();

	bgBezierPatchMapper::_ControlPointList& PatchList = pMapper->GetPatchControlPoints();
	pGuideLine->getInput()->getInput()->initialize();
	pGuideLine->initialize();

	bgData* pData = pGuideLine->getInput()->getInput();
	pData->initialize();
	bgData::_PositionList& PositionList = pData->GetPositionList();
	bgData::_ColorList&		ColorList = pData->GetColorList();
	bgData::_IndexList&		IndexList = pData->GetIndexList();

	bool bCreatedMode = MarkList.size() <= 0 ? true : false;
	CString message;
	int uSize = pMapper->GetUSize();
	int vSize = pMapper->GetVSize();
	for (int u = 0; u < uSize; u++)
	{
		for (int v = 0; v < vSize; v++)
		{
			glm::vec3 position = pMapper->FindControlPointPosition(u, v);
			PositionList.push_back(position.x);
			PositionList.push_back(position.y);
			PositionList.push_back(position.z);

			ColorList.push_back(1.0f);
			ColorList.push_back(0.0f);
			ColorList.push_back(0.0f);
			
			ControlPointIndex* indexInfo = pMapper->FindControlPointIndex(u, v);
			ASSERT(indexInfo != NULL);
			if (u == 0 || v == 0 || u == uSize - 1 || v == vSize - 1)
				indexInfo->bEdgePoint = true;
			else
				indexInfo->bEdgePoint = false;

			//CString message;
			//message.Format("position:%f,%f,%f \n", position.x, position.y, position.z);
			//OutputDebugString(message);
			if (bCreatedMode)
				AddMark(indexInfo, position, pRenderer);
			else
				ResetMark(indexInfo, position, pRenderer);
		}
	}
	

	for (int u = 0; u < uSize  ; u++)
	{
		for (int v = 0; v < vSize-1; v++)
		{
			IndexList.push_back( u*vSize + v); 
			IndexList.push_back( u*vSize + v+ 1);
//			message.Format("U Base Index :%d, %d\n", (u*vSize + v), (u*vSize + v + 1));
//			OutputDebugString(message);
		}		
	}

	for (int v = 0; v < vSize; v++)
	{
		for (int u = 0; u < uSize-1 ; u++)
		{
			IndexList.push_back(u*vSize + v);
			IndexList.push_back((u + 1)*vSize + v);
//			message.Format("V Base Index :%d, %d\n", (u*vSize + v), ( (u+1)*vSize + v) );
//			OutputDebugString(message);
		}
	}

	pGuideLine->build(*pRenderer);

	return true;
}

void bgBezierPatchControlPointsGizmo::AddMark(ControlPointIndex* indexInfo,glm::vec3& translation, cRenderer* pRenderer)
{
	bgSphereShapeInfo* pSphereInfo = new bgSphereShapeInfo();
	pSphereInfo->Radius = 0.05f;
	pSphereInfo->Sectors = 10.0f;
	pSphereInfo->Rings = 10.0f;

	if (bgActor* targetActor = MakeTargetMark(translation, pRenderer, pSphereInfo)){
		char name[100] = { 0, };
		sprintf_s(name, "mark%d%d", indexInfo->row, indexInfo->column);
		targetActor->name = name;
		
		MarkList.insert(std::make_pair(indexInfo, targetActor));
	}
}

void bgBezierPatchControlPointsGizmo::ResetMark(ControlPointIndex* indexInfo, glm::vec3& translation, cRenderer* pRenderer)
{
	char name[100] = { 0, };
	sprintf_s(name, "mark%d%d", indexInfo->row, indexInfo->column);
	std::string markname = name;
	_MarkList::iterator iMark = MarkList.begin();
	for (int index = 0; iMark != MarkList.end(); iMark++, index++)
	{
		bgActor* pActor = iMark->second;
		if (pActor->name.compare(markname) == 0){
			pActor->setTranslation(translation);
		}
	}
}

void bgBezierPatchControlPointsGizmo::InitializeMarkList()
{
	glm::vec3 IntersectedPoint;
	_MarkList::iterator iMark = MarkList.begin();
	for (int index = 0; iMark != MarkList.end(); iMark++, index++)
	{
		bgActor* pActor = iMark->second;
		pActor->intializeTransformed();
	}

}

bgActor* bgBezierPatchControlPointsGizmo::SelectControlPoint(cRenderer& Renderer, cCamera& Camera, int x, int y)
{
	glm::vec2 WindowSize = Camera.GetWindowSize();
	if (MarkList.size() <= 0 || x < 0 || x >= WindowSize.x || y < 0 || y >= WindowSize.y)
		return 0;

	glm::vec4 origin = glm::vec4(
		(2.0f*x) / WindowSize.x - 1.0f,
		1 - (2.0f*y) / WindowSize.y,
		-1, 1
		);


	glm::mat4 InverseMVP = glm::inverse(Camera.GetWorldViewMatrix())* glm::inverse(Camera.GetProjectionMatrix());
	origin = InverseMVP *origin;
	glm::vec3 RayOrigin = glm::vec3(origin.x, origin.y, origin.z);

	bgRay ray(RayOrigin, Camera.GetEyePos());

	glm::vec3 IntersectedPoint;
	_MarkList::iterator iMark = MarkList.begin();
	for (int index = 0; iMark != MarkList.end(); iMark++, index++)
	{
		bgActor* pActor = iMark->second;
		bgData*	 pbgData = pActor->getInput()->getInput();
		const bgSphereShapeInfo * pInfo = dynamic_cast<bgSphereShapeInfo*>(pbgData->GetShapeInfo());
		glm::mat4 transformedMat = pActor->GetTransformMat4();
		bool IsIntersected = ray.CalculateRayOnSphere(pInfo, transformedMat, IntersectedPoint);

		if (IsIntersected) {
			//CString message;
			//CString data;
			//data.Format("Mark Index:%d is selected. \n", index);
			//message += data;
			//OutputDebugString(message);
			return pActor;
		}
	}

	return NULL;
}

void bgBezierPatchControlPointsGizmo::Draw(cRenderer& Renderer, cCamera* pCamera, bgActor* pSelectedActor, bgShader* pSelectedShader)
{
	glm::mat4  cameraProjectMat4 = pCamera->GetProjectionMatrix() * pCamera->GetWorldViewMatrix();

	if (enableGuideLine)
	{
		Renderer.BindShader(pLineShader, 0, cameraProjectMat4, transformMat4);
			Renderer.RenderActor(pGuideLine);
		Renderer.UnbindShader(pLineShader);
	}

	if (enableMark)
	{
		_MarkList::iterator iMark = MarkList.begin();
		for (; iMark != MarkList.end(); iMark++)
		{
			ControlPointIndex* pIndex = iMark->first;
			bgActor* pActor = iMark->second;
			bgShader* pShader = pMarkShader;

			if (pActor == pSelectedActor){
				//	pShader = pSelectedShader;
				//	pActor->setShader(pShader);
				//	pActor->initialize();
				//	pActor->build(Renderer);
			}

			glm::mat4 mat4 = pActor->GetTransformMat4() * transformMat4;
			Renderer.BindShader(pShader, 0, cameraProjectMat4, mat4);
				Renderer.RenderActor(pActor);
			Renderer.UnbindShader(pShader);
		}
	}
}

void bgBezierPatchControlPointsGizmo::MoveControlPoint(cRenderer& Renderer,  glm::vec2& Differential, bgActor* pSelectedActor)
{
	if (pSelectedActor == NULL)
		return;

	bgActor* pTargetActor = NULL;
	_MarkList::iterator iMark = MarkList.begin();
	for (; iMark != MarkList.end(); iMark++)
	{
		bgActor* pActor = iMark->second;
		if (pSelectedActor == pActor){
			pTargetActor = pActor;
			break;
		}
	}

	if (pTargetActor == NULL)
		return;

	ControlPointIndex* pIndexInfo = GetMarkIndex(pTargetActor);
	ASSERT(pIndexInfo != NULL);

	pMapper->TranslateControlPoint(pIndexInfo->row, pIndexInfo->column, Differential);
	pMapper->execute();

	glm::vec3 position = pMapper->FindControlPointPosition(pIndexInfo->row, pIndexInfo->column);
	pSelectedActor->setTranslation(position);
	pSelectedActor->build(Renderer);

	bgBezierPatchMapper::_ControlPointList& PatchList = pMapper->GetPatchControlPoints();
	bgData* pData = pGuideLine->getInput()->getInput();
	bgData::_PositionList& PositionList = pData->GetPositionList();
	PositionList.clear();
	int uSize = pMapper->GetUSize();
	int vSize = pMapper->GetVSize();
	for (int u = 0; u < uSize; u++)
	{
		for (int v = 0; v < vSize; v++)
		{
			glm::vec3 position = pMapper->FindControlPointPosition(u, v);
			PositionList.push_back(position.x);
			PositionList.push_back(position.y);
			PositionList.push_back(position.z);
		}
	}

	pGuideLine->build(Renderer);

}

ControlPointIndex* bgBezierPatchControlPointsGizmo::GetMarkIndex(bgActor* pActor)
{
	_MarkList::iterator iMark = MarkList.begin();
	for (; iMark != MarkList.end(); iMark++)
	{
		bgActor* pMarkActor = iMark->second;
		if (pMarkActor == pActor)
			return iMark->first;
	}

	return NULL;
}


