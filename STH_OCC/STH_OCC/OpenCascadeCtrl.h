#pragma once

#include <vector>

// C3DWnd window
enum View3D_CurrentAction
{
	CurAction3d_Nothing,
	CurAction3d_DynamicZooming,
	CurAction3d_WindowZooming,
	CurAction3d_DynamicPanning,
	CurAction3d_GlobalPanning,
	CurAction3d_DynamicRotation,
};

struct SectionEdge
{
	Handle(AIS_Shape) edge;
	gp_Pnt start;
	gp_Pnt end;
};

struct LineFeature
{
	Handle(AIS_Shape) startPoint;
	Handle(AIS_Shape) endPoint;
	Handle(AIS_Shape) lineFeature;
	gp_Pnt startPnt;
	gp_Pnt endPnt;
	double lineLength;
	std::vector<SectionEdge> sectionEdges;
};

struct FaceFeature
{
	Handle(AIS_Shape) faceFeature;
	double perimeter;
	double area;
};

struct SectionInfo
{
	gp_Pnt pnt;
	double dist;
};

struct DragLineInfo
{
	Handle(AIS_Shape) newLine;
	gp_Pnt fixPnt;
	int lineIndex;
	bool firstPnt;
};

struct HalfEdge
{
	TopoDS_Wire edge;
	int pnt1;
	int pnt2;
};

#define CASCADESHORTCUTKEY MK_CONTROL

class COpenCascadeCtrl : public CStatic
{
    DECLARE_DYNAMIC(COpenCascadeCtrl)

public:
    COpenCascadeCtrl();
    virtual ~COpenCascadeCtrl();

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	Handle(V3d_View) m_pView;
	Handle(V3d_Viewer) m_pViewer;
	Handle(WNT_Window) m_pWNTWindow;
	Handle(AIS_InteractiveContext) m_pAISContext;
	Handle(Graphic3d_WNTGraphicDevice) m_pGraphicDevice;

	//Standard_Boolean     m_DegenerateModeIsOn;
	View3D_CurrentAction m_CurrentMode;
	Standard_Real m_rWidth, m_rHeight;
    Standard_Integer myXmax, myYmax;

public:
	void Clear();
	void Right();
	void Top();
	void Left();
	void Front();
	void Bottom();
	void Back();
	void ZoolAll();
    void Axo();

public:
	bool tappedLineButton;
	int selectType, selectIndex;
	bool hasFirstPoint, selectEdgeMode, selectFaceMode, dragMode;
	gp_Pnt startPoint, endPoint, dragPoint;
	Handle(AIS_Shape) ais_Line, ais_Pnt1, ais_Pnt2;
	std::vector<LineFeature> allLines;
	std::vector<DragLineInfo> lineOfPoint;
	// for make face
	std::vector<HalfEdge> halfEdgeInfo;
	std::vector<gp_Pnt> pntList;
	std::vector<int> connectEdges;
	std::vector<int> unconnectPnt;
	std::vector<FaceFeature> allFaces;
	std::vector<std::vector<int>> linkInfoList;
	std::vector<std::vector<int>> allTravel;
	std::vector<std::vector<int>> closeTravel;

	gp_Pnt ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView);
	void PlotLine(CPoint point);
	gp_Pnt GetCurrentPoint(CPoint point);
	double GetEdgeLength(TopoDS_Edge edge);
	double GetFaceArea(TopoDS_Shape shape);
	double GetFacePerimeter(TopoDS_Wire wire);
	void SetandPlotPoint(int number, CPoint point);
	void SearchEdge(TopoDS_Edge edge);
	void SearchPoint(TopoDS_Vertex vertex);
	void SearchFace(TopoDS_Face face);
	void ChackEdgeIntersection();
	void SortPnt(std::vector<SectionInfo> &SectionPnt);
	void DragLine(CPoint point);
	void SaveSectionEdge(int index1, int index2, gp_Pnt p1, gp_Pnt p2);
	void ChackAllEdgeIntersection();
	void PlotFaces();
	void BuildFaces();
	void BuildPntList();
	bool ExistPnt(gp_Pnt checkPnt, int &pntNum);
	std::vector<int> FindLinkVertex(int pointIndex);

	void SetInit();
	bool Travel(bool checkClose);
	void CheckVertex(int index, std::vector<int> &input_vector);
	void AddBranch(int index, std::vector<int> new_branch);
	void CheckClose();
	bool IsClose(std::vector<int> &check_vector);
	void CompareTravel();
	bool IsEqual(int index1, int index2);
	void GetLinkEdges();
	std::vector<int> VertexToEdges(std::vector<int> vertexs);
	int GetEdge(int index1, int index2);
	void RemoveOverlap();
	void ResetUnconnectPntList(int pntIndex);

	void test();
};
