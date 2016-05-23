#include "stdafx.h"
#include "bgMapper.h"


bgMapper::bgMapper()
{		
	pbgData = 0;
	modified = true;
}

bgMapper::~bgMapper()
{

}

void	bgMapper::clearAlgorithm()
{
	_AlgorithmChainMap::iterator iAlgo = AlgorithmChainMap.begin();
	for (; iAlgo != AlgorithmChainMap.end(); iAlgo++)
	{
		bgAlgorithm* pAlgo = iAlgo->second;
		delete pAlgo;
	}

	AlgorithmChainMap.clear();
}

void bgMapper::setVertexType(eVertexType _type)
{
	if (vertexType == _type)
		return;

	vertexType = _type;
}

void	bgMapper::setDrawElementType(eDrawElementType _drawElementType)
{
	drawElementType = _drawElementType;
}

bool bgMapper::updateAlgorithm(float t)
{
	bool bRes = false;
	for (int index = 0; index < AlgorithmChainMap.size(); index++)
	{
	/*	bgAlgorithm* pCurrnetAlgo = AlgorithmChainMap[index];
		bgAlgorithm* pBeforeAlgo = NULL;
		bgData*		pbgData = NULL;
		bgTexture* pbgTexture = NULL;
		if (index > 0)
		{
			pBeforeAlgo = AlgorithmChainMap[index - 1];
			pbgData = pBeforeAlgo->GetBgData();
			pbgTexture = pBeforeAlgo->GetBgTexture();

		}

		bRes = pCurrnetAlgo->Update(t);
		if (!bRes) break;*/
	}

	return bRes;
}

bool bgMapper::execute()
{	
	bool bRes = false;
	for (int index = 0; index < AlgorithmChainMap.size(); index++)
	{
		//bgAlgorithm* pCurrnetAlgo = AlgorithmChainMap[index];
		//bgAlgorithm* pBeforeAlgo = NULL;
		//bgData*		pbgData		 = NULL;
		//bgTexture* pbgTexture = NULL;
		//if (index > 0)
		//{
		//	pBeforeAlgo = AlgorithmChainMap[index - 1];
		//	pbgData = pBeforeAlgo->GetBgData();
		//	pbgTexture = pBeforeAlgo->GetBgTexture();
		//}

		//bRes = pCurrnetAlgo->Execute();
		//if (!bRes) break;
	}


	return bRes;
}

void	bgMapper::setAlgorithm(int index, bgAlgorithm* pAlgorithm)
{
	AlgorithmChainMap[index] = pAlgorithm;
}

bgAlgorithm*	bgMapper::getAlgorithm(int index)
{
	_AlgorithmChainMap::iterator iAlgo = AlgorithmChainMap.find(index);
	if (iAlgo != AlgorithmChainMap.end())
		return iAlgo->second;

	return NULL;
}

const bgAlgorithm*	bgMapper::getAlgorithm(int index) const
{
	_AlgorithmChainMap::const_iterator iAlgo = AlgorithmChainMap.find(index);
	if (iAlgo != AlgorithmChainMap.end())
		return iAlgo->second;

	return NULL;
}



std::string bgMapper::GetName()
{
	if (pbgData != 0)
	{
		switch (pbgData->GetSourceType())
		{
		case SOURCE_2D_TRIANGLE_PC:				return std::string("Triangle_PC"); break;
		case SOURCE_2D_RECTANGLE_PC:			return std::string("Rectangle_PC"); break;
		case	SOURCE_2D_CIRCLE_PC:					return std::string("Circle_PC"); break;
		case	SOURCE_2D_LINE_PC:							return std::string("Line_PC"); break;

		case SOURCE_2D_TRIANGLE_PTC:				return std::string("Triangle_PTC"); break;
		case SOURCE_2D_RECTANGLE_PTC:			return std::string("Rectangle_PTC"); break;
		case SOURCE_2D_MOVIE_RECTANGLE_PTC: return std::string("Movie_Rectangle_PTC"); break;
		case	SOURCE_2D_CIRCLE_PTC:					return std::string("Circle_PTC"); break;
		case	SOURCE_2D_LINE_PTC:						return std::string("Line_PTC"); break;

		case SOURCE_3D_CUBE_PC:						return std::string("Cube_PC"); break;
		case SOURCE_3D_SPHERE_PC:					return std::string("Sphere_PC"); break;
		case SOURCE_3D_PLANE_PC:						return std::string("Plane_PC"); break;
		case SOURCE_3D_CYLINDER_PC:				return std::string("Cylinder_PC"); break;

		case SOURCE_3D_CUBE_PNC:						return std::string("Cube_PNC"); break;
		case SOURCE_3D_SPHERE_PNC:					return std::string("Sphere_PNC"); break;
		case SOURCE_3D_PLANE_PNC:						return std::string("Plane_PNC"); break;
		case SOURCE_3D_CYLINDER_PNC:				return std::string("Cylinder_PNC"); break;
		}
	}
	return std::string("Not Reserved");
}


////////////////////////////////////////////////////////////////////
///bgSplineMapper
bgSplineMapper::bgSplineMapper()
{
	resolution = 40;
	bgAlgorithm* pbgAlgorithm = new bgSplineCurveAlgorithm();
	pbgData = new bgData();
	pbgData->vertexType = VERTEX_XYZ_COLOR;
	pbgData->drawElementType = ELEMENT_TYPE_LINES;

	pbgAlgorithm->SetBgData(pbgData);

	setAlgorithm(0, pbgAlgorithm);
}

bgSplineMapper::~bgSplineMapper()
{
	clearAlgorithm();
}

void bgSplineMapper::SetControlPoints(std::vector<glm::vec3>& _controlPoints)
{
	controlPoints.clear();
	controlPoints.resize(_controlPoints.size());
	std::copy(_controlPoints.begin(), _controlPoints.end(), controlPoints.begin());
	dynamic_cast<bgSplineCurveAlgorithm*>(getAlgorithm(0))->SetControlPoints(controlPoints, resolution);
	initialize();
}

void	bgSplineMapper::SetResolution(int _resolution)
{
	resolution = _resolution;
	dynamic_cast<bgSplineCurveAlgorithm*>(getAlgorithm(0))->SetResolution(resolution);
	initialize();
}

void bgSplineMapper::TranslateControlPoint(int index, glm::vec2& Differential)
{
	if (index <0 || index >= controlPoints.size())
		return;

	controlPoints[index].x += Differential.x;
	controlPoints[index].y += Differential.y;
	initialize();
}

bool bgSplineMapper::initialize()
{
	getAlgorithm(0)->Initialize();
	dynamic_cast<bgSplineCurveAlgorithm*>(getAlgorithm(0))->SetControlPoints(controlPoints, resolution);
	return false;
}

////////////////////////////////////////////////////////////////////
///bgBezierPatchMapper
bgBezierPatchMapper::bgBezierPatchMapper()
{
	uSize = 4;
	vSize =  4;
	stepSize = 1.0f;
	bgAlgorithm*  pbgAlgorithm = new bgBezierPatchAlgorithm();
	pbgData = new bgData();
	pbgData->vertexType = VERTEX_XYZ_COLOR_TEX0; // VERTEX_XYZ_COLOR;
	pbgData->drawElementType = ELEMENT_TYPE_TRIANGLES;

	pbgAlgorithm->SetBgData(pbgData);
	setAlgorithm(0, pbgAlgorithm);
}

bgBezierPatchMapper::~bgBezierPatchMapper()
{

}


bool bgBezierPatchMapper::initialize()
{
	if (ControlPointList.size() == 0){
		MakeControlPointIndex();
		return true;
	}
	
	_ControlPointList::iterator iControl = ControlPointList.begin();
	for (; iControl != ControlPointList.end(); iControl++)
	{
		ControlPointIndex* pIndex = iControl->first;
		pIndex->Initialize();
		iControl->second = glm::vec3();
	}

	getAlgorithm(0)->Initialize();

	return true;
}

void bgBezierPatchMapper::Reset()
{
	SetControlPoints(stepSize);
}

bool bgBezierPatchMapper::updateAlgorithm(float t)
{
	return false;
}

void bgBezierPatchMapper::SetControlPoints(float StepSize)
{
	stepSize = StepSize;
	const glm::vec3 P00_Point = glm::vec3(-(stepSize + stepSize / 2), 0.0f, 0.0f);
	for (int u = 0; u < uSize; u++)
	{
		float ypos = u * stepSize;
		for (int v = 0; v < vSize; v++)
		{
			float xpos = v*stepSize;
			ControlPointIndex* pIndex = FindControlPointIndex(u, v);
			ASSERT(pIndex != NULL);
			_ControlPointList::iterator iControl = ControlPointList.find(pIndex);
			ASSERT(iControl != ControlPointList.end());
			iControl->second = glm::vec3(P00_Point.x + xpos, P00_Point.y+ ypos, P00_Point.z);

		/*	CString message;
			message.Format("(%d,%d) : %f,%f,%f \n", u, v, iControl->second.x, iControl->second.y, iControl->second.z);
			OutputDebugString(message);*/
		}
	}

	dynamic_cast<bgBezierPatchAlgorithm*>(getAlgorithm(0))->SetControlNet(ControlPointList, uSize, vSize);

}

void	bgBezierPatchMapper::SetResolution(int _resolution)
{

}

void bgBezierPatchMapper::TranslateControlPoint(int u,int v, glm::vec2& Differential)
{
	ControlPointIndex* indexInfo = FindControlPointIndex(u, v);
	ASSERT(indexInfo != NULL);
	_ControlPointList::iterator iControl = ControlPointList.find(indexInfo);
	ASSERT(iControl != ControlPointList.end());
	glm::vec3 position = iControl->second;
	position.x += Differential.x;
	position.y += Differential.y;
	iControl->second = position;

	dynamic_cast<bgBezierPatchAlgorithm*>(getAlgorithm(0))->SetControlNet(ControlPointList, uSize, vSize);


}

void	 bgBezierPatchMapper::MakeControlPointIndex()
{
	for (int u = 0; u < uSize; u++)
	{
		for (int v = 0; v < vSize; v++)
		{
			ControlPointIndex* pIndex = new ControlPointIndex;
			pIndex->row = u; pIndex->column = v;
			ControlPointList.insert(std::make_pair(pIndex, glm::vec3()));
		}
	}

	_ControlPointList::iterator iControl = ControlPointList.begin();
	for (; iControl != ControlPointList.end(); iControl++)
	{
		ControlPointIndex* pIndex = iControl->first;
		pIndex->pNeighbor = new NeighborControlPoints;

		int u = pIndex->row;
		int v = pIndex->column;

		if ((u - 1) >= 0){
			ControlPointIndex* pDownIndex = FindControlPointIndex((u - 1), v);
			ASSERT(pDownIndex != NULL);
			pIndex->pNeighbor->pDown = pDownIndex;
		}

		if ((u + 1) < uSize){
			ControlPointIndex* pUpIndex = FindControlPointIndex((u + 1), v);
			ASSERT(pUpIndex != NULL);
			pIndex->pNeighbor->pUp = pUpIndex;
		}

		if ((v - 1) >= 0){
			ControlPointIndex* pLeftIndex = FindControlPointIndex(u, (v - 1));
			ASSERT(pLeftIndex != NULL);
			pIndex->pNeighbor->pLeft = pLeftIndex;
		}

		if ((v + 1) < vSize){
			ControlPointIndex* pRightIndex = FindControlPointIndex(u, (v + 1));
			ASSERT(pRightIndex != NULL);
			pIndex->pNeighbor->pRight = pRightIndex;
		}
	}

}

ControlPointIndex* bgBezierPatchMapper::FindControlPointIndex(int u, int v)
{
	_ControlPointList::iterator iControl = ControlPointList.begin();
	for (; iControl != ControlPointList.end(); iControl++)
	{
		ControlPointIndex* pIndex = iControl->first;
		if (pIndex->row == u && pIndex->column == v)
			return pIndex;
	}

	return NULL;
}

glm::vec3	bgBezierPatchMapper::FindControlPointPosition(int u, int v)
{
	_ControlPointList::iterator iControl = ControlPointList.begin();
	for (; iControl != ControlPointList.end(); iControl++)
	{
		ControlPointIndex* pIndex = iControl->first;
		if (pIndex->row == u && pIndex->column == v)
			return iControl->second;
	}

	return glm::vec3();
}
