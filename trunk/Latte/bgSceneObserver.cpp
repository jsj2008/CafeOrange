#include "stdafx.h"
#include "bgSceneObserver.h"


bgSceneObserver* bgSceneObserver::pSceneObserver = 0;

bgSceneObserver::bgSceneObserver()
{	
	pWnd = 0;
	pScene = 0;
}

bgSceneObserver::~bgSceneObserver()
{

}

void bgSceneObserver::setActiveView(CWnd*	_pWnd)
{
	pWnd = _pWnd;
	messageWnd.pWnd = pWnd;
}

void bgSceneObserver::sendInformation(eInformationTarget uiTarget, bgProperty& property)
{	
	messageWnd.SendInformation(uiTarget,property);
}

void bgSceneObserver::sendInformation(eInformationTarget uiTarget, bgActor& actor)
{
	messageWnd.SendInformation(uiTarget, actor);
}

void bgSceneObserver::getInformation(cScene* pScene, bgProperty& property)
{
	if (pScene == 0)
		return;

	property.messageMap["fill"] = std::to_string((int)pScene->fillMode);
	property.messageMap["shading"] = std::to_string((int)pScene->shadingModel);

}

