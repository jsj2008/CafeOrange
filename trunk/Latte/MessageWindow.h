#pragma once
#include "afxwin.h"
#include "bgProperty.h"


class MessageWindow : public CWnd
{
	DECLARE_MESSAGE_MAP()

public:	
	CWnd*		pWnd;

	MessageWindow();
	virtual ~MessageWindow();

	void SendInformation(eInformationTarget uiTarget, bgProperty& property);
	void SendInformation(eInformationTarget uiTarget, bgActor& actor);


};

