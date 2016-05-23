#include "stdafx.h"
#include "bgShapeInfo.h"

///////////////////////////////////////////////////////////////////////////////////////////
//bgShapeInfo
bgShapeInfo::bgShapeInfo()
{
	
}

bgShapeInfo::~bgShapeInfo()
{

}

///////////////////////////////////////////////////////////////////////////////////////////
//bgSphereShapeInfo
bgSphereShapeInfo::bgSphereShapeInfo()
{
	type = SHAPE_3D_SPHERE;
	Radius = 1.5f;
	Rings = Sectors = 20;
}

bgSphereShapeInfo::~bgSphereShapeInfo()
{

}