#pragma once

class CTextureDlg : public CDialogEx
{

	DECLARE_MESSAGE_MAP()
public:
	enum eTextureList{
		TEXTURE_LIST_INDEX,
		TEXTURE_LIST_FILE,
		TEXTURE_LIST_OPERATION,
//		TEXTURE_LIST_FILTER,
		TEXTURE_LIST_ADDRESS,
//		TEXTURE_LIST_SIZE,
	};

	CListCtrl TextureList;
	CComboBox AddressComboBox;
	CComboBox FilterComboBox;
	CComboBox OperationComboBox;

	CButton	  AddBtn;
	CButton	  DeleteBtn;

	void	DefineListHeader();
	CTextureDlg();  
	virtual ~CTextureDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	void AddItem(int Row,int Column, CString& Message);	   
	int	GetSelectedItemIndex();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddTexture();
	afx_msg void OnDeleteTexture();
	afx_msg void OnChangeOperation();
};
