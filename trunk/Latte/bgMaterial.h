#pragma once
#include "glm.hpp"

#include <GL/glut.h>
#include "bgTexture.h"
using namespace std;

class cRenderer;

class bgMaterial
{
public:
	glm::vec4		Diffuse;
	glm::vec4		Specular;
	glm::vec4		Ambient;
	glm::vec4		Emissive;
	float			Power;

	typedef std::vector<bgTexture*> _TextureList;
	_TextureList  TextureList;

	bool		LoadTexture(const std::string& FilePath);
	bool		DeleteTexture(const std::string& FilePath);
	bool		ChangeTexture(int order,bgTexture* pTexture);
	
	void		Set(cRenderer* pRenderer);
	void		Reset(cRenderer* pRenderer);
	void		UnSet(cRenderer* pRenderer);

	bgMaterial();
	virtual ~bgMaterial();

protected:
	void CreateTexture(bgTexture& Texture);
};

