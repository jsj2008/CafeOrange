#include "stdafx.h"
#include "bgActor.h"
#include "MessageWindow.h"

BEGIN_MESSAGE_MAP(MessageWindow, CWnd)

END_MESSAGE_MAP()

MessageWindow::MessageWindow()
{
	CString wnd_class_name = ::AfxRegisterWndClass(NULL);
	BOOL created = this->CreateEx(0, wnd_class_name,
		"MessageWindow", 0
		, 0
		, 0
		, 0
		, 0
		, HWND_MESSAGE, 0);
}


MessageWindow::~MessageWindow()
{

}

void MessageWindow::SendInformation(eInformationTarget uiTarget, bgProperty& property)
{
	int target = (int)uiTarget;
	pWnd->SendMessage(WM_SCENE_INFORMATION, (WPARAM)target, reinterpret_cast<LPARAM>(&property));
}

void MessageWindow::SendInformation(eInformationTarget uiTarget, bgActor& actor)
{
	int target = (int)uiTarget;
	pWnd->SendMessage(WM_ADD_ACTOR, (WPARAM)target, reinterpret_cast<LPARAM>(&actor));
}