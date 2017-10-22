#include "stdafx.h"

#include <Standard_Version.hxx>

#pragma message ("============== Set libraries for Open CASCADE Technology " OCC_VERSION_STRING)

#pragma comment (lib , "TKBO.lib") // for BRepAlgoAPI_Section
#pragma comment (lib , "TKBool.lib")
#pragma comment (lib , "TKBRep.lib")
#pragma comment (lib , "TKernel.lib")
#pragma comment (lib , "TKG2d.lib")
#pragma comment (lib , "TKG3d.lib")
#pragma comment (lib , "TKMath.lib")
#pragma comment (lib , "TKService.lib")
#pragma comment (lib , "TKV3d.lib")
#pragma comment (lib , "TKBO.lib")
#pragma comment (lib , "TKIGES.lib")

//#pragma comment (lib , "IK_LIB.lib")

//ImportExport
#pragma comment (lib , "PTKernel.lib")
#pragma comment (lib , "TKXSBase.lib")
#pragma comment (lib , "TKStep.lib")
#pragma comment (lib , "TKSTEPAttr.lib")  // TopoDS_Shape S = STEPConstruct::FindShape(aStyles.TransientProcess(), style->Item());
#pragma comment (lib , "TKSTEPBase.lib")  // Handle(StepShape_ShapeRepresentation) aCurrentSR = Handle(StepShape_ShapeRepresentation)::DownCast(aStyleCntxSlct.Representation());
#pragma comment (lib , "TKXDESTEP.lib")   //STEPCAFControl_Reader
#pragma comment (lib , "TKXCAF.lib")
#pragma comment (lib , "TKXCAFSchema.lib")
#pragma comment (lib , "TKLCAF.lib")

#pragma comment (lib , "TKTopAlgo.lib")

#pragma comment (lib , "TKGeomBase.lib")
#pragma comment (lib , "TKGeomAlgo.lib") 

//OCAF
#pragma comment (lib , "TKCAF.lib")
#pragma comment (lib , "TKCDF.lib")
#pragma comment (lib , "TKXSBase.lib")
#pragma comment (lib , "TKLCAF.lib")

#pragma comment (lib , "TKPrim.lib")

#pragma comment (lib , "TKFillet.lib")
#pragma comment (lib , "TKOffset.lib")


// For Colision
//#pragma comment (lib , "TKColBase.lib")
//#pragma comment (lib , "TKColProxy.lib")
//#pragma comment (lib , "TKOCCLicense.lib")