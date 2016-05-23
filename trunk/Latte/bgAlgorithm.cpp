#include "stdafx.h"
#include "bgData.h"
#include "bgAlgorithm.h"

bgAlgorithm::bgAlgorithm()
{
	pbgData = 0;
	pbgTexture = 0; 
	modified = false;
}

bgAlgorithm::~bgAlgorithm()
{

}

bgData* bgAlgorithm::GetBgData()
{
	return pbgData;
}

//bgTexture* bgAlgorithm::GetBgTexture()
//{
//	return pbgTexture;
//}

//void bgAlgorithm::SetBgTexture(bgTexture* pTexture, bool bDeepCopy)
//{
//	pbgTexture = pTexture;
//}
//


void	bgAlgorithm::SetBgData(bgData* _pbgData, bool bDeepCopy)
{
	pbgData = _pbgData;
}



////////////////////////////////////////////////////////////////////
///bgCurveAlgorithm
bgCurveAlgorithm::bgCurveAlgorithm()
{
	NumControlPoints = 0;
}

bgCurveAlgorithm::~bgCurveAlgorithm()
{

}

bool bgCurveAlgorithm::Initialize()
{
	NumControlPoints = 0;
	ControlPoints.clear();
	//Vertices.clear();

	return false;
}

bool bgCurveAlgorithm::Update(float t)
{
	return false;
}

bool bgCurveAlgorithm::Execute()
{
	return false;
}

void	 bgCurveAlgorithm::SetResolution(int resolution)
{
	Resolution = resolution;
	modified = true;
}

void	 bgCurveAlgorithm::SetControlPoints(_ControlPoints& points, int resolution)
{
	ControlPoints.clear();
	ControlPoints.resize(points.size());
	std::copy(points.begin(), points.end(), ControlPoints.begin());
	NumControlPoints = ControlPoints.size();
	Resolution = resolution;
	modified = true;
}

glm::vec3 bgCurveAlgorithm::GetPosition(float t)
{
	return glm::vec3();
}


////////////////////////////////////////////////////////////////////
///bgBezierCurveAlgorithm
bgBezierCurveAlgorithm::bgBezierCurveAlgorithm()
{

}

bgBezierCurveAlgorithm::~bgBezierCurveAlgorithm()
{

}

bool bgBezierCurveAlgorithm::Initialize()
{
	return bgCurveAlgorithm::Initialize();
}

bool bgBezierCurveAlgorithm::Update(float time)
{
	modified = true;
	Execute();

	return true;
}

bool bgBezierCurveAlgorithm::Execute()
{
	if (NumControlPoints != 4)
		return false;

	glm::vec3 c0 = ControlPoints[0];
	glm::vec3 c1 = ControlPoints[1];
	glm::vec3 c2 = ControlPoints[2];
	glm::vec3 c3 = ControlPoints[3];

	bgData::_PositionList& PointList = pbgData->GetPositionList();
	bgData::_ColorList& ColorList = pbgData->GetColorList();
	bgData::_IndexList& IndexList = pbgData->GetIndexList();

	PointList.clear(); ColorList.clear();IndexList.clear();

	for (int index = 0; index < Resolution; index++)
	{
		float t = (float)index / (float)Resolution;
		glm::vec3 position;

		float x = B0(t)*c0.x + B1(t)*c1.x + B2(t)*c2.x + B3(t)*c3.x;
		float y = B0(t)*c0.y + B1(t)*c1.y + B2(t)*c2.y + B3(t)*c3.y;
		float z = B0(t)*c0.z + B1(t)*c1.z + B2(t)*c2.z + B3(t)*c3.z;

		PointList.push_back(x);PointList.push_back(y);PointList.push_back(z);
		ColorList.push_back(0.0f); ColorList.push_back(1.0f); ColorList.push_back(0.0f);
	}

	PointList.push_back(c3.x); PointList.push_back(c3.y); PointList.push_back(c3.z);
	ColorList.push_back(0.0f); ColorList.push_back(1.0f); ColorList.push_back(0.0f);

	for (int index = 0; index < PointList.size()/3 -1 ; index++)
	{
		IndexList.push_back(index); IndexList.push_back(index+1);
	}

	return true;
}

glm::vec3 bgBezierCurveAlgorithm::GetPosition(float t)
{
	glm::vec3 position;
	if (NumControlPoints != 4)
		return position;

	glm::vec3 c0 = ControlPoints[0];
	glm::vec3 c1 = ControlPoints[1];
	glm::vec3 c2 = ControlPoints[2];
	glm::vec3 c3 = ControlPoints[3];

	position.x = B0(t)*c0.x + B1(t)*c1.x + B2(t)*c2.x + B3(t)*c3.x;
	position.y = B0(t)*c0.y + B1(t)*c1.y + B2(t)*c2.y + B3(t)*c3.y;
	position.z = B0(t)*c0.z + B1(t)*c1.z + B2(t)*c2.z + B3(t)*c3.z;

	return position;
}

float bgBezierCurveAlgorithm::B0(float t)
{
	return ((1.f - t) * (1.f - t) * (1.f - t));
}

float bgBezierCurveAlgorithm::B1(float t)
{
	return (3.f * t * (1.f - t) * (1.f - t));
}

float bgBezierCurveAlgorithm::B2(float t)
{
	return (3.f * t * t * (1.f - t));
}

float bgBezierCurveAlgorithm::B3(float t)
{
	return (t * t * t);
}

float bgBezierCurveAlgorithm::D0(float t)
{
	return (6.f * t) - (3.f * t * t) - 3.f;
}

float bgBezierCurveAlgorithm::D1(float t)
{
	return 3.f - (12.f * t) + (9.f * t * t);
}

float bgBezierCurveAlgorithm::D2(float t)
{
	return (6.f * t) - (9.f * t * t);
}

float bgBezierCurveAlgorithm::D3(float t)
{
	return (3.f * t * t);
}

////////////////////////////////////////////////////////////////////
///bgSplineCurveAlgorithm
bgSplineCurveAlgorithm::bgSplineCurveAlgorithm()
{
	Span = 0;
	Order = 3;
}

bgSplineCurveAlgorithm::~bgSplineCurveAlgorithm()
{
	

}

bool bgSplineCurveAlgorithm::Initialize()
{
	KnotList.clear();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//Set Knot Vector
	int knotValue = 0;
	for (int index = 0; index < Order + NumControlPoints; index++)
	{
		if (index <= NumControlPoints && index >= Order)
			knotValue++;

		float knot = (float)knotValue / (float)(NumControlPoints - Order + 1);
		KnotList.push_back(knot);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//CString message;
	//for (int index = 0; index < KnotList.size(); index++){
	//	CString data;
	//	data.Format("%d,", KnotList[index]);
	//	message += data;
	//}
	//	
	//OutputDebugString(message);

	return true;
}

bool bgSplineCurveAlgorithm::Update(float t)
{
	bgBezierCurveAlgorithm::Update(t);
	return true;
}

bool bgSplineCurveAlgorithm::Execute()
{
	bgBezierCurveAlgorithm::Execute();
	return true;
}

glm::vec3 bgSplineCurveAlgorithm::GetPosition(float t)
{

	return glm::vec3();
}



////////////////////////////////////////////////////////////////////
///bgBezierPatchAlgorithm
bgBezierPatchAlgorithm::bgBezierPatchAlgorithm()
{
	uSize = vSize = 0;
}


bgBezierPatchAlgorithm::~bgBezierPatchAlgorithm()
{

}

bool bgBezierPatchAlgorithm::Initialize()
{
	pbgData->initialize();
	ControlPointList.clear();



	return true;
}

bool bgBezierPatchAlgorithm::Update(float t)
{

	return true;
}

bool bgBezierPatchAlgorithm::Execute()
{

	return true;
}

void bgBezierPatchAlgorithm::SetControlNet(_ControlPointList& controlList,int usize, int vsize)
{
	Initialize();
	uSize = usize; vSize = vsize;
	ControlPointList.insert(controlList.begin(), controlList.end());
	MakeTriagleList();
}

void bgBezierPatchAlgorithm::MakeTriagleList()
{
	bgData::_PositionList& PointList = pbgData->GetPositionList();
	bgData::_ColorList&		ColorList = pbgData->GetColorList();
	bgData::_IndexList&		IndexList = pbgData->GetIndexList();
	bgData::_TexcelList&	TexcelList = pbgData->GetTexcelList();
	
	CString message;
	int index = 0;
	for (int u = 0; u < uSize; u++)
	{
		for (int v = 0; v < vSize; v++)
		{
			glm::vec3 position = FindControlPointPosition(u, v);
			PointList.push_back(position.x);
			PointList.push_back(position.y);
			PointList.push_back(position.z);

			ColorList.push_back(1.f);
			ColorList.push_back(1.f);
			ColorList.push_back(1.f);
		
		/*	glm::vec2 texcel = glm::vec2((float)v / (float)vSize, (float)u / (float)uSize);
			TexcelList.push_back(texcel.x);
			TexcelList.push_back(texcel.y);

			message.Format("index%d:%f, %f \n", index, texcel.x, texcel.y);
			OutputDebugString(message);*/
			//message.Format("index%d:%f, %f, %f \n", index, position.x, position.y , position.z);
			//OutputDebugString(message);
			index++;
		}
	}

	index = 0;
	int numPositionCount = PointList.size();

	const float uStep = 1.0f / (float)(uSize-1);
	const float vStep = 1.0f / (float)(vSize-1);
	for (int v = 0; v < vSize ; v++)
	{
		for (int u = 0; u < uSize; u++)
		{
			glm::vec2 texcel = glm::vec2((float)u*uStep, (float)v*vStep);
			TexcelList.push_back(texcel.x);
			TexcelList.push_back(texcel.y);

			//CString message;
			//message.Format("index%d:%f, %f \n", index, texcel.x, texcel.y);
			//OutputDebugString(message);

			index++;
		}
	}

	
	for (int u = 0; u < uSize - 1; u++)
	{
		for (int v = 0; v < vSize - 1; v++)
		{
			int v0 = u*vSize + v;
			int v1 = (u + 1)*vSize + (v + 1);
			int v2 = (u + 1)*vSize + v;
			int v3 = v0;
			int v4 = v0 + 1;
			int v5 = v1;

			IndexList.push_back(v0); IndexList.push_back(v1); IndexList.push_back(v2);
			IndexList.push_back(v3); IndexList.push_back(v4); IndexList.push_back(v5);
	
			


	//		message.Format("index : %d, %d, %d, %d, %d, %d \n", v0, v1, v2, v3, v4, v5);
	//		OutputDebugString(message);
		}
	}

}

ControlPointIndex* bgBezierPatchAlgorithm::FindControlPointIndex(int u, int v)
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

glm::vec3	bgBezierPatchAlgorithm::FindControlPointPosition(int u, int v)
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
