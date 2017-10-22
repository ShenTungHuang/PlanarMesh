// STH_OCCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "STH_OCC.h"
#include "STH_OCCDlg.h"
#include "ImportExport.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CSTH_OCCDlg dialog




CSTH_OCCDlg::CSTH_OCCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSTH_OCCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSTH_OCCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XYPlane, m_buttonXY);
	DDX_Control(pDX, IDC_YZPlane, m_buttonYZ);
	DDX_Control(pDX, IDC_XZPlane, m_buttonXZ);
	DDX_Control(pDX, IDC_Line, m_buttonLine);
	DDX_Control(pDX, IDC_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_FeatureString, m_featureType);
	DDX_Control(pDX, IDC_PropertyString, m_featureProperty);
}

BEGIN_MESSAGE_MAP(CSTH_OCCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_XYPlane, &CSTH_OCCDlg::OnBnClickedXyplane)
	ON_BN_CLICKED(IDC_Line, &CSTH_OCCDlg::OnBnClickedLine)
	ON_BN_CLICKED(IDC_YZPlane, &CSTH_OCCDlg::OnBnClickedYzplane)
	ON_BN_CLICKED(IDC_XZPlane, &CSTH_OCCDlg::OnBnClickedXzplane)
	ON_BN_CLICKED(IDC_DELETE, &CSTH_OCCDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CSTH_OCCDlg message handlers

BOOL CSTH_OCCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// Get window size
	CWnd* pwndParent = this->GetParent();
	CRect rect_dlg;
	GetWindowRect(&rect_dlg);

	// For button image
	HBITMAP bmp_XY,bmp_YZ, bmp_XZ, bmp_Line, bmp_Delete;
	bmp_XY = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPXY));
	m_buttonXY.SetBitmap(bmp_XY);
	m_buttonXY.SetWindowPos(NULL, rect_dlg.right - 152, 10, 48, 48, NULL);
	bmp_YZ = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPYZ));
	m_buttonYZ.SetBitmap(bmp_YZ);
	m_buttonYZ.SetWindowPos(NULL, rect_dlg.right - 102, 10, 48, 48, NULL);
	bmp_XZ = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPXZ));
	m_buttonXZ.SetBitmap(bmp_XZ);
	m_buttonXZ.SetWindowPos(NULL, rect_dlg.right - 52, 10, 48, 48, NULL);
	bmp_Line = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPLINE));
	m_buttonLine.SetBitmap(bmp_Line);
	m_buttonLine.SetWindowPos(NULL, rect_dlg.right - 152, 60, 48, 48, NULL);
	bmp_Delete = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPDEL));
	m_buttonDelete.SetBitmap(bmp_Delete);
	m_buttonDelete.SetWindowPos(NULL, rect_dlg.right - 152, 110, 48, 48, NULL);
	m_featureType.ShowWindow(SW_HIDE);
	m_featureType.SetWindowPos(NULL, rect_dlg.right - 152, 180, 500, 20, NULL);
	m_featureProperty.ShowWindow(SW_HIDE);
	m_featureProperty.SetWindowPos(NULL, rect_dlg.right - 152, 200, 500, 40, NULL);

	GetDlgItem(IDC_LOADPART)-> ShowWindow(SW_HIDE);

	/* ----- OCC environment builder ----- */
	m_3dWnd = new COpenCascadeCtrl();
	CRect rect;
	GetDlgItem(IDC_OCCVIEW)->GetWindowRect(&rect);
	rect.top = 0;
	rect.left = 0;
	rect.right = rect_dlg.right - 155;
	rect.bottom = rect_dlg.bottom;
	ScreenToClient(&rect);
	m_3dWnd->Create(_T(""), WS_CHILD|WS_VISIBLE, rect, this, IDC_OCCVIEW);
	/* ----- OCC environment builder ----- */

	// Plot axis
	Handle(Geom_Axis2Placement) anAxis = new Geom_Axis2Placement(gp_Pnt(0,0,0),gp_Vec(0,0,1),gp_Dir(1,0,0));
	aTrihedron = new AIS_Trihedron(anAxis);
	aTrihedron->SetColor(Quantity_NOC_WHITE);
	m_3dWnd->m_pAISContext->Display(aTrihedron);

	// show button tip
	m_Mytip.Create(this); 
	m_Mytip.AddTool( GetDlgItem(IDC_XYPlane), "XY Plane" ); 
	m_Mytip.AddTool( GetDlgItem(IDC_YZPlane), "YZ Plane" ); 
	m_Mytip.AddTool( GetDlgItem(IDC_XZPlane), "XZ Plane" );
	m_Mytip.AddTool( GetDlgItem(IDC_Line), "Line" );
	m_Mytip.AddTool( GetDlgItem(IDC_DELETE), "Delete" );
	m_Mytip.SetDelayTime(200);
	m_Mytip.SetTipTextColor( RGB(0,0,0) );
	m_Mytip.SetTipBkColor( RGB(255,255,255));
	m_Mytip.Activate(TRUE);

	// Detecting features
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_COMPOUND);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_SOLID);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_SHELL);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_FACE);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_WIRE);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_EDGE);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_VERTEX);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_SHAPE);
	m_3dWnd->m_pAISContext->DeactivateStandardMode(TopAbs_COMPOUND);
	m_3dWnd->m_pAISContext->ActivateStandardMode(TopAbs_VERTEX);
	m_3dWnd->m_pAISContext->ActivateStandardMode(TopAbs_EDGE);
	m_3dWnd->m_pAISContext->ActivateStandardMode(TopAbs_FACE);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSTH_OCCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSTH_OCCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSTH_OCCDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	WPARAM wParam = MAKEWPARAM(nFlags,0);
	LPARAM lParam = MAKELPARAM(point.x, point.y);
	// send message
	m_3dWnd->SendMessage(WM_MOUSEMOVE, wParam, lParam);

	CDialog::OnMouseMove(nFlags, point);
}

void CSTH_OCCDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	WPARAM wParam = MAKEWPARAM(nFlags,0); 
	LPARAM lParam = MAKELPARAM(point.x, point.y); 
	// send message
	m_3dWnd->SendMessage(WM_RBUTTONDOWN, wParam, lParam);

	CDialog::OnRButtonDown(nFlags, point);
}

void CSTH_OCCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	WPARAM wParam = MAKEWPARAM(nFlags,0); 
	LPARAM lParam = MAKELPARAM(point.x, point.y);
	// send message
	m_3dWnd->SendMessage(WM_LBUTTONDOWN, wParam, lParam);
	// show information
	ShowFeatureInfo();
	CDialog::OnLButtonDown(nFlags, point);
}

void CSTH_OCCDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	WPARAM wParam = MAKEWPARAM(nFlags,0); 
	LPARAM lParam = MAKELPARAM(point.x, point.y);
	// send message
	m_3dWnd->SendMessage(WM_LBUTTONUP, wParam, lParam);
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CSTH_OCCDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	WPARAM wParam = MAKEWPARAM(nFlags,zDelta); 
	LPARAM lParam = MAKELPARAM(pt.x, pt.y); 
	if (m_3dWnd != NULL) { return m_3dWnd->SendMessage(WM_MOUSEWHEEL, wParam, lParam); }

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CSTH_OCCDlg::OnBnClickedXyplane()
{
	m_3dWnd->Top();
}

void CSTH_OCCDlg::OnBnClickedYzplane()
{
	m_3dWnd->Front();
}

void CSTH_OCCDlg::OnBnClickedXzplane()
{
	m_3dWnd->Left();
}

void CSTH_OCCDlg::OnBnClickedLine()
{
	if ( m_3dWnd->tappedLineButton == false )
	{
		// setting mode
		m_buttonLine.SetState(true);
		m_3dWnd->tappedLineButton = true;
		m_3dWnd->dragMode = false;

		// hide info
		if ( m_3dWnd->selectType == 1 )
		{
			m_featureType.ShowWindow(SW_HIDE);
			m_featureProperty.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		// setting mode and clear view
		m_buttonLine.SetState(false);
		m_3dWnd->m_pAISContext->Remove(m_3dWnd->ais_Pnt1, false);
		m_3dWnd->m_pAISContext->Remove(m_3dWnd->ais_Line, false);
		m_3dWnd->m_pAISContext->UpdateCurrentViewer();
		m_3dWnd->hasFirstPoint = false;
		m_3dWnd->tappedLineButton = false;
	}

	// for reset after tap on vertex
	if ( m_3dWnd->selectType != -1 )
	{
		m_3dWnd->selectType = -1;
		if ( m_3dWnd->selectIndex != -1 )
		{
			if ( m_3dWnd->selectEdgeMode == true )
			{
				m_3dWnd->m_pAISContext->SetColor(m_3dWnd->allLines[m_3dWnd->selectIndex].lineFeature, Quantity_NOC_GREEN, true);
				m_3dWnd->selectEdgeMode = false;
			}
			else if ( m_3dWnd->selectFaceMode == true )
			{
				m_3dWnd->m_pAISContext->SetColor(m_3dWnd->allFaces[m_3dWnd->selectIndex].faceFeature, Quantity_NOC_GREEN, true);
				m_3dWnd->selectFaceMode = false;
			}
		}
		m_3dWnd->m_pAISContext->UpdateCurrentViewer();
		m_3dWnd->selectIndex = -1;
	}
}

void CSTH_OCCDlg::OnBnClickedDelete()
{
	// Delete selected line and setting mode
	if ( m_3dWnd->selectIndex != -1 )
	{
		m_3dWnd->m_pAISContext->Remove(m_3dWnd->allLines[m_3dWnd->selectIndex].lineFeature, false);
		m_3dWnd->m_pAISContext->Remove(m_3dWnd->allLines[m_3dWnd->selectIndex].startPoint, false);
		m_3dWnd->m_pAISContext->Remove(m_3dWnd->allLines[m_3dWnd->selectIndex].endPoint, false);
		
		m_3dWnd->allLines.erase(m_3dWnd->allLines.begin() + m_3dWnd->selectIndex);
		
		m_3dWnd->selectIndex = -1;
		m_3dWnd->selectType = -1;
		ShowFeatureInfo();
		m_3dWnd->m_pAISContext->UpdateCurrentViewer();
		m_3dWnd->dragMode = false;
		m_3dWnd->selectEdgeMode = false;

		// reset section edge information if more than  edges
		if ( m_3dWnd->allLines.size() > 1 )
		{
			m_3dWnd->ChackAllEdgeIntersection();
		}

		// remove all faces
		if ( m_3dWnd->allFaces.size() > 0 )
		{
			for ( int ii = 0 ; ii < m_3dWnd->allFaces.size() ; ii++ )
			{
				m_3dWnd->m_pAISContext->Remove(m_3dWnd->allFaces[ii].faceFeature, false);
			}
			m_3dWnd->allFaces.clear();
			m_3dWnd->m_pAISContext->UpdateCurrentViewer();
		}

		// recalculate if more than 3 edges
		if ( m_3dWnd->allLines.size() >= 3 )
		{
			m_3dWnd->PlotFaces();
		}
		//m_3dWnd->test();
	}
}

void CSTH_OCCDlg::ShowFeatureInfo()
{
	// featureType = -1	->  nothing
	// featureType = 1	->  edge
	// featureType = 2	->  face

	if ( m_3dWnd->selectType == -1 || m_3dWnd->selectType == 3 )
	{
		// nothing selected or tap on vertex
		m_featureType.ShowWindow(SW_HIDE);
		m_featureProperty.ShowWindow(SW_HIDE);
	}
	else if ( m_3dWnd->selectType == 1 )
	{
		// select an edge
		m_featureType.ShowWindow(SW_SHOW);
		m_featureProperty.ShowWindow(SW_SHOW);
		CString propertyString = "Selected Feature is Edge";
		m_featureType.SetWindowTextA(propertyString);
		CString lengthString;
		lengthString.Format("%.2f",m_3dWnd->allLines[m_3dWnd->selectIndex].lineLength);
		CString showLengthString = "Length of Edge : " + lengthString;
		m_featureProperty.SetWindowTextA(showLengthString);
	}
	else if ( m_3dWnd->selectType == 2 )
	{
		// select a face
		m_featureType.ShowWindow(SW_SHOW);
		m_featureProperty.ShowWindow(SW_SHOW);
		CString propertyString = "Selected Feature is Face";
		m_featureType.SetWindowTextA(propertyString);
		CString lengthString, areaString;
		lengthString.Format("%.2f",m_3dWnd->allFaces[m_3dWnd->selectIndex].perimeter);
		areaString.Format("%.2f",m_3dWnd->allFaces[m_3dWnd->selectIndex].area);
		CString showLengthString = "Area of Face : " + areaString + "\n" + "Perimeter of  Face : " + lengthString;
		m_featureProperty.SetWindowTextA(showLengthString);
	}
}

BOOL CSTH_OCCDlg::PreTranslateMessage(MSG* pMsg)
{
	// for hot key setting
	if( (pMsg->message == WM_KEYUP) && (pMsg->wParam == 0x4C) )	
	{
		// L key  for Line button
		OnBnClickedLine();
	}
	else if( (pMsg->message == WM_KEYUP) && (pMsg->wParam == 0x44) )
	{
		// D key for delete button
		OnBnClickedDelete();
	}
	else if ( pMsg->message==WM_MOUSEMOVE )
	{ m_Mytip.RelayEvent(pMsg); }

	return CDialog::PreTranslateMessage(pMsg);
}
