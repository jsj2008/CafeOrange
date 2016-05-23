#pragma once

#include "VideoFrameDlg.h"
#include "PhotoFrameDlg.h"
#include <map>
#include <string>
using namespace std;



/////////////////////////////////////////////////////////////////////////////
// COutputList
class COutputList : public CListBox
{
	DECLARE_MESSAGE_MAP()

public:
	COutputList();
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();	
};
/////////////////////////////////////////////////////////////////////////////
// CTextureList
class CTextureList : public CListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CTextureList();
	virtual ~CTextureList();

	bool Initialize();

protected:
	CImageList imgList;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	int selectedItem; 
};

/////////////////////////////////////////////////////////////////////////////
// COutputWnd
class COutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(COutputWnd)
	DECLARE_MESSAGE_MAP()
public:
	CMFCTabCtrl	m_wndTabs;
	COutputList	OutputList;
	CTextureList	TextureList;

	CPhotoFrameDlg  PhtoFrameDlg;
	CVideoFrameDlg	VideoFrameDlg;

	COutputList			m_wndOutputFind;

	void UpdateFonts();
	void AddMessage(std::map<string, string>& messageMap);

	COutputWnd();
protected:
	void FillBuildWindow(std::string& message);

	void AdjustHorzScroll(CListBox& wndListBox);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	
};

