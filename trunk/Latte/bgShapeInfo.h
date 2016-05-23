#pragma once
#include "GlobalEnum.h"

///////////////////////////////////////////////////////////////////////////////////////////
//bgShapeInfo
class bgShapeInfo
{
public:
	bgShapeInfo();
	virtual ~bgShapeInfo();

	eShapeType	GetType(){ return type; }

protected:
	eShapeType type;

};
///////////////////////////////////////////////////////////////////////////////////////////
//bgSphereShapeInfo
class bgSphereShapeInfo : public bgShapeInfo
{
public:
	bgSphereShapeInfo();
	virtual ~bgSphereShapeInfo();

	float Radius;
	int	Rings;
	int	Sectors;

protected:
};