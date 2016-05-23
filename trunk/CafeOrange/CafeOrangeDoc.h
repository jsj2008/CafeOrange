#pragma once
//#include "Renderer.h"
#include "Scene.h"
#include <string>
#include <vector>
using namespace std;


class cScene;

class CCafeOrangeDoc : public CDocument
{
	DECLARE_DYNCREATE(CCafeOrangeDoc)
	DECLARE_MESSAGE_MAP()

public: 
	class cSceneItem{
	public:		
		std::string		Name;
		eSceneType	SceneType;
		cScene*			pScene;
		cSceneItem(eSceneType type)
		{
			pScene = 0;
			SceneType = type;
		}
	};

	typedef std::vector<cSceneItem*> _SceneList;
	_SceneList SceneList;

	cScene* GetScene(eSceneType	SceneType);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


	CCafeOrangeDoc();
	virtual ~CCafeOrangeDoc();	

protected:
	void SceneInitialize();

public:
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

};
