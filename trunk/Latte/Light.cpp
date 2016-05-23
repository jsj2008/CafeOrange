#include "StdAfx.h"
#include "Renderer.h"
#include "Light.h"

GLenum	GetLightIndex(int LightID)
{
	GLenum GLlight = 0;
	switch(LightID)
	{
	case 0: GLlight = GL_LIGHT0;	break;
	case 1: GLlight = GL_LIGHT1;	break;
	case 2: GLlight = GL_LIGHT2;	break;
	case 3: GLlight = GL_LIGHT3;	break;
	case 4: GLlight = GL_LIGHT4;	break;
	case 5: GLlight = GL_LIGHT5;	break;
	case 6: GLlight = GL_LIGHT6;	break;
	case 7: GLlight = GL_LIGHT7;	break;
	}

	return GLlight;
}


//////////////////////////////////////////////////////////////////////
//cLight
cLight::cLight(int _LightID)
{
	Enabled = true;
	LightID  = _LightID;
	//Material.Diffuse = glm::vec4(0.7f,0.7f,0.7f,1.0f);
	Material.Diffuse = glm::vec4(0.9f,0.1f,0.1f,1.0f);
	Material.Ambient = glm::vec4(0.2f,0.2f,0.2f,1.0f);
	Material.Specular = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f); 
}


cLight::~cLight()
{

}

void	cLight::Enable(bool _Enable)
{
	Enabled = _Enable;
}

void cLight::SetPosition(glm::vec3 _Position)
{
	Position = _Position;
}


void cLight::EnableLight(bool Enable)
{
	switch(LightID)
	{
	case 0: Enable == true? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);	break;
	case 1: Enable == true? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);	break;
	case 2: Enable == true? glEnable(GL_LIGHT2) : glDisable(GL_LIGHT2);	break;
	case 3: Enable == true? glEnable(GL_LIGHT3) : glDisable(GL_LIGHT3);	break;
	case 4: Enable == true? glEnable(GL_LIGHT4) : glDisable(GL_LIGHT4);	break;
	case 5: Enable == true? glEnable(GL_LIGHT5) : glDisable(GL_LIGHT5);	break;
	case 6: Enable == true? glEnable(GL_LIGHT6) : glDisable(GL_LIGHT6);	break;
	case 7: Enable == true? glEnable(GL_LIGHT7) : glDisable(GL_LIGHT7);	break;
	}
}
//////////////////////////////////////////////////////////////////////
//cDirectionalLight


cDirectionalLight::cDirectionalLight(int _LightID) : cLight(_LightID)
{
	Type = LIGHT_TYPE_DIRECTION;
}

cDirectionalLight::~cDirectionalLight()
{

}

void cDirectionalLight::Build()
{
	ASSERT(LightID >= 0 && LightID <= 8 );

	GLenum GLlight = GetLightIndex(LightID);

	glLightfv(GLlight, GL_AMBIENT,  &(Material.Ambient.r));
 	glLightfv(GLlight, GL_DIFFUSE,    &(Material.Diffuse.r));
	glLightfv(GLlight, GL_SPECULAR, &(Material.Specular.r));
	glLightfv(GLlight, GL_POSITION,  &(Position.x));

	EnableLight(Enabled);
}