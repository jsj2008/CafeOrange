#pragma once
#include "Object.h"

class cRenderer;
class cLight : public cObject
{
public:
	enum eType
	{
		LIGHT_TYPE_DIRECTION,
		LIGHT_TYPE_POINT,
		LIGHT_TYPE_SPOT,
	};

	eType	  Type;
	glm::vec3 Position;
	glm::vec3 Rotation;
	
	bgMaterial Material;

	void		Enable(bool _Enable);
	void		SetPosition(glm::vec3 _Position);

	virtual void Build() {};
	virtual void Render(cRenderer& Renderer){};

	cLight(int _LightID);
	virtual ~cLight();

protected:
	bool  Enabled;
	int		LightID;


	void EnableLight(bool Enable);
};


class cDirectionalLight : public cLight
{
public:
	virtual void Build();

	cDirectionalLight(int _LightID);
	virtual ~cDirectionalLight();

};