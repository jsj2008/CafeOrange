#include "StdAfx.h"
#include "bgMaterial.h"
#include "Object.h"

/////////////////////////////////////////////////////////////////////////////////////////
///cObject

cObject::cObject()
{
	PickingID = 0;
}

cObject::~cObject()
{
	//Release();
}

void cObject::Destory()
{
	_Childern::iterator iChild = Children.begin();
	for( ; iChild != Children.end() ; iChild++){
		cObject *pNode = *iChild;
		//pNode->Release();
		delete pNode;
	}

	Children.clear();
}

void cObject::Release()
{
	delete this;
}

void cObject::Update()
{
	_Childern::iterator iChild = Children.begin();
	for( ; iChild != Children.end() ; iChild++){
		cObject *pNode = *iChild;
		pNode->Update();
	}
}


void cObject::AddChild(cObject* pNode)
{
	if(pNode == 0 ) return;

	Children.push_back(pNode);

}


/////////////////////////////////////////////////////////////////////////////////////////
///cGeometryNode
cGeometryObject::cGeometryObject()
{
	Translation = glm::vec3(0, 0 ,0);
	Rotation  = glm::vec3(0, 0 ,0);
	Scale = glm::vec3(1, 1 ,1);
}

cGeometryObject::~cGeometryObject()
{
	Clear();
}

void cGeometryObject::Clear()
{
	_ModelList::iterator iModel = ModelList.begin();
	for( ; iModel != ModelList.end(); iModel++){
		cModel* pModel = *iModel;
		delete pModel;
	}
	ModelList.clear();
}

void cGeometryObject::Update()
{
	glPushMatrix();

	glLoadMatrixf(&TransformMat[0][0]);
	
	cObject::Update();

	glPopMatrix();
}

void cGeometryObject::SetBoundingBox()
{
	 glm::vec3 Min, Max;
	_ModelList::iterator iModel = ModelList.begin();
	for( ; iModel != ModelList.end(); iModel++){
		cModel* pModel = *iModel;
		pModel->GetVertexMinMax(Min, Max);

		if(BoundingBox.Min.x > Min.x)
			BoundingBox.Min.x = Min.x;
		if(BoundingBox.Min.y > Min.y)
			BoundingBox.Min.y = Min.y;
		if(BoundingBox.Min.z > Min.z)
			BoundingBox.Min.z= Min.z;

		if(BoundingBox.Max.x < Max.x)
			BoundingBox.Max.x = Max.x;
		if(BoundingBox.Max.y < Max.y)
			BoundingBox.Max.y = Max.y;
		if(BoundingBox.Max.z < Max.z)
			BoundingBox.Max.z = Max.z;	
	}
}

void cGeometryObject::Render(cRenderer& Renderer)
{
	glPushMatrix();
	glTranslatef(Translation.x,Translation.y,Translation.z);
	glRotatef(Rotation.x, 1, 0, 0);
	glRotatef(Rotation.y, 0, 1, 0);
	glRotatef(Rotation.z, 0, 0, 1);
	glScalef(Scale.x, Scale.y, Scale.z);

		_ModelList::iterator iModel = ModelList.begin();
		for( ; iModel != ModelList.end(); iModel++){
			cModel* pModel = *iModel;
		
			pModel->Render(Renderer);
		}
	glPopMatrix();
}

bgMaterial* cGeometryObject::GetMaterial(int index)
{
	if(ModelList.size() <= (size_t)0)
		return 0;

	_ModelList::iterator iModel = ModelList.begin();
	std::advance(iModel, index);

	if(iModel == ModelList.end())
		return 0;
		
	return &(*iModel)->Material;
}

const bgMaterial* cGeometryObject::GetMaterial(int index) const
{
	if(ModelList.size() <= (size_t)0)
		return 0;

	_ModelList::const_iterator iModel = ModelList.begin();
	std::advance(iModel, index);

	if(iModel == ModelList.end())
		return 0;
		
	return &(*iModel)->Material;
}

cModel* cGeometryObject::GetModel(int index)
{
	if(ModelList.size() <= (size_t)0)
		return 0;

	_ModelList::iterator iModel = ModelList.begin();
	std::advance(iModel, index);

	if(iModel == ModelList.end())
		return 0;

	return *iModel;
}

const cModel* cGeometryObject::GetModel(int index) const
{
	if(ModelList.size() <= (size_t)0)
		return 0;

	
	_ModelList::const_iterator iModel = ModelList.begin();
	std::advance(iModel, index);

	if(iModel == ModelList.end())
		return 0;

	return *iModel;
}

