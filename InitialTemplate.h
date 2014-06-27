#ifndef __InitialTemplate_h
#define __InitialTemplate_h

#include "vtkProgress.h"

#include "vtkPoints.h"
#include "vtkPolyData.h"

#include "vtkSTLWriter.h"
#include "SelectPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkMatrix4x4.h"
#include "vtkCollisionDetectionFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkStripper.h"
#include "vtkCellArray.h"
#include "vtkPolyDataNormals.h"
#include "vtkDataArray.h"
#include "vtkIdList.h"
#include "vtkAppendPolyData.h"
#include "vtkImplicitModeller.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkTriangleFilter.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "vtkLoopSubdivisionFilter.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkSmartPointer.h"
#include "vtkFeatureEdges.h"
#include "vtkModifiedBSPTree.h"
#include "vtkDenseArray.h"
#include "vtkTriangle.h"
#include "vtkDoubleArray.h"
#include "vtkLineSource.h"
#include "vtkTubeFilter.h"
#include "vtkLinearSubdivisionFilter.h"
#include "vtkCellData.h"
#include "vtkCellLocator.h"
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkDelaunay3D.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDecimatePro.h>
#include <vtkSTLWriter.h>
#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkSphereSource.h>

#include <QtGui>
#include <QtXML>
#include "vtkSaveCurveCommand.h"
#include <vtkOrientedGlyphContourRepresentation.h>
#include "QContourObject.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkWorldPointPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkContourWidget.h>

class InitialTemplate
{
public:
	InitialTemplate();
	InitialTemplate(vtkRenderWindow*,
					vtkRenderer*,
					vtkRenderWindowInteractor*,
					vtkInteractorStyleTrackballCamera*,
					vtkWorldPointPicker*);
	~InitialTemplate();

	vtkSmartPointer<vtkPolyData> m_Output;
	vtkSmartPointer<vtkPolyData> m_OutputIni;
	vtkSmartPointer<vtkPolyData> m_OutputFin;

	double thickness;
	double tubeRadius;
	vtkSmartPointer<vtkPoints> m_AxisXMLPoints;
	vtkSmartPointer<vtkProgress> Progress;

	vtkSmartPointer<vtkPoints> m_AllSubdivisionPts;
	vtkSmartPointer<vtkRenderWindow> renWin;
	vtkSmartPointer<vtkRenderer> ren;
	vtkSmartPointer<vtkRenderWindowInteractor> iren;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> StyleTrackballCamera;
	vtkSmartPointer<vtkWorldPointPicker> picker;
	vtkSmartPointer<vtkSaveCurveCommand> SaveCurveCommand;

	vtkSmartPointer<vtkDataArray>CellNormalArray;

	void Initialize();
	int GetInitialTemplate(vtkPolyData *inputMesh, vtkPoints *Pts);
	void GetFinalTemplate(vtkPoints *Pts);
	int GetFinalPlateWithPunching(double Radius, vtkPoints *Pts);

	void FixPoint(double *fTipPoint);
	void SaveCurrentCurve();
	void CloseContour();
	void OnUndoPoint();
	void UndoPunching();

	void ClearContours();
	void DeepCopy(InitialTemplate*);

protected:

private:
	QList<QContourObject *> *QContourObjectList;

	bool IsFirstPt;
	bool Multi_Angle;
	bool TurnOver;

	bool IsContourClosed;
	vtkSmartPointer<vtkActor> FirstUserPtActor;
	int IsFirstUserPt;

	vtkSmartPointer<vtkPolyData> m_Mesh;
	vtkSmartPointer<vtkPoints> m_Loop;
	
	vtkSmartPointer<vtkPolyData> m_innSurface;
	vtkSmartPointer<vtkPolyData> m_outSurface;
	vtkSmartPointer<vtkPolyData> m_wall;
	vtkSmartPointer<vtkPolyData> m_innSurf;
	vtkSmartPointer<vtkPolyData> m_outSurf;
	vtkSmartPointer<vtkPolyData> m_mesh;

	vtkSmartPointer<vtkPoints> AllControlPts;
	//vtkSmartPointer<vtkPoints>ControlPts;
	vtkSmartPointer<vtkPoints> ControlPts;
	vtkSmartPointer<vtkPoints> SplineFilterPoints;	
	vtkSmartPointer<vtkPoints> ConnectionControlPts;
	vtkSmartPointer<vtkPoints> ConnectionSplineFilterPoints;
	vtkSmartPointer<vtkPolyDataMapper> ControlPtsMapper;
	vtkSmartPointer<vtkPolyDataMapper> SubdivisionPtsMapper;	
	vtkSmartPointer<vtkPolyDataMapper> ConnectionSubdivisionPtsMapper;
	QContourObject *pContourObject;
	QContourObject *pConnectionContourObject;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> m_rep;

	int Clip(vtkPolyData* inputMesh,vtkPoints* points,vtkPolyData* outputMesh,
		vtkPolyData* outputMesh1,int Mode);
	void CollisionPoints(vtkPolyData* PD0,vtkPolyData* PD1,vtkPoints*CollisionPts);
	int OuterModelGeneration(vtkPolyData* inputPD,vtkPolyData* outputPD);
	int GetEdgePoints(vtkPolyData* inputPD,vtkPoints* EdgePts);
	void WallGeneration(vtkPoints* EdgePts0,
		vtkPoints* EdgePts1,vtkPolyData*outputPD);
	int GetIntersectionPoint(double p1[3], double p2[3], double x[3], vtkPolyData* PD);
	void CreateTube(double x0[3],double x1[3], double Radius,
		vtkPolyData*OutputPD, bool IsCappingOn);
	void Collision(vtkPolyData* PD0,vtkPolyData* PD1,vtkPolyData* OutputPD0,
										 vtkPolyData* OutputPD1, vtkPoints*CollisionPts, 
										 int Mode0, int Mode1, double x0[3], double x1[3]);
	int Punching(vtkPolyData*outSurf_Input, vtkPolyData*innSurf_Input, vtkPolyData*Mesh_Input, 
								double point[3], double Radius, vtkDataArray *CellNormalArray,
								vtkPolyData*outSurf_Output, vtkPolyData*innSurf_Output, vtkPolyData*Mesh_Output);
	void Project(vtkPolyData* inputMesh,vtkPoints* InitialPts,vtkPoints* ResultPts);

	void DrawContour(vtkPoints *Pts, int Mode);
	void DrawConnectionContour(vtkPoints *Pts);
	void GenerateContourWidget(vtkPoints*points, int Mode);
	void ConvertPt(double *worldPoint,double *result);
	void ExtensionPt(double x[3], double y[3]);
};
#endif