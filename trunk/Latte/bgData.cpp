#include "stdafx.h"
#include "bgData.h"
#include "gtx/intersect.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

bgData::bgData()
{
	pShapeInfo = 0;
}

bgData::~bgData()
{

}

bool bgData::initialize()
{
	PositionList.clear();
	NormalList.clear();
	ColorList.clear();
	TexcelList.clear();

	IndexList.clear();

	pShapeInfo = 0;

	return true;
}

void bgData::DeepCopy(bgData& _bgData)
{
	initialize();

	PositionList = _bgData.GetPositionList();
	NormalList = _bgData.GetNormalList();
	ColorList = _bgData.GetColorList();
	TexcelList = _bgData.GetTexcelList();

}

void bgData::GenerateFromData(eSourceType destType, bgData* selectedData) // eSourceType srcType)
{	
	if (destType == SOURCE_2D_LINE_PC)
	{
		sourceType = destType;
		eSourceType srcType = selectedData->GetSourceType();

		TexcelList.clear();
		drawElementType = ELEMENT_TYPE_LINES;

		if (srcType == SOURCE_2D_TRIANGLE_PC || srcType == SOURCE_2D_TRIANGLE_PTC)
		{
			GLushort indices[] ={
				0, 1,1,2,2,0
			};
			SetIndexData(indices, 3*2);
		}
		else if (srcType == SOURCE_2D_RECTANGLE_PC || srcType == SOURCE_2D_RECTANGLE_PTC || srcType == SOURCE_2D_MOVIE_RECTANGLE_PTC)
		{
			GLushort indices[] = {
				0, 1, 1, 3, 3, 2,2,0
			};
			SetIndexData(indices, 4 * 2);
		}
	}
}

bool bgData::generate(eCanvasSize type)
{
	float MeshWidth = 1.0f;
	float MeshHeight = 1.0f;

	switch (type)
	{
	case CANVAS_TYPE_FULL: break;
	case CANVAS_TYPE_0_9x:		MeshWidth = MeshHeight = 0.9f; break;
	case CANVAS_TYPE_0_8x:		MeshWidth = MeshHeight = 0.8f; break;
	case CANVAS_TYPE_0_7x:		MeshWidth = MeshHeight = 0.7f; break;
	case CANVAS_TYPE_0_6x:		MeshWidth = MeshHeight = 0.6f; break;
	case CANVAS_TYPE_0_5x:		MeshWidth = MeshHeight = 0.5f; break;
	case CANVAS_TYPE_0_4x:		MeshWidth = MeshHeight = 0.4f; break;
	case CANVAS_TYPE_0_3x:		MeshWidth = MeshHeight = 0.3f; break;
	default: break;
	}

	sourceType = SOURCE_2D_MOVIE_RECTANGLE_PTC;
	drawElementType = ELEMENT_TYPE_TRIANGLES;
	
	generateCanvasRectangle(-MeshWidth, -MeshHeight, MeshWidth, MeshHeight);

	return true;
}

bool bgData::generateCanvasRectangle(float left,float bottom, float right, float top)
{
	PositionList.clear();
	NormalList.clear();
	ColorList.clear();
	IndexList.clear();

	int meshWidth	= (int)((fabs(right) + fabs(left)) * 10.f) + 1;
	int meshHeight = (int)((fabs(top) + fabs(bottom)) * 10.f) + 1;

	PositionList.resize(meshWidth* meshHeight * 3);
//	NormalList.resize(meshWidth* meshHeight * 3);
	TexcelList.resize(meshWidth* meshHeight * 3);
	ColorList.resize(meshWidth* meshHeight * 3);

	_PositionList::iterator iPos = PositionList.begin();
	_ColorList::iterator		 iColor = ColorList.begin();
//	_NormalList::iterator  iNormal = NormalList.begin();
	_TexcelList::iterator	 iTexcel = TexcelList.begin();

	float startXPos = left;
	float startYPos = bottom;

	for (int row = 0; row < meshWidth; row++)
	{
		for (int col = 0; col < meshHeight; col++)
		{
			*iPos++ = startXPos + 0.1f* (float)col;
			*iPos++ = startYPos + 0.1f* (float)row;
			*iPos++ =   0.f;

			*iColor++ = 1.0f;
			*iColor++ = 1.0f;
			*iColor++ = 1.0f;

	/*		*iNormal++ = 0.f;
			*iNormal++ = 0.f;
			*iNormal++ = 1.f;*/

			*iTexcel++ = (float)col / (meshWidth-1);
			*iTexcel++ = (float)row / (meshHeight-1);
		}
	}

	IndexList.resize((meshWidth - 1) * (meshHeight - 1) * 2 * 3);
	_IndexList::iterator iIndex = IndexList.begin();
	for (int row = 0; row < meshWidth - 1; row++)
	{
		for (int col = 0; col < meshHeight - 1; col++)
		{
			*iIndex++ = meshHeight * (row + 1) + col;
			*iIndex++ = meshHeight * row +col ;
			*iIndex++ = meshHeight * (row + 1) + col+ 1;

			*iIndex++ = meshHeight * (row + 1) + col+1;
			*iIndex++ = meshHeight * row + col;
			*iIndex++ = meshHeight * row + col + 1;
		}
	}



	return true;
}

bool bgData::generate(eSourceType type, bgShapeInfo* pShapeInfo)
{
	sourceType = type;

	if (type > SOURCE_2D_BEGIN && type < SOURCE_2D_END)
		generate2D(type);
	else if (type > SOURCE_3D_BEGIN && type < SOURCE_3D_END)
		generate3D(type, pShapeInfo);
	else{
		ASSERT(0);
		return false;
	}

	SetVertexType();
	return true;
}
bool bgData::generateRay(glm::vec3& origin, glm::vec3& direction)
{
	PositionList.clear();
	ColorList.clear();
	IndexList.clear();

	float colors[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	GLushort indices[] =
	{
		0, 1,
	};

	const float deltaT = -10.0f;// 10000.0f;
	PositionList.resize(6);
	_PositionList::iterator iPos = PositionList.begin();
	*iPos++ = origin.x;
	*iPos++ = origin.y;
	*iPos++ = origin.z;

	*iPos++ = origin.x +deltaT*direction.x;
	*iPos++ = origin.y + deltaT*direction.y;
	*iPos++ = origin.z + deltaT*direction.z;
	
	SetColorData(colors, 6);
	SetIndexData(indices, 2);

	drawElementType = ELEMENT_TYPE_LINES;

	return true;
}

bool bgData::generate2D(eSourceType type)
{
	switch (type)
	{
	case SOURCE_2D_TRIANGLE_PC:	   
		generateTriangle(false, false);		drawElementType = ELEMENT_TYPE_TRIANGLES;  break;
	case SOURCE_2D_RECTANGLE_PC:  
		generateRentangle(false, false);	drawElementType = ELEMENT_TYPE_TRIANGLE_STRIP;  break;
	case SOURCE_2D_CIRCLE_PC:				
		break;
	case SOURCE_2D_LINE_PC:				
		generateLine(); drawElementType = ELEMENT_TYPE_LINES; break;
	case SOURCE_2D_TRIANGLE_PTC:		 
		generateTriangle(false, true);		drawElementType = ELEMENT_TYPE_TRIANGLES;  break;
	case SOURCE_2D_RECTANGLE_PTC:	
	case SOURCE_2D_MOVIE_RECTANGLE_PTC:
		generateRentangle(false, true);	drawElementType = ELEMENT_TYPE_TRIANGLE_STRIP;  break;

	}

	return true;
}
bool bgData::generateTriangle(bool hasNormal, bool hasTexcel)
{
	float positions[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	float colors[] =
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLushort indices[] =
	{
		0, 1, 2
	};

	SetPositionData(positions, 9);
	SetColorData(colors, 9);
	SetIndexData(indices, 3);

	if (hasTexcel)
	{
		float texcels[] = {
			0.f,	0.f,
			1.f,	0.f,
			0.5f, 1.f,
		};
		SetTexcelData(texcels, sizeof(texcels) / sizeof(texcels[0]));
	}

	return true;
}
bool bgData::generateRentangle(bool hasNormal, bool hasTexcel)
{
	float positions[] =
	{
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

	};
	float colors[] =
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLushort indices[] =
	{
		0, 1, 2, 2,1,3		
	};


	SetPositionData(positions, sizeof(positions)/sizeof(positions[0]));
	SetColorData(colors, sizeof(colors) / sizeof(colors[0]));
	SetIndexData(indices, sizeof(indices) / sizeof(indices[0]));

	if (hasTexcel)
	{
		float texcels[] = {
			0.f, 1.f,
			0.f, 0.f,			
			1.f, 1.f,
			1.f, 0.f,
		};
		SetTexcelData(texcels, sizeof(texcels) / sizeof(texcels[0]));

	}



	return true;
}
bool bgData::generateLine()
{

	float positions[] =
	{
		0.0f, 0.0f,  0.0f,
		0.0f, 5.0f, 5.0f,		
	};
	float colors[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	GLushort indices[] =
	{
		0, 1,
	};

	SetPositionData(positions, 6);
	SetColorData(colors, 6);
	SetIndexData(indices, 2);


	return true;
}

bool bgData::generate3D(eSourceType type, bgShapeInfo* _pShapeInfo)
{
	pShapeInfo = _pShapeInfo;
	switch (type)
	{
	case SOURCE_3D_SPHERE_PC:
		{
			if (pShapeInfo != 0){
				bgSphereShapeInfo* pInfo = dynamic_cast<bgSphereShapeInfo*>(pShapeInfo);
				generateSphere(pInfo->Radius, pInfo->Rings, pInfo->Sectors, false, false);
			}
			else
				generateSphere(1.5f, 20, 20, false, false);
			drawElementType = ELEMENT_TYPE_QUADS;
		}
		break; 
	}

	return true;
}

//http://www.whynotsometime.com/Why_Not_Sometime/3D_Shape_Coordinates.html
bool bgData::generateSphere(float radius, unsigned int rings, unsigned int sectors, bool hasNormal, bool basTexcel)
{	
	PositionList.clear();
	NormalList.clear();
	ColorList.clear();
	IndexList.clear();

	float R = 1.f / (float)(rings - 1);
	float S = 1.f / (float)(sectors - 1);

	PositionList.resize( rings* sectors * 3);
	ColorList.resize( rings* sectors * 3);

	NormalList.resize(rings* sectors * 3);
	//texcels.resize(rings* sectors * 3);

	float half_pi = glm::half_pi<float>();
	float pi = glm::pi<float>();
	_PositionList::iterator iPos = PositionList.begin();
	_ColorList::iterator iColor = ColorList.begin();
	_NormalList::iterator iNormal = NormalList.begin();

	int index = 0;
	for (unsigned int r = 0; r < rings ; r++)
	{
		for (unsigned int s = 0; s < sectors ; s++)
		{
			float y = glm::sin(-half_pi + pi * r * R);		
			float x = glm::cos(2 * pi * s * S) * glm::sin(pi * r * R);
			float z = glm::sin(2 * pi * s * S) * glm::sin(pi * r* R);

			*iPos++ = x * radius;
			*iPos++ = y * radius;
			*iPos++ = z* radius;

			*iColor++ = 1.f;
			*iColor++ = 1.f;
			*iColor++ = 1.f;		

			*iNormal++ = x;
			*iNormal++ = y;
			*iNormal++ = z;

			//CString message;
			//message.Format("Pos:%d:%f,%f,%f\n", index++, x, y, z);
			//OutputDebugString(message);			
		}
	}

	   IndexList.resize( rings* sectors * 4);
	 _IndexList::iterator iIndex = IndexList.begin();
	 for (unsigned int r = 1; r < rings; r++)
	{
		for (unsigned int s = 0; s < sectors - 1; s++)
		{
			*iIndex++ = r*sectors + s;
			*iIndex++ = r*sectors + (s + 1);
			*iIndex++ = (r + 1)*sectors +(s + 1);
			*iIndex++ = (r + 1)*sectors + s; 
		}
	}

	return true;

}

void	bgData::SetVertexType()
{
	int type = (int)VERTEX_POSITION;
	if (NormalList.size() > 0)
		type |= (int)VERTEX_NORMAL;
	if (ColorList.size() > 0)
		type |= (int)VERTEX_COLOR;
	if (TexcelList.size() > 0)
		type |= (int)VERTEX_TEX0;

	switch (type)
	{
	case (int)VERTEX_XYZ_COLOR:					
		vertexType = VERTEX_XYZ_COLOR; break;
	case (int)VERTEX_XYZ_NORMAL_DIFFUSE: 
		vertexType = VERTEX_XYZ_NORMAL_DIFFUSE; break;
	case (int)VERTEX_XYZ_TEX0: 
		vertexType = VERTEX_XYZ_TEX0; break;
	case (int)VERTEX_XYZ_NORMAL_TEX0: 
		vertexType = VERTEX_XYZ_NORMAL_TEX0; break;
	case (int)VERTEX_XYZ_COLOR_TEX0:
		vertexType = VERTEX_XYZ_COLOR_TEX0; break;
	default:
		ASSERT(0);
		break;
	}

}

void bgData::SetPositionData( float positions[] , int size)
{
	std::copy(&positions[0], &positions[size], back_inserter(PositionList));
}

void bgData::SetTexcelData(const float texcels[], int size)
{
	std::copy(&texcels[0], &texcels[size], back_inserter(TexcelList));
}

void bgData::SetNormalData(const float normals[],int size)
{
	std::copy(&normals[0], &normals[size], back_inserter(NormalList));
}

void bgData::SetColorData(const float colors[],int size)
{	
	std::copy(&colors[0], &colors[size], back_inserter(ColorList));	
}

void bgData::SetIndexData(const GLushort indices[], int size)
{	
	std::copy(&indices[0], &indices[size], back_inserter(IndexList));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgDataSet
bgDataSet::bgDataSet()
{

}

bgDataSet::~bgDataSet()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////bgRay
bgRay::bgRay(glm::vec3&  _origin, glm::vec3& _eye)
{
	origin = _origin; 
	direction =glm::normalize( _origin - _eye);
	
}

bgRay::~bgRay()
{

}


bool bgRay::CalculateRayOnTriangle(vector<glm::vec3>& VectorList, glm::mat4& transformedMat, glm::vec3& IntersectedPoint)
{
	std::vector<glm::vec3>::iterator iVec = VectorList.begin();
	for (; iVec != VectorList.end(); iVec++)
	{
		glm::vec4 tmp = glm::vec4(*iVec, 1);
		*iVec = glm::vec3(transformedMat * tmp);
	}

	bool bIntersected = glm::intersectRayTriangle(origin, direction, VectorList[0], VectorList[1], VectorList[2], IntersectedPoint);

	//CString message;
	//message.Format("origin:%f,%f,%f \n", origin.x, origin.y, origin.z);
	//OutputDebugString(message);

	//if (bIntersected)
	//	message = CString("Intersected");
	//else 
	//	message = CString("Not Intersected");
	//CString data;
	//data.Format(":%f,%f,%f \n", IntersectedPoint.x, IntersectedPoint.y, IntersectedPoint.z);
	//
	//message += data;
	//OutputDebugString(message);

	return bIntersected;
}

bool bgRay::CalculateRayOnSphere(const bgSphereShapeInfo* pInfo, glm::mat4& transformedMat, glm::vec3& IntersectedPoint)
{
	glm::vec4 center = transformedMat[3];
	glm::vec3 IntersectedNormal;
	bool bIntersected = glm::intersectRaySphere(origin, direction,
		glm::vec3(center.x, center.y, center.z),
		pInfo->Radius,
		IntersectedPoint,
		IntersectedNormal
		);

	CString message;
	message.Format("origin:%f,%f,%f \n", origin.x, origin.y, origin.z);
	OutputDebugString(message);

	if (bIntersected)
		message = CString("Intersected");
	else
		message = CString("Not Intersected");
	CString data;
	data.Format(":%f,%f,%f \n", IntersectedPoint.x, IntersectedPoint.y, IntersectedPoint.z);

	message += data;
	OutputDebugString(message);

	return bIntersected;	
}