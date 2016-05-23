#pragma once
//#include "glm.hpp"
#include "GlobalEnum.h"
#include "bgMaterial.h"
#include <vector>
using namespace std;


// Color Composition
#define ARGB(a, r, g, b)	((DWORD)( (((DWORD)(BYTE)(a)) << 24) | (((DWORD)(BYTE)(r)) << 16) | (((WORD)(BYTE)(g)) << 8) | ((BYTE)(b)) ))
#define ARGBAlpha(c)		((BYTE)(((DWORD)(c)) >> 24))
#define ARGBRed(c)		((BYTE)(((DWORD)(c)) >> 16))
#define ARGBGreen(c)		((BYTE)(((WORD)(c)) >> 8))
#define ARGBBlue(c)		((BYTE)(c))

#define RGBA(a, b, g, r)	((DWORD)( (((DWORD)(BYTE)(a)) << 24) | (((DWORD)(BYTE)(b)) << 16) | (((WORD)(BYTE)(g)) << 8) | ((BYTE)(r)) ))
#define RGBAAlpha(c)		((BYTE)(((DWORD)(c)) >> 24))
#define RGBARed(c)		((BYTE)(c))
#define RGBAGreen(c)		((BYTE)(((WORD)(c)) >> 8))
#define RGBABlue(c)		((BYTE)(((DWORD)(c)) >> 16))


class cPixelRGBA{ // 0xAABBGGRR
public:
	union{
		DWORD Color;
		struct{
			BYTE R, G, B, A;
		};
	};
	cPixelRGBA(){}
	cPixelRGBA(DWORD ARGBPixel){
		A = RGBAAlpha(ARGBPixel);
		B = RGBARed(ARGBPixel);
		G = RGBAGreen(ARGBPixel);
		R = RGBABlue(ARGBPixel);
	}
};

class cCVertex
{
public:
	glm::vec3	Position;
	cPixelRGBA	Diffuse;

//	static DWORD GetFVF() { return (D3DFVF_XYZ | D3DFVF_DIFFUSE); }
	cCVertex()
	{

	}

	cCVertex(glm::vec3 _Position, DWORD _DiffuseBGRA = 0xFFFFFFFF){
		Position = _Position;
		Diffuse = _DiffuseBGRA;
	}
	void Set(glm::vec3 _Position, DWORD _DiffuseBGRA = 0xFFFFFFFF){
		Position = _Position;
		Diffuse = _DiffuseBGRA;
	}
	cCVertex& operator=(const cCVertex& Other){
		Position = Other.Position;
		Diffuse	 = Other.Diffuse;
		return *this;
	}
};

typedef std::vector<cCVertex> cCVertices;

class cVertex
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 Texel;
};

class cShaderEffect;
class cRenderer;
class cModel
{
public:
	typedef  unsigned int  GLMode;
	GLMode DrawMode;

	typedef std::vector<cVertex> _VertexList;	
	_VertexList VertexList;

	cShaderEffect*	pShaderEffect;
	bgMaterial Material;
	virtual void Render(cRenderer& Renderer);

	void GetVertexMinMax(glm::vec3& Min,glm::vec3& Max);

	cModel();
	virtual ~cModel();
};

