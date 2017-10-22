#pragma once


#include <Storage_Error.hxx>
#include <MgtBRep_TriangleMode.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <STEPControl_StepModelType.hxx>
#include <Quantity_HArray1OfColor.hxx>
#include <TColStd_HArray1OfReal.hxx>

#include <Standard_Macro.hxx>
#include <vector>
#include <AIS_Shape.hxx>




class CImportExport
{
public:
	CImportExport(void);
	~CImportExport(void);

//BRep
//======================================================================
static void ReadBREP(const Handle(AIS_InteractiveContext)& anInteractiveContext);
static Handle(TopTools_HSequenceOfShape) ReadBREP();
static Standard_Boolean ReadBREP(const Standard_CString& aFileName, TopoDS_Shape& aShape);

//Step
//======================================================================
static void ReadSTEP(const Handle(AIS_InteractiveContext)& anInteractiveContext);
static Handle(TopTools_HSequenceOfShape) ReadSTEP();
static IFSelect_ReturnStatus ReadSTEP(const Standard_CString& aFileName,
									        Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape);
/* Read With Color */
/*
static IFSelect_ReturnStatus ReadSTEP(const Standard_CString& aFileName,
									  std::vector<Handle(AIS_Shape)>& aHSequenceOfAIS_Shape);
*/
//Iges
//======================================================================
static void ReadIGES(const Handle(AIS_InteractiveContext)& anInteractiveContext);
static Handle(TopTools_HSequenceOfShape) ReadIGES(); // not by reference --> the sequence is created here !!
static Standard_Integer ReadIGES(const Standard_CString& aFileName,
                                     Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape);

/* ----- Save Step ----- */
static Standard_Boolean SaveSTEP(const Standard_CString& aFileName,
                                              const Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape);
};
