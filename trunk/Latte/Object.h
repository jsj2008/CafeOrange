#pragma once

#include "Model.h"
#include <vector>
#include <string>
using namespace std;

class cBoundingBox
{
public:
	glm::vec3 Min;
	glm::vec3 Max;
};

class cRenderer;
/////////////////////////////////////////////////////////////////////////////////////////////
////cObject
class cObject
{
public:
	int	 PickingID;
	std::string Name;
	typedef std::vector<cObject*>  _Childern;

	
	void Release();
	void Destory();
	void AddChild(cObject* pNode);
	
	virtual void Update();
	virtual void Render(cRenderer& Renderer) { }

	cObject();
	virtual ~cObject();

protected:
	_Childern Children;

private:

};

/////////////////////////////////////////////////////////////////////////////////////////////
////cGeometryObject
class cGeometryObject : public cObject
{
public:
	cBoundingBox BoundingBox;

	glm::vec3 Translation;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 TransformMat;

	typedef std::vector<cModel*> _ModelList;
	_ModelList ModelList;

	bgMaterial* GetMaterial(int index);
	const bgMaterial* GetMaterial(int index) const ;

	cModel* GetModel(int index);
	const cModel* GetModel(int index) const ;

	void SetBoundingBox();
	void Clear();

	virtual void Update();
	virtual void Render(cRenderer& Renderer);

	cGeometryObject();
	virtual ~cGeometryObject();

protected:
};



/////////////////////////////////////////////////////////////////////////////////////////////
//[펌] glut.h의 exit 재정의 에러(stdlib.h) / http://kindnap.tistory.com/81



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////