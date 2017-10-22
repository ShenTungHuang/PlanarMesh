// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的
// 專案專用 Include 檔案

#if !defined(AFX_STDAFX_H__4A96A446_055A_11D4_A60A_0800369C8A03__INCLUDED_)
#define AFX_STDAFX_H__4A96A446_055A_11D4_A60A_0800369C8A03__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _CRT_SECURE_NO_DEPRECATE

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Open CASCADE Technology :


#if !defined(WNT)
   #error WNT precompiler directive is mandatory for Open CASCADE Technology
#endif
#if !defined(CSFDB)
   #error CSFDB precompiler directive is mandatory for Open CASCADE Technology
#endif

#ifdef MAINWIN
# define false FALSE
# define true TRUE
#endif

#pragma warning(  disable : 4244 )        // Issue warning 4244
#include <Standard_ShortReal.hxx>
#pragma warning(  default : 4244 )        // Issue warning 4244

//AIS
#include <AIS_Circle.hxx>
#include <AIS_DiameterDimension.hxx>
#include <AIS_Drawer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_Plane.hxx>
#include <AIS_Point.hxx>
#include <AIS_SequenceOfInteractive.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Trihedron.hxx>
//Aspect
#include <Aspect_Background.hxx>
#include <Aspect_ColorMapEntry.hxx>
#include <Aspect_GenericColorMap.hxx>
#include <Aspect_LineStyle.hxx>
#include <Aspect_MarkMap.hxx>
#include <Aspect_TypeMap.hxx>
#include <Aspect_TypeMapEntry.hxx>
#include <Aspect_TypeOfLine.hxx>
#include <Aspect_TypeOfMarker.hxx>
#include <Aspect_TypeOfText.hxx>
#include <Aspect_WidthMapEntry.hxx>
#include <Aspect_WidthOfLine.hxx>
#include <Aspect_Window.hxx>
//Bnd
#include <Bnd_Box.hxx>
#include <Bnd_Box2d.hxx>
//BndLib
#include <BndLib_Add2dCurve.hxx>
//BRep
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
//BRepAlgo
#include <BRepAdaptor_CompCurve.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAlgo.hxx>
#include <BRepAlgo_Fuse.hxx>
#include <BRepAlgo_Section.hxx>
//BRepAlgoAPI
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBndLib.hxx>
//BRepBuilderAPI
#include <BRepBuilderAPI.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_Transform.hxx>
//BRepClass
#include <BRepClass_FaceClassifier.hxx>
//BRepTools
#include <BRepTools.hxx>
//BRepPrimAPI
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
//BRepClass3d
#include <BRepClass3d_SolidClassifier.hxx>
//BRepExtrema
#include <BRepExtrema_DistShapeShape.hxx>
//BRepFeat
#include <BRepFeat_MakeCylindricalHole.hxx>
#include <BRepFeat_SplitShape.hxx>
//BRepFilletAPI
#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepGProp.hxx>
//BRepLProp
#include <BRepLProp_SLProps.hxx>
//BRepOffsetAPI
#include <BRepOffsetAPI_DraftAngle.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
//BRepTools
#include <BRepTools_WireExplorer.hxx>
//DBRep
#include <DBRep.hxx>
//DsgPrs
#include <DsgPrs_LengthPresentation.hxx>
//DrawTrSurf
#include <DrawTrSurf.hxx>
//GC
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <GccEnt.hxx>
//GCE2d
#include <GCE2d_MakeCircle.hxx>
#include <GCE2d_MakeSegment.hxx>
//GCPnts
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_TangentialDeflection.hxx>
//Geom
#include <GeomAPI.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Point.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Surface.hxx>
//Geom2d
#include <Geom2d_BezierCurve.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_Circle.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_TrimmedCurve.hxx>
//Geom2dAdaptor
#include <Geom2dAdaptor_Curve.hxx>
//Geom2dGcc
#include <Geom2dGcc_QualifiedCurve.hxx>
#include <Geom2dGcc_Circ2d2TanRad.hxx>
#include <Geom2dGcc_Lin2d2Tan.hxx>
//GeomLProp
#include <GeomLProp_SLProps.hxx>
//GeomAbs
#include <GeomAbs_CurveType.hxx>
// GeomAPI
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
//GeomAdaptor
#include <GeomAdaptor_Curve.hxx>
//GeomPlate
#include <GeomPlate_BuildAveragePlane.hxx>
//GeomTools
#include <GeomTools_Curve2dSet.hxx>
//Graphic3d
#include <Graphic3d_AspectFillArea3d.hxx>
#include <Graphic3d_WNTGraphicDevice.hxx>
//gp
#include <gp_Ax2d.hxx>
#include <gp_Circ.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <gp_Vec2d.hxx>
#include <GProp_GProps.hxx>
//Graphic3d
#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
//MMgt
#include <MMgt_TShared.hxx>
//NCollection
#include <NCollection_List.hxx>
//OSD
#include <OSD_Environment.hxx>

#include <Precision.hxx>
//Prs3d
#include <Prs3d_IsoAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_Projector.hxx>
#include <Prs3d_Text.hxx>
#include <PrsMgr_PresentationManager2d.hxx>
//Quantity
#include <Quantity_Factor.hxx>
#include <Quantity_Length.hxx>
#include <Quantity_NameOfColor.hxx>
#include <Quantity_PhysicalQuantity.hxx>
#include <Quantity_PlaneAngle.hxx>
#include <Quantity_TypeOfColor.hxx>
//SelectBasics
#include <SelectBasics_BasicTool.hxx>
#include <SelectBasics_ListOfBox2d.hxx>
//SelectMgr
#include <SelectMgr_EntityOwner.hxx>
#include <SelectMgr_SelectableObject.hxx>
#include <SelectMgr_Selection.hxx>
#include <SelectMgr_SelectionManager.hxx>
#include <ShapeSchema.hxx> 
//Standard
#include <Standard_Boolean.hxx>
#include <Standard_CString.hxx>
#include <Standard_DefineHandle.hxx> 
#include <Standard_ErrorHandler.hxx>
#include <Standard_Integer.hxx>
#include <Standard_IStream.hxx>
#include <Standard_Macro.hxx>
#include <Standard_NotImplemented.hxx>
#include <Standard_OStream.hxx>
#include <Standard_PrimitiveTypes.hxx>
#include <Standard_Real.hxx>
#include <Standard_Transient_proto.hxx>
//StdPrs
#include <StdPrs_Curve.hxx>
#include <StdPrs_PoleCurve.hxx>
#include <StdPrs_Point.hxx>
#include <StdSelect_SensitiveText2d.hxx>
#include <StdSelect_TextProjector2d.hxx>
#include <StdSelect_ViewerSelector2d.hxx>
//TColgp
#include <TColgp_Array1OfPnt2d.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_HArray1OfPnt2d.hxx>
#include <TColgp_SequenceOfVec.hxx>
//TCollection
#include <TCollection_AsciiString.hxx>
//TColStd
#include <TColStd_HSequenceOfTransient.hxx>
#include <TColStd_MapIteratorOfMapOfTransient.hxx>
#include <TColStd_MapOfTransient.hxx>
//TopExp
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
//TopOpeBRepTool
#include <TopOpeBRepTool.hxx>
//TopoDS
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_ListIteratorOfListOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>
//TopTools
#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

#include <UnitsAPI.hxx>
//V3d
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
//WNT
#include <WNT_Window.hxx>

#include <ElSLib.hxx>

#include <ProjLib.hxx>

#include <TDataStd_Integer.hxx>

#include <TDocStd_Document.hxx>
//TopTools
#include <TopTools_ListOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>

#include <GC_MakePlane.hxx>

#include <Visual3d_Layer.hxx>

#include <IntCurvesFace_Intersector.hxx>

// specific IGES, STEP or DXF
#include <XSControl_WorkSession.hxx>
#include <IFSelect_ReturnStatus.hxx>

// specific IGES STEP
//6.2.1:#include <Interface_DT.hxx>
// specific IGES, STEP or DXF
#include <Interface_Static.hxx>

// specific STEP
#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>
// specific IGES
#include <Interface_InterfaceModel.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESToBRep_Actor.hxx>
#include <IGESToBRep_Reader.hxx>

#include <V2d_View.hxx>

#include <WNT_WDriver.hxx>
#include <wnt_wclass.hxx>

//#endif // !defined(AFX_STDAFX_H__E42A3367_2F78_11D6_9EB9_0010A40C9085__INCLUDED_)
#include "ImportExport.h"

#endif // !defined(AFX_STDAFX_H__4A96A446_055A_11D4_A60A_0800369C8A03__INCLUDED_)
