#ifndef RECONSTRUMAINWINDOW_H
#define RECONSTRUMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_ReconWindow.h"
#include "InitialTemplate.h"

#include <QtGui>
#include <QLineEdit>

#include "AxialFor3DView.h"
#include "CoronalFor3DView.h"
#include "SagittalFor3DView.h"
//#include "PanoView.h"
#include "ReconstruView.h"
#include "ProjectInfo.h"
#include "vtkProgress.h"

#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkMassProperties.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <ctkColorPickerButton.h>
#include <QPushButton>
#include "ActorObject.h"
#include <vtkActor.h>
#include <QGridLayout>
#include <vtkAbstractPicker.h>
#include <vtkWidgetEvent.h>
#include <vtkDistanceWidget.h>
#include <vtkHandleWidget.h>
#include "MeasureObject.h"
#include <vtkCommand.h>
#include "MoveCommand.h"
#include <vtkEvent.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkCutter.h>
#include <vtkClipPolyData.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkMatrix4x4.h>
#include <vtkContourFilter.h>
#include <vtkFillHolesFilter.h>
#include <vtkDensifyPolyData.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <vtkSphereSource.h>
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkPolyDataCollection.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkCylinder.h>
#include <vtkCylinderSource.h>
#include <vtkConnectivityFilter.h>

class ReconstruMainWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	ReconstruMainWindow(void);
public:
	~ReconstruMainWindow(void);

private:
	Ui::ReconWindow ui;

	//CAxialView* pAxialView;	
	////CPanoView* pPanoView;
	//CImplantSdiView* pImplantSdiView;
	ProjectInfo *pInfoDlg;
	vtkSmartPointer<vtkProgress> Progress;

	CAxialFor3DView* pAxialView;
	SagittalFor3DView* pSagittalView;
	CoronalFor3DView* pCoronalView;
	CReconstruView* pReconstruView;
	bool m_bTransferSucceed;

	InitialTemplate *iniTemplate;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourWidgetRep;
	vtkSmartPointer<vtkContourWidget> contourWidget;
	vtkSmartPointer<vtkPoints> temPts;
	double node[3];
	double punchRadius;

	double p1[3];	double p2[3];	double p3[3];
	int meaCount;
	int delMeaCount;
	QList<MeasureObject *> meaList;

	void ExportStl();
	void keyReleaseEvent(QKeyEvent *event);
	void initializePlaneWidget();
	
	void addModel();
	void addMeasure();

	void recut(vtkPolyData* mesh);

	//void SetActorListItem();
	//bool m_bViewMaximize;

public slots:
	void OnFileOpen();
	void OnInformation();
	void OnStlFileOpen();
	void OnSetViewMode();
	void OnShowSlice();
	void OnAxialSliceUpdate();
	void OnCoronalSliceUpdate();
	void OnSagittalSliceUpdate();
	void OnSetImageWindow(int nWindow);
	void OnSetImageLevel(int nLevel);
	void OnDoThreshold();
	void OnConnect();
	void OnConnectAxi();
	void OnConnectCor();
	void OnConnectSag();
	void OnDeleteModel();
	void OnDeleteMeasure();
	void OnReconstru();
	void OnExportStl();
	void OnExportToMainWindow();
	void OnCut();
	void advanceProgressBar(int nProgress);
	void advanceStatusBar(QString text);
	void OnShowWidget();
	void OnViewMaximize();
	void OnSetCurrentActorListItem();
	void SaveCurve();

	//measure
	void OnChangeDisFlag();
	void OnChangeAngFlag();
	void OnRecDistanceMeasure();
	void OnRecAngleMeasure();
	void On2DDistanceMeasure();
	void On2DAngleMeasure();

	//model table
	void OnUpdateModelVisib();
	void OnUpdateColorOfModel(QColor);
	void OnUpdateColorList();
	void OnUpdateOpacity(int);

	//measure table
	void OnUpdateMeasureVisib();
	void OnUpdateValue();
	void OnUpdate2DMeasure();

	//template generator
	void OnSelectPts();
	void OnContourWidget();
	void OnEndSelect();
	void OnDeleteSelect();
	void OnTemGenerate();
	void OnImportAxis();
	void OnPunching();
	void OnXMLTube();
	void OnUndoPunching();
	void XMLFileImport(vtkPoints* XMLPoints);
	void CreateTube(double x0[3],double x1[3], double Radius,vtkPolyData*OutputPD, bool IsCappingOn);

signals:
	void dataTransfer(vtkImageImport *id0,vtkPolyDataMapper *pd0,QString sPatientName);
	void shift();
};

#endif 
