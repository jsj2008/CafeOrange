
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CafeOrange.h"
#endif

#include "CafeOrangeDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCafeOrangeDoc, CDocument)
BEGIN_MESSAGE_MAP(CCafeOrangeDoc, CDocument)

	
END_MESSAGE_MAP()

CCafeOrangeDoc::CCafeOrangeDoc()
{
	SceneInitialize();
}

CCafeOrangeDoc::~CCafeOrangeDoc()
{
	_SceneList::iterator iItem = SceneList.begin();
	for(  ; iItem != SceneList.end(); iItem++)
	{
		cSceneItem* pItem = *iItem;
		delete pItem->pScene;
		delete pItem;
	}

	SceneList.clear();
}

BOOL CCafeOrangeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	
	return TRUE;
}

void CCafeOrangeDoc::SceneInitialize()
{
	cSceneItem* pItem = new cSceneItem(SCENE_TYPE_PROTOTYPE);
	pItem->pScene = new cScene(SCENE_TYPE_PROTOTYPE);
	pItem->Name = STRING_PROGRAM_NAME;
	if (pItem)
		SceneList.push_back(pItem);
}

void CCafeOrangeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

cScene* CCafeOrangeDoc::GetScene(eSceneType SceneType)
{
	_SceneList::iterator iScene = SceneList.begin();
	for( ; iScene != SceneList.end() ; iScene++)
	{
		cSceneItem* pItem = *iScene;
		if (pItem->SceneType == SceneType)
			return pItem->pScene;
	}

	return 0;
}

#ifdef SHARED_HANDLERS

void CCafeOrangeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCafeOrangeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCafeOrangeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

#ifdef _DEBUG
void CCafeOrangeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCafeOrangeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



