#pragma once
#include "opencv2/core.hpp"
#include <vector>
using namespace std;

class bgActor;
class bgProperty;
class bgGizmo;
class bgMovieAlgorithm;
class bgMovie;
class bgPhotoEffectAlgorithm;
class cScene;
class CPropertiesWnd;
class CVideoFrameDlg;
class CMFCPropertyGridProperty;
class CMFCPropertyGridColorProperty;

////////////////////////////////////////////////////////////////////////////////////////////
//cAction
class cAction
{
public:
	bgActor*	pObject;
	cScene*		pScene;
	bgMovie*	pMovie;
	virtual void Insert(bgActor* _pObject) { pObject = _pObject; }
	virtual void Insert(cScene* _pScene) { pScene = _pScene; }
	virtual void Insert(bgMovie* _pMovie) { pMovie = _pMovie; }
	virtual void Delete(){ };
	virtual void Do(){ };
	virtual void Update(){}

	cAction();
	virtual ~cAction();

protected:
	
};

////////////////////////////////////////////////////////////////////////////////////////////
//cObjectTreeAction
class cObjectTreeAction : public cAction
{
public:
	class TreeActorItem{
	public:
		HTREEITEM hItem;
		bgActor*	  pActor;
		TreeActorItem(){
			pActor = 0;
		}
	};

	typedef std::vector<TreeActorItem> _TreeItemList;
	_TreeItemList TreeItemList;

	CTreeCtrl* pObjectTree; 
	
	void	InitializeUI();

	void InsertVideoItem(const std::string& name);
	virtual void Insert(bgActor* pObject);

	cObjectTreeAction(CTreeCtrl* _pObjectTree);
	virtual ~cObjectTreeAction();

protected:
	HTREEITEM FindItem(const std::string& name);
	HTREEITEM FindItem(const std::string& name, HTREEITEM hRoot);

	void SortItem(HTREEITEM item);

private:
};


////////////////////////////////////////////////////////////////////////////////////////////
// cSceneTreeAction
class cSceneTreeAction : public cAction
{
public:
	CTreeCtrl* pShaderTree; 
	virtual void Insert(bgActor* _pObject);

	void	InitializeUI();

	cSceneTreeAction(CTreeCtrl* _pShaderTree);
	virtual ~cSceneTreeAction();

protected:

private:
};

////////////////////////////////////////////////////////////////////////////////////////////
// cDataPropertyAction 
class cDataPropertyAction : public cAction
{
public:
	CPropertiesWnd* pPropertyGrid;
	bgMovieAlgorithm* pMovieEffect;
	bgPhotoEffectAlgorithm* pPhotoEffect;

	virtual void Insert(bgActor* _pObject);
	virtual void Insert(cScene* _pScene);
	virtual void Insert(bgProperty* _pProperty);
	virtual void Insert(bgGizmo* _pGizmo);
	virtual void Insert(bgMovie* _pMovie) { pMovie = _pMovie; }

	void	Insert(class bgCanvasActor* _pCanvasActor);
	void InsertMovieEffect(int algorithmIndex);
	void InsertPhotoEffect(int algorithmIndex);

	 cDataPropertyAction(CPropertiesWnd* _pPropertyGrid);
	 virtual ~cDataPropertyAction();

protected:
	void DoFixedPipeProperty();
	void DoFixedPipeShadowMap();
	

private:
};


////////////////////////////////////////////////////////////////////////////////////////////
// cMaterialPropertyAction 
class cMaterialPropertyAction : public cAction
{
public:
	CMFCPropertyGridColorProperty* pDiffuseProperty;
	CMFCPropertyGridColorProperty* pSpecularProperty;
	CMFCPropertyGridColorProperty* pAmbientProperty;
	CMFCPropertyGridColorProperty* pEmissiveProperty;

	CMFCPropertyGridProperty*		pAlphaProperty;

	CPropertiesWnd* pPropertyGrid;
	virtual void Insert(bgActor* _pObject);

	void InitializeUI();
	cMaterialPropertyAction(CPropertiesWnd* _pPropertyGrid);
	virtual ~cMaterialPropertyAction();

protected:

private:
};

////////////////////////////////////////////////////////////////////////////////////////////
//cGizmo
class bgGizmo;
class cGizmoAction
{
public:
	bgGizmo *pObject;
	cScene *pScene;

	virtual void Insert(bgGizmo* _pObject) { pObject = _pObject; }
	virtual void Insert(cScene* _pScene) { pScene = _pScene; }
	virtual void Delete(){ };
	virtual void Do(){ };
	virtual void Update(){}

	cGizmoAction();
	virtual ~cGizmoAction();

protected:

};

////////////////////////////////////////////////////////////////////////////////////////////
//cGizmoTreeAction
class cGizmoTreeAction : public cGizmoAction
{
public:
	class TreeActorItem{
	public:
		HTREEITEM hItem;
		bgActor*	  pActor;
		TreeActorItem(){
			pActor = 0;
		}
	};

	typedef std::vector<TreeActorItem> _TreeItemList;
	_TreeItemList TreeItemList;

	CTreeCtrl* pObjectTree;
	virtual void Insert(bgGizmo* pObject);

	cGizmoTreeAction(CTreeCtrl* _pObjectTree);
	virtual ~cGizmoTreeAction();

protected:


private:
	HTREEITEM CanvasTreeItem;
};
