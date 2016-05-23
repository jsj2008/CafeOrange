#pragma once

#include "bgMapper.h"
#include "bgShader.h"
#include "bgShaderProperty.h"

class cRenderer;
class bgMaterial;
class bgShaderFactory;
class bgMovie;

class bgActor : public bgObject
{
public:
	bgActor(bool _pickable=true);
	virtual ~bgActor();

	bool setInput( bgMapper* _pMapper);
	bgMapper* getInput() { return pMapper;  }
	const bgMapper* getInput() const { return pMapper; }
	///////////////////////////////////////////////////////////////////////////
	//shader set/get
	bool setShader( bgShader* _pShader);
	bgShader* getShader() { return pShader;  }
	const bgShader* getShader() const { return pShader; }
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//material set/get
	bool setMaterial(bgMaterial* _pMaterial);
	bgMaterial* getMaterial(){ return pMaterial; }
	const bgMaterial* getMaterial() const { return pMaterial; }
	///////////////////////////////////////////////////////////////////////////
	bool IsPickable() { return pickable;  }
	bool setPickable(bool _pickable) { pickable = _pickable; }
	///////////////////////////////////////////////////////////////////////////
	void		setSelected(bool _selected) { selected = _selected;  }
	bool		getSelected() { return selected; }
	void		setSelectionMapper(bgMapper* pMapper);
	///////////////////////////////////////////////////////////////////////////
	virtual bool initialize();
	virtual bool update();
	virtual bool build(cRenderer& Renderer);

	void		setTranslation(glm::vec3& translate);
	void		setScale(glm::vec3& scale);
	void		setPosition(glm::vec3& position);
	void		intializeTransformed();
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 추후 DrawGeometryStrategy클래스로 작성.
	GLuint getVAO() { return VAO; }
	GLuint getVBO() { return VBO;  }
	GLuint  getIBO() { return IBO;  }

	eDrawElementType getDrawType() { return drawType;  }
	int	getPrimitiveCount(){ return primitvieCount;  }
	int	getIndexCount(){ return indexCount;  }
	glm::mat4 GetTransformMat4() { return transformMat4;  }
	void		SetTransformMat(glm::mat4& _transformMat) { transformMat4 = _transformMat; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	bgMapper*	pMapper;
	bgShader*		pShader;

	bgShaderProperty		propertyMap;
	glm::mat4	 transformMat4;
	bool		pickable;
	bool		selected;

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 추후 DrawGeometryStrategy클래스로 작성.
	GLuint VAO;
	GLuint VBO;
	GLuint  IBO;
	eDrawElementType	drawType;
	int	primitvieCount;
	int    indexCount;
	bgMaterial* pMaterial;

	void setPrimitiveCount(int positionCount);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
private:
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgCanvasActor
class bgCanvasActor : public bgActor
{
public:
	eCanvasSize			canvasSize;
	eCanvasEffect			canvasEffect;
	eCanvasMorph		canvasMorph;

	virtual bool initialize();
	virtual bool build(cRenderer& Renderer);

	void setMovie(bgMovie* _pMovie);
	void resize(eCanvasSize size);

	bgCanvasActor(bool _pickable = true);
	virtual ~bgCanvasActor();

protected:
	bgMovie* pMovie;


};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgRayActor
class bgRayActor : public bgActor
{
public:
	bgRayActor(bool _pickable = true);
	virtual ~bgRayActor();

	virtual bool initialize();
	bool initializeRay(bgShaderFactory* pShaderFactory, glm::vec3& origin, glm::vec3& direction);

	glm::vec3 GetOrigin();
	glm::vec3  GetDirection();

	void  SetOrigin(glm::vec3& origin);
	void  SetDirection(glm::vec3& direction);

};
