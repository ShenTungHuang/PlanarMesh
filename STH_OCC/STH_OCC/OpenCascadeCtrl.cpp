#include "stdafx.h"
#include "OpenCascadeCtrl.h"
#include <ElSLib.hxx>
#include <ProjLib.hxx>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// COpenCascadeCtrl

//Standard_Integer cXmin = 0;
//Standard_Integer cYmin = 0;  
Standard_Integer cXmax = 600;
Standard_Integer cYmax = 600;
//Standard_Integer cZmin = 0;
//Standard_Integer cZmax = 200;
Standard_Integer cZoomStep = 20;



IMPLEMENT_DYNAMIC(COpenCascadeCtrl, CStatic)

COpenCascadeCtrl::COpenCascadeCtrl()
{
	m_pView = NULL;
	m_pViewer = NULL;
	m_pWNTWindow = NULL;
	m_pAISContext = NULL;
	m_pGraphicDevice = NULL;
    m_CurrentMode = CurAction3d_Nothing;
}

COpenCascadeCtrl::~COpenCascadeCtrl()
{ m_pView->Remove(); }


BEGIN_MESSAGE_MAP(COpenCascadeCtrl, CStatic)
    ON_WM_CREATE()
    ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// COpenCascadeCtrl message handlers
int COpenCascadeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( CStatic::OnCreate(lpCreateStruct) == -1 ) { return -1; }

	// build m_pGraphicDevice
	try
	{ m_pGraphicDevice = new Graphic3d_WNTGraphicDevice(); }
	catch (Standard_Failure)
	{ AfxMessageBox("Fatal Error During Graphic Initialisation"); }; 
    
	// build m_pViewer
	m_pViewer = new V3d_Viewer(m_pGraphicDevice, (short *) (""));
	m_pViewer->SetDefaultLights();
	m_pViewer->SetLightOn();
	
	// build m_pAISContext, base on m_pViewer
	m_pAISContext = new AIS_InteractiveContext(m_pViewer);
	m_pView = m_pViewer->CreateView();	
	m_pView->SetDegenerateModeOn();

    // store for restore state after rotation (witch is in Degenerated mode)
    //m_DegenerateModeIsOn = Standard_True;

	// build m_pWNTWindow point to m_pGraphicDevice
	/* add by STH for test */
	HWND temp = GetSafeHwnd();
	/* add by STH for test */
	m_pWNTWindow = new WNT_Window(m_pGraphicDevice,GetSafeHwnd());
    m_pView->SetWindow(m_pWNTWindow );
	if ( !m_pWNTWindow->IsMapped() ) { m_pWNTWindow ->Map(); }

	// Plot Triedron
	m_pView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_RED, 0.1,V3d_ZBUFFER);

	// Set Antialiasing On
	m_pView->SetAntialiasingOn();

	// Set Back Ground
	m_pView->SetBgGradientColors(Quantity_Color(Quantity_NOC_NAVYBLUE),Quantity_Color(Quantity_NOC_SNOW),Aspect_GFM_DIAG2);

	// variable and mode initial
	tappedLineButton = false;
	selectIndex = -1;
	selectType = -1;
	selectEdgeMode = false;
	selectFaceMode = false;
	hasFirstPoint = false;
	dragMode = false;

    return 0;
}


void COpenCascadeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect aRect;
	GetWindowRect(aRect);
	if( m_rWidth != aRect.Width() || m_rHeight != aRect.Height() )
	{
		m_rWidth = aRect.Width();
		m_rHeight = aRect.Height();
		::PostMessage ( GetSafeHwnd () , WM_SIZE , SW_SHOW , m_rWidth + m_rHeight*65536 );
	}
	
	m_pView->Redraw();
}

void COpenCascadeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags & MK_LBUTTON)
    {
		if(nFlags & MK_CONTROL)
		{
			// for Pan view
			m_pView->Pan(point.x-myXmax,myYmax-point.y);
			myXmax = point.x; myYmax = point.y;
		}
		else if ( dragMode = true )
		{
			// remove all faces in viewer
			if ( selectEdgeMode == false && selectFaceMode == false && allFaces.size() > 0 )
			{
				for ( int ii = 0 ; ii < allFaces.size() ; ii++ )
				{
					m_pAISContext->Remove(allFaces[ii].faceFeature, false);
				}
				m_pAISContext->UpdateCurrentViewer();
			}
			// call DragLine() to re-plot lines
			DragLine(point);
		}
	}
	else if ( nFlags == 0 && tappedLineButton == true && hasFirstPoint == true  )
	{
		// mouse move without button
		PlotLine(point);
	}
    else if ( nFlags & MK_RBUTTON)
    {
		// for Rotation view
		if ( nFlags & MK_CONTROL ) { m_pView->Rotation(point.x,point.y); }
	}
	else
	{
		myXmax = point.x; myYmax = point.y;
		m_pAISContext->MoveTo(point.x , point.y , m_pView );
	}

	CStatic::OnMouseMove(nFlags, point);
}

BOOL COpenCascadeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   // «öÀ£Ctrl+MouseWheel ÁY©ñª«Åé
   if (  nFlags & MK_CONTROL  )
   {
	  cXmax = pt.x + zDelta/cZoomStep; 
	  cYmax = pt.y + zDelta/cZoomStep;
	  if (cYmax < 0)	{ cYmax = 0; }
	  if (cXmax < 0)	{ cXmax = 0; }

	  m_pView->Zoom(pt.x,pt.y,cXmax,cYmax);
    }
	return TRUE;
}

void COpenCascadeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(nFlags & MK_CONTROL) { m_pView->StartRotation(point.x,point.y); }
	CStatic::OnRButtonDown(nFlags, point);
}

void COpenCascadeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( tappedLineButton == false )
	{
		// reset view and variable
		selectType = -1;
		if ( selectIndex != -1 )
		{
			if ( selectEdgeMode == true )
			m_pAISContext->SetColor(allLines[selectIndex].lineFeature, Quantity_NOC_GREEN, true);

			if ( selectFaceMode == true )
			m_pAISContext->SetColor(allFaces[selectIndex].faceFeature, Quantity_NOC_GREEN, true);

			selectIndex = -1;
			selectFaceMode = false;
			selectEdgeMode = false;
		}
		// get select feature
		m_pAISContext->Select(); 
		m_pAISContext->InitSelected();
		if (m_pAISContext->MoreSelected())
		{
			TopoDS_Shape selectshape = m_pAISContext->SelectedShape();
			TopAbs_ShapeEnum shape_type = selectshape.ShapeType();

			if ( shape_type == TopAbs_EDGE )
			{
				// call SearchEdge()
				selectType = 1;
				SearchEdge(TopoDS::Edge(selectshape));
			}
			else if ( shape_type == TopAbs_FACE )
			{
				// call SearchFace()
				selectType = 2;
				SearchFace(TopoDS::Face(selectshape));
			}
			else if ( shape_type == TopAbs_VERTEX )
			{
				// call SearchPoint()
				selectType = 3;
				SearchPoint(TopoDS::Vertex(selectshape));
			}
			m_pAISContext->ClearSelected();
			m_pAISContext->UpdateCurrentViewer();
		}
	}
	CStatic::OnLButtonDown(nFlags, point);

}

void COpenCascadeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( tappedLineButton == true )
	{
		// reset variable
		selectFaceMode = false;
		selectEdgeMode = false;
		// plot line mode
		if ( hasFirstPoint == false )
		{
			// set first point
			SetandPlotPoint(1, point);
			hasFirstPoint = true;
		}
		else if ( hasFirstPoint == true )
		{
			// set second point and re-plot
			SetandPlotPoint(2, point);
			PlotLine(point);
			// set ais object for plot
			TopoDS_Shape temp = ais_Line->Shape();
			Handle(AIS_Shape) temp_ais = new AIS_Shape(temp);
			temp_ais->SetColor(Quantity_NOC_GREEN);
			// Save Line Feature
			LineFeature tempLine;
			tempLine.lineFeature = temp_ais;
			tempLine.startPoint = new AIS_Shape(ais_Pnt1->Shape());
			tempLine.endPoint = new AIS_Shape(ais_Pnt2->Shape());
			tempLine.startPnt = startPoint;
			tempLine.endPnt = endPoint;
			tempLine.lineLength = GetEdgeLength(TopoDS::Edge(temp));
			allLines.push_back(tempLine);
			// save segment info
			SectionEdge temp_seg;
			temp_seg.start = startPoint;
			temp_seg.end = endPoint;
			temp_seg.edge = new AIS_Shape(temp);
			allLines[allLines.size() - 1].sectionEdges.push_back(temp_seg);
			// reset view
			m_pAISContext->Remove(ais_Line,false);
			m_pAISContext->Remove(ais_Pnt1,false);
			m_pAISContext->Remove(ais_Pnt2,false);
			m_pAISContext->Display( allLines[allLines.size()-1].lineFeature, false );
			m_pAISContext->Display( allLines[allLines.size()-1].startPoint, false );
			m_pAISContext->Display( allLines[allLines.size()-1].endPoint, false );
			m_pAISContext->UpdateCurrentViewer();
			// set segment edges
			ChackEdgeIntersection();
			// plot faces
			if ( allLines.size() >= 3 && selectFaceMode == false && selectEdgeMode == false )
			{
				PlotFaces();
			}
			// reset first point
			SetandPlotPoint(1, point);
		}
	}
	else if ( allLines.size() > 0 && dragMode == true )
	{
		// get position in drag mode
		dragPoint = GetCurrentPoint(point);
		TopoDS_Vertex vert = BRepBuilderAPI_MakeVertex(dragPoint);
		Handle(AIS_Shape) ais_pnt = new AIS_Shape(vert);
		for ( int ii = 0 ; ii < lineOfPoint.size() ; ii++ )
		{
			// check tap on first or end point
			int index = lineOfPoint[ii].lineIndex;
			if ( lineOfPoint[ii].firstPnt )
			{
				// pick on start point
				allLines[index].startPoint = new AIS_Shape(ais_pnt->Shape());
				allLines[index].startPnt = dragPoint;
				m_pAISContext->Display(allLines[index].startPoint, false);
			}
			else
			{
				// pick on end point
				allLines[index].endPoint = new AIS_Shape(ais_pnt->Shape());
				allLines[index].endPnt = dragPoint;
				m_pAISContext->Display(allLines[index].endPoint, false);
			}
			// reset view
			m_pAISContext->Remove(lineOfPoint[ii].newLine, false);
			TopoDS_Edge newEdge = BRepBuilderAPI_MakeEdge(allLines[index].startPnt, allLines[index].endPnt);
			allLines[index].lineFeature = new AIS_Shape(newEdge);
			allLines[index].lineFeature->SetColor(Quantity_NOC_GREEN);
			allLines[index].lineLength = GetEdgeLength(newEdge);
			m_pAISContext->Display(allLines[index].lineFeature, false);
			// set segment info
			SectionEdge temp_seg;
			temp_seg.start = allLines[index].startPnt;
			temp_seg.end = allLines[index].endPnt;
			temp_seg.edge = new AIS_Shape(newEdge);
			allLines[index].sectionEdges.clear();
			allLines[index].sectionEdges.push_back(temp_seg);
		}
		// all re-check segment info for all lines
		ChackAllEdgeIntersection();
		// plot faces
		if ( allLines.size() >= 3 && selectFaceMode == false && selectEdgeMode == false )
		{
			PlotFaces();
		}
		m_pAISContext->UpdateCurrentViewer();
		lineOfPoint.clear();
		dragMode = false;

		//test();
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void COpenCascadeCtrl::Clear()
{
	AIS_ListOfInteractive aList;
	m_pAISContext->DisplayedObjects(aList);
	AIS_ListIteratorOfListOfInteractive aListIterator;
	for( aListIterator.Initialize(aList) ; aListIterator.More() ; aListIterator.Next() )
	{ m_pAISContext->Remove(aListIterator.Value()); }
}

// all view proj function
void COpenCascadeCtrl::Right() { m_pView->SetProj(V3d_Yneg); }
void COpenCascadeCtrl::Top() { m_pView->SetProj(V3d_Zpos); }
void COpenCascadeCtrl::Left() { m_pView->SetProj(V3d_Ypos); }
void COpenCascadeCtrl::Front() { m_pView->SetProj(V3d_Xpos); }
void COpenCascadeCtrl::Bottom() { m_pView->SetProj(V3d_Zneg); }
void COpenCascadeCtrl::Back() { m_pView->SetProj(V3d_Xneg); }
void COpenCascadeCtrl::ZoolAll() { m_pView->FitAll(); m_pView->ZFitAll(); }
void COpenCascadeCtrl::Axo() { m_pView->SetProj(V3d_XposYnegZpos); }

gp_Pnt COpenCascadeCtrl::ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView)
{
	// conver view position to CAD environment position
	V3d_Coordinate XEye,YEye,ZEye,XAt,YAt,ZAt;
	aView->Eye(XEye,YEye,ZEye);
	aView->At(XAt,YAt,ZAt);
	gp_Pnt EyePoint(XEye,YEye,ZEye);
	gp_Pnt AtPoint(XAt,YAt,ZAt);

	gp_Vec EyeVector(EyePoint,AtPoint);
	gp_Dir EyeDir(EyeVector);

	gp_Pln PlaneOfTheView = gp_Pln(AtPoint,EyeDir);
	Standard_Real X,Y,Z;
	aView->Convert(x,y,X,Y,Z);
	gp_Pnt ConvertedPoint(X,Y,Z);
	gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView,ConvertedPoint);

	gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(), ConvertedPointOnPlane.Y(), PlaneOfTheView);
	return ResultPoint;
}

void COpenCascadeCtrl::PlotLine(CPoint point)
{
	// get plot line when buttonUP
	m_pAISContext->Remove(ais_Line, false);
	TopoDS_Edge dragLine = BRepBuilderAPI_MakeEdge(startPoint, GetCurrentPoint(point));
	ais_Line = new AIS_Shape(dragLine);
	ais_Line->SetColor(Quantity_NOC_GREEN);
	// plot line and update view
	m_pAISContext->Display(ais_Line, false);
	m_pAISContext->UpdateCurrentViewer();
}

gp_Pnt COpenCascadeCtrl::GetCurrentPoint(CPoint point)
{
	// get current position
	gp_Pnt temp = ConvertClickToPoint(point.x, point.y, m_pView);
	// check if close to existing point
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		// check start point on line
		if ( allLines[ii].startPnt.Distance(temp) < 10 )
		{
			temp = allLines[ii].startPnt;
			break;
		}
		// check end point on line
		if ( allLines[ii].endPnt.Distance(temp) < 10 )
		{
			temp = allLines[ii].endPnt;
			break;
		}
	}
	// return current point or existing point
	return temp;
}

double COpenCascadeCtrl::GetEdgeLength(TopoDS_Edge edge)
{
	// calculate and return length for edge
	GProp_GProps System;
	BRepGProp::LinearProperties(edge, System);
	return  System.Mass();
}

double COpenCascadeCtrl::GetFaceArea(TopoDS_Shape face)
{
	// calculate and return area for face
	GProp_GProps System;
	BRepGProp::SurfaceProperties(face, System);
	return  System.Mass();
}

double COpenCascadeCtrl::GetFacePerimeter(TopoDS_Wire wire)
{
	// calculate and return area for face
	GProp_GProps System;
	BRepGProp::LinearProperties(wire, System);
	return  System.Mass();
}

void COpenCascadeCtrl::SetandPlotPoint(int number, CPoint point)
{
	// get current position and then plot and setting
	if ( number == 1 )
	{
		// set and plot start point for line
		startPoint = GetCurrentPoint(point);
		TopoDS_Vertex vert = BRepBuilderAPI_MakeVertex(startPoint);
		ais_Pnt1 = new AIS_Shape(vert);
		m_pAISContext->Display(ais_Pnt1);
	}
	else if ( number == 2 )
	{
		// set and plot end point for line
		endPoint = GetCurrentPoint(point);
		TopoDS_Vertex vert = BRepBuilderAPI_MakeVertex(endPoint);
		ais_Pnt2 = new AIS_Shape(vert);
		m_pAISContext->Display(ais_Pnt2);
	}
}

void COpenCascadeCtrl::SearchEdge(TopoDS_Edge edge)
{
	// search edge be tapped
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		if ( TopoDS::Edge(allLines[ii].lineFeature->Shape()) == edge )
		{
			// set index variable and display color
			selectIndex = ii;
			m_pAISContext->SetColor(allLines[ii].lineFeature, Quantity_NOC_RED, true);
			selectEdgeMode = true;
			selectFaceMode = false;
			break;
		}
	}
}

void COpenCascadeCtrl::SearchPoint(TopoDS_Vertex vertex)
{
	// search all lines by selected vertex
	gp_Pnt aPoint = BRep_Tool::Pnt (vertex);

	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		if ( allLines[ii].startPnt.IsEqual(aPoint, 0.01) || allLines[ii].endPnt.IsEqual(aPoint, 0.01) )
		{
			// set DragLineInfo variable
			DragLineInfo temp;
			temp.lineIndex = ii;
			temp.newLine = new AIS_Shape(allLines[ii].lineFeature->Shape());
			temp.newLine->SetColor(Quantity_NOC_GREEN);
			// check start or end point
			if ( allLines[ii].startPnt.IsEqual(aPoint, 0.01) )
			{
				// if pick on start point
				temp.fixPnt = allLines[ii].endPnt;
				temp.firstPnt = true;
				m_pAISContext->Remove(allLines[ii].startPoint, false);
			}
			else
			{
				// if pick on end point
				temp.fixPnt = allLines[ii].startPnt;
				temp.firstPnt = false;
				m_pAISContext->Remove(allLines[ii].endPoint, false);
			}
			// save information
			lineOfPoint.push_back(temp);
			// re-plot variables for drag later
			m_pAISContext->Remove(allLines[ii].lineFeature, false);
			m_pAISContext->Display(lineOfPoint[lineOfPoint.size() - 1].newLine, false);
		}
	}

	m_pAISContext->UpdateCurrentViewer();
	dragMode = true;
}

void COpenCascadeCtrl::SearchFace(TopoDS_Face face)
{
	// search face be tapped
	for ( int ii = 0 ; ii < allFaces.size() ; ii++ )
	{
		if ( TopoDS::Face(allFaces[ii].faceFeature->Shape()) == face )
		{
			// set index variable and display color
			selectIndex = ii;
			m_pAISContext->SetColor(allFaces[ii].faceFeature, Quantity_NOC_RED, true);
			selectFaceMode = true;
			selectEdgeMode = false;
			break;
		}
	}
}

void COpenCascadeCtrl::ChackEdgeIntersection()
{
	std::vector<SectionInfo> SectionPnt;
	SectionPnt.clear();
	// Check all existed section edges
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		for ( int jj = 0 ; jj < allLines[ii].sectionEdges.size() ; jj++ )
		{
			if ( allLines[ii].sectionEdges[jj].start.IsEqual(startPoint, 0.01) || 
				allLines[ii].sectionEdges[jj].start.IsEqual(endPoint, 0.01) || 
				allLines[ii].sectionEdges[jj].end.IsEqual(startPoint, 0.01) || 
				allLines[ii].sectionEdges[jj].end.IsEqual(startPoint, 0.01) )
			{} // do nothing when edge connected
			else
			{
				// Get section point
				Handle(Geom_Curve) aCurve1 = GC_MakeSegment(startPoint, endPoint).Value();
				Handle(Geom_Curve) aCurve2 = GC_MakeSegment(allLines[ii].sectionEdges[jj].start, allLines[ii].sectionEdges[jj].end).Value();
				GeomAPI_ExtremaCurveCurve InterCurve(aCurve1, aCurve2);
				int ss = InterCurve.NbExtrema();
				if ( InterCurve.NbExtrema() == 1 )
				{
					// save info for saperate new edge
					gp_Pnt  p1, p2;
					InterCurve.NearestPoints(p1,p2);
					if ( allLines[ii].sectionEdges[jj].start.IsEqual(p1, 0.01) || allLines[ii].sectionEdges[jj].end.IsEqual(p1, 0.01) )
					{}
					else
					{
						allLines[ii].sectionEdges.insert(allLines[ii].sectionEdges.begin() + jj, allLines[ii].sectionEdges[jj]);

						SectionInfo section_temp;
						section_temp.pnt = p1;
						section_temp.dist = startPoint.Distance(p1);
						SectionPnt.push_back(section_temp);

						// saperate into two edge - Part1
						SaveSectionEdge(ii, jj, allLines[ii].sectionEdges[jj].start, p1);

						// saperate into two edge - Part2
						SaveSectionEdge(ii, jj+1, p1, allLines[ii].sectionEdges[jj+1].end);
						// leave current edge segment and go to next edge
						jj = allLines[ii].sectionEdges.size();
					}
				}
			}
		}
	}

	// saperate new edge
	if ( SectionPnt.size() > 0 )
	{
		// sort point in sequence
		if ( SectionPnt.size() >= 2 )
		{ SortPnt(SectionPnt); }
		// set segment edges by point
		gp_Pnt tempStartPnt, tempEndPnt;
		int index1 = allLines.size() - 1;
		int index2 = 0;
		for ( int ii = 0 ; ii < SectionPnt.size() + 1 ; ii++ )
		{
			if ( ii != SectionPnt.size() )
			{
				// for all part not include end part
				allLines[index1].sectionEdges.push_back(allLines[index1].sectionEdges[index2]);
				allLines[index1].sectionEdges[index2+1].start = SectionPnt[ii].pnt;
				tempStartPnt = allLines[index1].sectionEdges[index2].start;
				tempEndPnt = SectionPnt[ii].pnt;
			}
			else if ( ii == SectionPnt.size() )
			{
				// for set end part
				tempStartPnt = SectionPnt[ii - 1].pnt;
				tempEndPnt = allLines[index1].sectionEdges[index2].end;
			}
			// save information
			SaveSectionEdge(index1, index2, tempStartPnt, tempEndPnt);
			index2++;
		}

		SectionPnt.clear();
	}

	m_pAISContext->UpdateCurrentViewer();
}

void COpenCascadeCtrl::SortPnt(std::vector<SectionInfo> &SectionPnt)
{
	// sort points in sequence by bubble
	int n = SectionPnt.size();
	for ( int ii = 0 ; ii < n - 1 ; ii++ )
	{
		for ( int jj = 0 ; jj < n-ii-1 ; jj++)
		{
			if (SectionPnt[jj].dist > SectionPnt[jj+1].dist)
			{
				SectionInfo temp = SectionPnt[jj];
				SectionPnt[jj] = SectionPnt[jj+1];
				SectionPnt[jj+1] = temp;
			}
		}
	}
}

void COpenCascadeCtrl::DragLine(CPoint point)
{
	// get current point
	dragPoint = GetCurrentPoint(point);
	for ( int ii = 0 ; ii < lineOfPoint.size() ; ii++ )
	{
		// remove previous and plot current line
		m_pAISContext->Remove(lineOfPoint[ii].newLine, false);
		TopoDS_Edge dragLine = BRepBuilderAPI_MakeEdge(lineOfPoint[ii].fixPnt, dragPoint);
		lineOfPoint[ii].newLine = new AIS_Shape(dragLine);
		lineOfPoint[ii].newLine->SetColor(Quantity_NOC_GREEN);
		m_pAISContext->Display(lineOfPoint[ii].newLine, false);
	}

	m_pAISContext->UpdateCurrentViewer();
}

void COpenCascadeCtrl::SaveSectionEdge(int index1, int index2, gp_Pnt p1, gp_Pnt p2)
{
	// save section edges and point for original edge
	TopoDS_Edge newEdge = BRepBuilderAPI_MakeEdge(p1, p2);
	SectionEdge temp;
	temp.edge = new AIS_Shape(newEdge);
	temp.start = p1;
	temp.end = p2;
	allLines[index1].sectionEdges[index2] = temp;
}

void COpenCascadeCtrl::ChackAllEdgeIntersection()
{
	// resave all line information unless first one in a temp
	std::vector<LineFeature> temp_vec;
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		allLines[ii].sectionEdges.clear();
		SectionEdge temp;
		temp.edge = new AIS_Shape(allLines[ii].lineFeature->Shape());
		temp.start = allLines[ii].startPnt;
		temp.end = allLines[ii].endPnt;
		allLines[ii].sectionEdges.push_back(temp);
		if ( ii > 0 )
		{ temp_vec.push_back(allLines[ii]); }
	}
	allLines.erase(allLines.begin() + 1, allLines.end());
	
	// add new line and call ChackEdgeIntersection() to check intersection
	for ( int ii = 0 ; ii < temp_vec.size() ; ii++ )
	{
		allLines.push_back(temp_vec[ii]);
		startPoint = temp_vec[ii].startPnt;
		endPoint = temp_vec[ii].endPnt;
		ChackEdgeIntersection();
	}
}

void COpenCascadeCtrl::PlotFaces()
{
	// remove all faces and reset vertor
	if ( allFaces.size() > 0 )
	{
		for ( int ii = 0 ; ii < allFaces.size() ; ii++ )
		{
			m_pAISContext->Remove(allFaces[ii].faceFeature, false);
		}
		allFaces.clear();
	}

	// build faces
	BuildFaces();

	// plot all faces
	for ( int ii = 0 ; ii < allFaces.size() ; ii++ )
	{
		m_pAISContext->Display(allFaces[ii].faceFeature, false);
	}
	m_pAISContext->UpdateCurrentViewer();
}

void COpenCascadeCtrl::BuildFaces()
{
	// initial variables
	halfEdgeInfo.clear();
	pntList.clear();
	connectEdges.clear();
	linkInfoList.clear();
	allTravel.clear();
	closeTravel.clear();
	allFaces.clear();
	// build point list for next link process
	BuildPntList();

	int level = 1;
	bool checkClose = false;
	// call SetInit() to set initial root
	SetInit();
	// travel to build edge link by vertex
	while ( allTravel.size() > 0 )
	{
		if ( Travel(checkClose) == false )
		{
			// if without new branch?
			if ( unconnectPnt.size() == 0 )
			{ break; } // leave if without ant point?
			else
			{
				// reset new root
				std::vector<int> temp;
				temp.push_back(unconnectPnt[0]);
				allTravel.push_back(temp);
				unconnectPnt.erase(unconnectPnt.begin() + 0);
			}
		}
		else
		{
			// 
			if ( unconnectPnt.size() != 0 )
			{
				// reset new root
				std::vector<int> temp;
				temp.push_back(unconnectPnt[0]);
				allTravel.push_back(temp);
				unconnectPnt.erase(unconnectPnt.begin() + 0);
			}
		}

		// check level to start call function check closed
		if ( level < 3 )
			level++;
		
		if ( level >= 3 )
			checkClose = true;
	}
	// compare and delete the same vertex result
	CompareTravel();
	// get edge name by vertext link
	GetLinkEdges();
	// compare and delete the same edge result
	CompareTravel();

	 // make face by edge name
	for ( int ii = 0 ; ii < closeTravel.size() ; ii++ )
	{
		// add section edge to build closed wire
		BRepBuilderAPI_MakeWire MKW;
		for ( int jj = 0 ; jj < closeTravel[ii].size() ; jj++ )
		{
			int index = closeTravel[ii][jj];
			MKW.Add( halfEdgeInfo[index].edge );
		}
		// If wire is done
		if ( MKW.IsDone() )
		{
			// build face by closed wire
			TopoDS_Wire temp_wire = MKW.Wire();
			TopoDS_Face face = BRepBuilderAPI_MakeFace(temp_wire);
			Handle(AIS_Shape) temp_AIS_face = new AIS_Shape(face);
			// save information
			FaceFeature temp;
			temp.faceFeature = new AIS_Shape(face);
			temp.faceFeature->SetColor(Quantity_NOC_GREEN);
			temp.faceFeature->SetDisplayMode(AIS_Shaded);
			temp.faceFeature->SetMaterial(Graphic3d_NOM_PLASTIC);
			temp.faceFeature->SetTransparency(0.8);
			// save area and perimeter
			temp.area = GetFaceArea(BRepBuilderAPI_MakeFace(temp_wire).Face());
			temp.perimeter = GetFacePerimeter(temp_wire);
			allFaces.push_back(temp);
		}
	}
	// remove overlap face
	RemoveOverlap();
}

void COpenCascadeCtrl::BuildPntList()
{
	// build point list by all point from each section edge
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		for ( int jj = 0 ; jj < allLines[ii].sectionEdges.size() ; jj++ )
		{
			HalfEdge temp;

			// save all pnts
			if ( ii == 0 && jj == 0 )
			{
				// save first pair points
				pntList.push_back( allLines[ii].sectionEdges[jj].start );
				pntList.push_back( allLines[ii].sectionEdges[jj].end );
				temp.pnt1 = 0;
				temp.pnt2 = 1;
				connectEdges.push_back(1);
				connectEdges.push_back(1);
			}
			else
			{
				int pntNum;
				// for check start point
				if ( ExistPnt(allLines[ii].sectionEdges[jj].start, pntNum) )
				{
					// if point is exist increase number of connect edge
					temp.pnt1 = pntNum;
					connectEdges[pntNum]++;
				}
				else
				{
					// if point not exist, add in and update index
					pntList.push_back( allLines[ii].sectionEdges[jj].start );
					temp.pnt1 = pntList.size()-1;
					connectEdges.push_back(1);
				}

				// for check end point
				if ( ExistPnt(allLines[ii].sectionEdges[jj].end, pntNum) )
				{
					// if point is exist increase number of connect edge
					temp.pnt2 = pntNum;
					connectEdges[pntNum]++;
				}
				else
				{
					// if point not exist, add in and update index
					pntList.push_back( allLines[ii].sectionEdges[jj].end );
					temp.pnt2 = pntList.size() - 1;
					connectEdges.push_back(1);
				}
			}
			// pre-process to convert type for connect
			BRepBuilderAPI_MakeWire MW;
			MW.Add( TopoDS::Edge(allLines[ii].sectionEdges[jj].edge->Shape()) );
			if ( MW.IsDone() )
			{
				temp.edge = MW.Wire();
				halfEdgeInfo.push_back(temp);
			}
		}
	}

	// build link info list
	for ( int ii = 0 ; ii < pntList.size() ; ii++ )
	{
		std::vector<int> temp = FindLinkVertex( ii );
		linkInfoList.push_back(temp);
	}
	// build unconnect point list
	std::vector<int> temp_list;
	for ( int ii = 0 ; ii < connectEdges.size() ; ii++ )
	{
		// don't save point without linke edge
		if ( connectEdges[ii] == 1 )
			temp_list.push_back(ii);
		else
			unconnectPnt.push_back(ii);
	}
	// remove useless edge
	for ( int ii = 0 ; ii < temp_list.size() ; ii++ )
	{
		for ( int jj = 0 ; jj < linkInfoList.size() ; jj++ )
		{
			std::vector<int>::iterator it = std::find(linkInfoList[jj].begin(), linkInfoList[jj].end(), temp_list[ii]);
			bool Find = it != linkInfoList[jj].end();
			if ( Find == true )
			{
				int pos = std::distance(linkInfoList[jj].begin(), it);
				linkInfoList[jj].erase(linkInfoList[jj].begin() + pos);
			}
		}
	}

}

bool COpenCascadeCtrl::ExistPnt(gp_Pnt checkPnt, int &pntNum)
{
	// check exist
	for ( int ii = 0 ; ii < pntList.size() ; ii++ )
	{
		if ( pntList[ii].IsEqual(checkPnt, 0.01) )
		{
			// retun index and true if exist
			pntNum = ii;
			return true;
		}
	}
	// return false if not exist
	return false;
}

std::vector<int> COpenCascadeCtrl::FindLinkVertex(int pointIndex)
{
	std::vector<int> linkList;
	for ( int ii = 0 ; ii < halfEdgeInfo.size() ; ii++ )
	{
		if ( halfEdgeInfo[ii].pnt1 == pointIndex )
		{
			// add point into list if pnt1 equal to input point
			linkList.push_back(halfEdgeInfo[ii].pnt2);
		}
		else if ( halfEdgeInfo[ii].pnt2 == pointIndex )
		{
			// add point into list if pnt2 equal to input point
			linkList.push_back(halfEdgeInfo[ii].pnt1);
		}
	}
	// return point list
	return linkList;
}

void COpenCascadeCtrl::SetInit()
{
	// find vertex with most edges link
	int num = 0;
	int index = -1;
	for ( int ii = 0 ; ii < linkInfoList.size() ; ii++ )
	{
		if ( linkInfoList[ii].size() > num )
		{
			num = linkInfoList[ii].size();
			index = ii;
		}
	}

	// set the vertex with most edges to be the first root 
	std::vector<int> temp;
	temp.push_back(index);
	allTravel.push_back(temp);
	ResetUnconnectPntList(index);
}

bool COpenCascadeCtrl::Travel(bool checkClose)
{
	// initial variable
	int addRoot = 0;
	int rootSize = allTravel.size();
	int loop_index = 0;
	int vector_index = 0;

	while ( loop_index < rootSize )
	{
		// get root vertex and edges
		int vertex_index = allTravel[vector_index][allTravel[vector_index].size() - 1];
		std::vector<int> next_vertex = linkInfoList[vertex_index];
		// remove previous link point
		int test_value = allTravel[vector_index].size()-2;
		if ( test_value >= 0 )
		{ CheckVertex(allTravel[vector_index][allTravel[vector_index].size() - 2], next_vertex); }
		else
		{ CheckVertex(-1, next_vertex); }
		// add new branch by vertex
		addRoot = addRoot + next_vertex.size();
		AddBranch(vector_index, next_vertex);
		// setting variables
		vector_index = vector_index + next_vertex.size();
		loop_index++;
	}
	// check close if over 3 level
	if ( checkClose == true )
	{ CheckClose(); }
	// return quantity of new branch
	return addRoot;
}

void COpenCascadeCtrl::CheckVertex(int index, std::vector<int> &input_vector)
{
	// check don't pick the previous link point
	for ( int ii = 0 ; ii < input_vector.size() ; ii++ )
	{
		if ( input_vector[ii] == index )
			input_vector.erase(input_vector.begin() + ii);
	}
}

void COpenCascadeCtrl::AddBranch(int index, std::vector<int> new_branch)
{
	// add new branchs by vertex
	int size = new_branch.size();
	int add_index = index;
	for ( int ii = 0 ; ii < size ; ii++ )
	{
		// add new branch
		if ( ii < size-1 )
		{
			allTravel.insert(allTravel.begin() + index, allTravel[index]);
			index++;
		}
		allTravel[add_index + ii].push_back(new_branch[ii]);
		// check and remove point be used
		ResetUnconnectPntList(new_branch[ii]);
	}
}

void COpenCascadeCtrl::CheckClose()
{
	// call IsClose function check if path is closed
	for ( int ii = 0 ; ii < allTravel.size() ; ii++ )
	{
		if ( IsClose(allTravel[ii]) )
		{
			closeTravel.push_back(allTravel[ii]);
			allTravel.erase(allTravel.begin() + ii);
			ii--;
		}
	}
}

bool COpenCascadeCtrl::IsClose(std::vector<int> &check_vector)
{
	// return true if the new vertex can make close wire
	for ( int ii = 0 ; ii < check_vector.size() - 1 ; ii++ )
	{
		if ( check_vector[ii] == check_vector[check_vector.size() - 1] )
		{
			check_vector.erase(check_vector.begin(), check_vector.begin() + ii);

			return true;
		}
	}

	return false;
}

void COpenCascadeCtrl::CompareTravel()
{
	for ( int ii = 0 ; ii < closeTravel.size() ; ii++ )
	{
		for ( int jj = ii + 1 ; jj < closeTravel.size() ; jj++ )
		{
			// compare  if path with same size
			if ( closeTravel[ii].size() == closeTravel[jj].size() )
			{
				// call IsEqual to check if path is equal
				if  (IsEqual(ii, jj) )
				{
					// remove
					closeTravel.erase(closeTravel.begin() + jj);
					jj--;
				}
			}
		}
	}
}

bool COpenCascadeCtrl::IsEqual(int index1, int index2)
{
	// sort index
	std::vector<int> test1 = closeTravel[index1];
	std::sort(test1.begin(), test1.end());
	std::vector<int> test2 = closeTravel[index2];
	std::sort(test2.begin(), test2.end());
	// return compare result
	return std::equal ( test1.begin(), test1.end(), test2.begin() );
}

void COpenCascadeCtrl::GetLinkEdges()
{
	// copy closeTravel to allTravel
	// and save edge info in closeTravel
	allTravel.clear();
	allTravel = closeTravel;
	closeTravel.clear();
	// call VertexToEdges to get all edges
	for ( int ii = 0 ; ii < allTravel.size() ; ii++ )
	{
		closeTravel.push_back( VertexToEdges(allTravel[ii]) );
	}
}

std::vector<int> COpenCascadeCtrl::VertexToEdges(std::vector<int> vertexs)
{
	// call GetEdge in loop to get each edge name
	std::vector<int> edges;
	for ( int ii = 1 ; ii < vertexs.size() ; ii++ )
	{
		edges.push_back( GetEdge(vertexs[ii-1], vertexs[ii]) );
	}

	return edges;
}


int COpenCascadeCtrl::GetEdge(int index1, int index2)
{
	// return edgename by start and end vertex
	for ( int ii = 0 ; ii < halfEdgeInfo.size() ; ii++ )
	{
		if ( ( halfEdgeInfo[ii].pnt1 == index1 && halfEdgeInfo[ii].pnt2 == index2 ) || ( halfEdgeInfo[ii].pnt2 == index1 && halfEdgeInfo[ii].pnt1 == index2 ) )
		{
			return ii;
		}
	}

	return -1;
}

void COpenCascadeCtrl::RemoveOverlap()
{
	// check if more than two face
	if ( allFaces.size() >= 2 )
	{
		for ( int ii = 0 ; ii < allFaces.size()-1 ; ii++ )
		{
			// get U&V parameter by face
			Standard_Real Umin, Umax, Vmin, Vmax;
			BRepTools::UVBounds(TopoDS::Face(allFaces[ii].faceFeature->Shape()), Umin, Umax, Vmin, Vmax);
			// get point by (Umax + Umin) / 2, (Vmax + Vmin) / 2
			gp_Pnt testPnt;
			Handle(Geom_Surface) surface1 = BRep_Tool::Surface( TopoDS::Face(allFaces[ii].faceFeature->Shape()) );
			surface1->D0( (Umax + Umin) / 2, (Vmax + Vmin) / 2, testPnt );
			// check all face
			for ( int jj = ii+1 ; jj < allFaces.size() ; jj++ )
			{
				BRepClass_FaceClassifier BCFC( TopoDS::Face(allFaces[jj].faceFeature->Shape()), testPnt, 0.1 );
				if (BCFC.State() == TopAbs_IN)
				{
					// remove if point on face
					allFaces.erase(allFaces.begin() + jj);
					closeTravel.erase(closeTravel.begin() + jj);
					jj--;
				}
			}
		}
	}
}

void COpenCascadeCtrl::ResetUnconnectPntList(int pntIndex)
{
	// remove point index be used
	std::vector<int>::iterator it = std::find(unconnectPnt.begin(), unconnectPnt.end(), pntIndex);
	bool Find = it != unconnectPnt.end();
	if ( Find == true )
	{
		int pos = std::distance(unconnectPnt.begin(), it);
		unconnectPnt.erase(unconnectPnt.begin() + pos);
	}
}


void COpenCascadeCtrl::test()
{
	for ( int ii = 0 ; ii < allLines.size() ; ii++ )
	{
		for ( int jj = 0 ; jj < allLines[ii].sectionEdges.size() ; jj++ )
		{
			TopoDS_Vertex vert1 = BRepBuilderAPI_MakeVertex(allLines[ii].sectionEdges[jj].start);
			Handle(AIS_Shape) ais_pnt1 = new AIS_Shape(vert1);
			TopoDS_Vertex vert2 = BRepBuilderAPI_MakeVertex(allLines[ii].sectionEdges[jj].end);
			Handle(AIS_Shape) ais_pnt2 = new AIS_Shape(vert2);
			m_pAISContext->Display( ais_pnt1 );
			m_pAISContext->Display( ais_pnt2 );
			m_pAISContext->Display( allLines[ii].sectionEdges[jj].edge );
			m_pAISContext->Remove( allLines[ii].sectionEdges[jj].edge );
			m_pAISContext->Remove( ais_pnt1 );
			m_pAISContext->Remove( ais_pnt2 );
		}
	}
}
