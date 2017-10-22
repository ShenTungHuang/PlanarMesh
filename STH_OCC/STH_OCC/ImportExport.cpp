#include "StdAfx.h"
#include "ImportExport.h"

#include <vector>

#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Face.hxx>

#include "TColStd_SequenceOfAsciiString.hxx"
#include "TColStd_SequenceOfExtendedString.hxx"
#include "OSD_Timer.hxx"

#include "IGESControl_Reader.hxx"
#include "STEPControl_Controller.hxx"


#include <IGESControl_Controller.hxx>
#include <STEPCAFControl_Reader.hxx>


#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>


#include <STEPConstruct_Styles.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <STEPConstruct.hxx>


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

CImportExport::CImportExport(void)
{
}

CImportExport::~CImportExport(void)
{
}

//======================================================================
//=                                                                    =
//=                      BREP                                          =
//=                                                                    =
//======================================================================

void CImportExport::ReadBREP(const Handle(AIS_InteractiveContext)& anInteractiveContext)
{
    Handle(TopTools_HSequenceOfShape) aSequence = CImportExport::ReadBREP();
    for(int i=1;i<= aSequence->Length();i++) {
			anInteractiveContext->Display(new AIS_Shape(aSequence->Value(i)));
	}
}

Handle(TopTools_HSequenceOfShape) CImportExport::ReadBREP()
{
  setlocale(LC_ALL,"chinese-traditional");
  CFileDialog dlg(TRUE,
				  NULL,
				  NULL,
				  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				  "BREP Files (*.brep, *.rle)|*.brep; *.rle; |All Files (*.*)|*.*||", 
				  NULL );

  Handle(TopTools_HSequenceOfShape) aSequence= new TopTools_HSequenceOfShape();

  if (dlg.DoModal() == IDOK) 
  {
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	CString filename = dlg.GetPathName();
    TopoDS_Shape aShape;
    Standard_CString aFileName = (Standard_CString)(LPCTSTR)filename;
	Standard_Boolean result = ReadBREP(aFileName,aShape);
	if (result)
    {
 	    if (!BRepAlgo::IsValid(aShape))
	        MessageBox(0,"warning : The shape is not valid!!!","CasCade Warning",MB_ICONWARNING);
       aSequence->Append(aShape);
    }
    else 
	   MessageBox(0,"Error : The file is not read","CasCade Error",MB_ICONERROR);

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
  }

  return aSequence;
}

Standard_Boolean CImportExport::ReadBREP(const Standard_CString& aFileName, TopoDS_Shape& aShape)
{
	BRep_Builder aBuilder;
	Standard_Boolean result = BRepTools::Read(aShape,aFileName,aBuilder);
    return result;
}

//======================================================================

//======================================================================
//=                                                                    =
//=                      STEP                                          =
//=                                                                    =
//======================================================================

void CImportExport::ReadSTEP(const Handle(AIS_InteractiveContext)& anInteractiveContext)
{
    Handle(TopTools_HSequenceOfShape) aSequence = CImportExport::ReadSTEP();
	if (!aSequence.IsNull())
	{	
		int length = aSequence->Length();
		for(int i=1;i<= aSequence->Length();i++)
			anInteractiveContext->Display(new AIS_Shape(aSequence->Value(i)), Standard_False);
	}
}

Handle(TopTools_HSequenceOfShape) CImportExport::ReadSTEP()// not by reference --> the sequence is created here !!
{
  CFileDialog dlg(TRUE,
                  NULL,
                  NULL,
                  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                  "STEP Files (*.stp;*.step)|*.stp; *.step|All Files (*.*)|*.*||", 
                  NULL );

TCHAR tchBuf[80];

//CString CASROOTValue = ((GetEnvironmentVariable("CASROOT", tchBuf, 80) > 0) ? tchBuf : NULL); 
//CString initdir = (CASROOTValue + "\\..\\data\\step");

//dlg.m_ofn.lpstrInitialDir = initdir;
  
  Handle(TopTools_HSequenceOfShape) aSequence= new TopTools_HSequenceOfShape();
  if (dlg.DoModal() == IDOK) 
  {
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    CString C = dlg.GetPathName();
    Standard_CString aFileName = (Standard_CString)(LPCTSTR)C;
	IFSelect_ReturnStatus ReturnStatus = ReadSTEP(aFileName,aSequence);
    switch (ReturnStatus) 
    {
       case IFSelect_RetError :
           MessageBox(0,"Not a valid Step file","ERROR",MB_ICONWARNING);
       break;
       case IFSelect_RetFail :
           MessageBox(0,"Reading has failed","ERROR",MB_ICONWARNING);
       break;
       case IFSelect_RetVoid :
            MessageBox(0,"Nothing to transfer","ERROR",MB_ICONWARNING);
       break;
    }
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));       
  }
  return aSequence;
}

IFSelect_ReturnStatus CImportExport::ReadSTEP(const Standard_CString& aFileName,
                                              Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape
)
{
   aHSequenceOfShape->Clear();

  // create additional log file
  STEPControl_Reader aReader;
  IFSelect_ReturnStatus status = aReader.ReadFile(aFileName);
  if (status != IFSelect_RetDone)
    return status;

  aReader.WS()->MapReader()->SetTraceLevel(2); // increase default trace level

  Standard_Boolean failsonly = Standard_False;
  aReader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);
  
  // Root transfers
  Standard_Integer nbr = aReader.NbRootsForTransfer();
  aReader.PrintCheckTransfer (failsonly, IFSelect_ItemsByEntity);
  for ( Standard_Integer n = 1; n<=nbr; n++) {
    Standard_Boolean ok = aReader.TransferRoot(n);
  }

  // Collecting resulting entities
  Standard_Integer nbs = aReader.NbShapes();
  if (nbs == 0) {
    return IFSelect_RetVoid;
  }

 
  for (Standard_Integer i=1; i<=nbs; i++) {
    aHSequenceOfShape->Append(aReader.Shape(i));	
  }

  return status;
}
//======================================================================
//=                                                                    =
//=                      IGES                                          =
//=                                                                    =
//======================================================================



void CImportExport::ReadIGES(const Handle(AIS_InteractiveContext)& anInteractiveContext)
{
    Handle(TopTools_HSequenceOfShape) aSequence = CImportExport::ReadIGES();
    for(int i=1;i<= aSequence->Length();i++)
        anInteractiveContext->Display(new AIS_Shape(aSequence->Value(i)));

}

Handle(TopTools_HSequenceOfShape) CImportExport::ReadIGES()// not by reference --> the sequence is created here !!
{
  CFileDialog dlg(TRUE,
                  NULL,
                  NULL,
                  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                  "IGES Files (*.iges , *.igs)|*.iges; *.igs|All Files (*.*)|*.*||", 
                  NULL );

TCHAR tchBuf[80];

//CString CASROOTValue = ((GetEnvironmentVariable("CASROOT", tchBuf, 80) > 0) ? tchBuf : NULL); 
//CString initdir = (CASROOTValue + "\\..\\data\\iges");
//
//dlg.m_ofn.lpstrInitialDir = initdir;
  
  Handle(TopTools_HSequenceOfShape) aSequence = new TopTools_HSequenceOfShape();
  if (dlg.DoModal() == IDOK) 
  {
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    CString C = dlg.GetPathName();
    Standard_CString aFileName = (Standard_CString)(LPCTSTR)C;
    Standard_Integer status = ReadIGES(aFileName,aSequence);
	if (status != IFSelect_RetDone)
    {
		   MessageBox(0,"Error : The file is not read","CasCade Error",MB_ICONERROR);
	}
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
   }
  return aSequence;
}
//----------------------------------------------------------------------

Standard_Integer CImportExport::ReadIGES(const Standard_CString& aFileName,
                                         Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape)
{

    IGESControl_Reader Reader;

    Standard_Integer status = Reader.ReadFile(aFileName);

    if (status != IFSelect_RetDone) return status;
    Reader.TransferRoots();
    TopoDS_Shape aShape = Reader.OneShape();     
	aHSequenceOfShape->Append(aShape);

    return status;
}


/* ----- Save Step ----- */
Standard_Boolean CImportExport::SaveSTEP(const Standard_CString& aFileName,
                                              const Handle(TopTools_HSequenceOfShape)& aHSequenceOfShape)

{
	
    STEPControl_Writer aWriter;

	IFSelect_ReturnStatus status;
	for (Standard_Integer i=1;i<=aHSequenceOfShape->Length();i++)  
        {
			status =  aWriter.Transfer(aHSequenceOfShape->Value(i), STEPControl_AsIs);
            if ( status != IFSelect_RetDone ) return status;
        }     
	setlocale(LC_ALL,"chinese-traditional");
    status = aWriter.Write(aFileName);
    return 0;
}