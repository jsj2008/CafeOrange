#pragma once
#include "DefineEvent.h"
#include "Scene.h"
#include "bgProperty.h"
#include "MessageWindow.h"

class bgSceneObserver
{
public:
	
	static bgSceneObserver* GetInstancePtr(){
		if (pSceneObserver == 0)
			pSceneObserver = new bgSceneObserver();
		return pSceneObserver;
	}
	
	static bgSceneObserver& GetInstance(){
		if (pSceneObserver == 0)
			pSceneObserver = new bgSceneObserver();
		return *pSceneObserver;
	}

	~bgSceneObserver();

	void setActiveScene(cScene* _pScene) { pScene = _pScene;  }
	cScene* getActiveScene(){ return pScene;  }
	
	void setActiveView(CWnd*	_pWnd);

	void sendInformation(eInformationTarget uiTarget, bgProperty& property);
	void	sendInformation(eInformationTarget uiTarget, bgActor& actor);

	void getInformation(cScene* pScene, bgProperty& property);

protected:
	static bgSceneObserver* pSceneObserver;
	bgSceneObserver();

private:
	cScene* pScene;	
	CWnd*	pWnd;
	MessageWindow	messageWnd;
};

