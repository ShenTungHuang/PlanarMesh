// STH_OCCDlg.h : header file
//

#pragma once
#include "OpenCascadeCtrl.h"
#include "afxwin.h"


// CSTH_OCCDlg dialog
class CSTH_OCCDlg : public CDialog
{
// Construction
public:
	CSTH_OCCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STH_OCC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	COpenCascadeCtrl *m_3dWnd;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	Handle(AIS_Trihedron) aTrihedron; //Axis
	Handle(AIS_Shape) my_AISShape; // For Save Shape
	CButton m_buttonXY;
	CButton m_buttonYZ;
	CButton m_buttonXZ;
	CButton m_buttonLine;
	CButton m_buttonDelete;
	CStatic m_featureType;
	CStatic m_featureProperty;
	afx_msg void OnBnClickedXyplane();
	afx_msg void OnBnClickedYzplane();
	afx_msg void OnBnClickedXzplane();
	afx_msg void OnBnClickedLine();
	afx_msg void OnBnClickedDelete();
	void ShowFeatureInfo();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CToolTipCtrl m_Mytip;
};
