#pragma once

#include <vector>
using namespace std;

#include "glm.hpp"
#include "bgShapeInfo.h"
#include "bgObject.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////bgRay
class bgRay
{
public:
	bgRay(glm::vec3&  _origin, glm::vec3& _eye);
	~bgRay();

	glm::vec3 origin;
	glm::vec3 direction;

	bool CalculateRayOnTriangle(vector<glm::vec3>& VectorList, glm::mat4& transformedMat, glm::vec3& IntersectedPoint);
	bool CalculateRayOnSphere(const bgSphereShapeInfo* pInfo, glm::mat4& transformedMat, glm::vec3& IntersectedPoint);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////bgData
class bgData : public bgObject
{
public:
	bgData();
	virtual ~bgData();
		
	typedef vector<float> _PositionList;
	typedef vector<float> _NormalList;
	typedef vector<float> _ColorList;
	typedef vector<float> _TexcelList;
	typedef vector<GLushort>	_IndexList;

	const int NumPosition() const{ return PositionList.size(); }
	const int NumNormal()const  { return NormalList.size(); }
	const int NumColor() const { return ColorList.size(); }
	const int NumTexcel() const{ return TexcelList.size(); }
	const int NumIndex()  const{ return IndexList.size(); }

	const int NumTotalVertexAttrib() const { return (NumPosition() + NumNormal() + NumColor() + NumTexcel()); }

	const GLushort*		GetIndexListPointer() const { return IndexList.data(); }
	const float*			GetPositionListPointer()	 const { return PositionList.data(); }
	const float*			GetNormalListPointer()		 const { return NormalList.data(); }
	const float*			GetColorListPointer()			 const { return ColorList.data(); }
	const float*			GetTexcelListPointer()		const  { return TexcelList.data(); }
	
	_PositionList&	GetPositionList() { return PositionList;  }
	_NormalList&		GetNormalList() { return NormalList;  }
	_ColorList&			GetColorList() { return ColorList;  }
	_TexcelList&		GetTexcelList() { return TexcelList; }
	_IndexList&			GetIndexList() { return IndexList; }

	bgShapeInfo* GetShapeInfo() { return pShapeInfo; }
	const bgShapeInfo* GetShapeInfo() const {return pShapeInfo;}

	eDrawElementType	GetDrawElementType() { return drawElementType;  }
	eSourceType				GetSourceType() { return sourceType;  }
	eVertexType				GetVertexType() { return vertexType;  }

	virtual bool initialize();
	
	bool generate(eSourceType type,bgShapeInfo* pShapeInfo = 0);
	bool generate(eCanvasSize type);

	void DeepCopy(bgData& _bgData);
	void GenerateFromData(eSourceType destType, bgData* selectedData); // eSourceType srcType);

	//Picking
	bool generateRay(glm::vec3& origin, glm::vec3& direction);	

	eVertexType	vertexType;
	eSourceType	sourceType;
	eDrawElementType	drawElementType;

protected:
	_PositionList	PositionList;
	_NormalList	NormalList;
	_ColorList		ColorList;
	_TexcelList		TexcelList;

	_IndexList	IndexList;

	bgShapeInfo* pShapeInfo;


	void	SetVertexType();

	bool generate2D(eSourceType type);
	bool generate3D(eSourceType type, bgShapeInfo* pShapeInfo);

	//Movie Rectangle
	bool generateCanvasRectangle(float left, float bottom, float right, float top);

	//2D Generation
	bool generateTriangle(bool hasNormal, bool hasTexcel);
	bool generateRentangle(bool hasNormal, bool hasTexcel);
	bool generateLine();

	//3D Generation
	bool generateSphere(float radius, unsigned int rings, unsigned int sectors,bool hasNormal, bool basTexcel);

	void SetPositionData(float positions[], int size);
	void SetTexcelData(const float texcels[], int size);
	void SetNormalData(const float normals[], int size);
	void SetColorData(const float colors[], int size);
	void SetIndexData(const GLushort indices[], int size);
	

private:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgDataSet
class bgDataSet
{
public:
	bgDataSet();
	virtual ~bgDataSet();

	typedef vector<bgData*> _bgDataList;
	_bgDataList bgDataList;

protected:

private:
};


