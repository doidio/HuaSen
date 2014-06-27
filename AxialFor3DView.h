#ifndef AXIALFOR3DVIEW_H
#define AXIALFOR3DVIEW_H

#include <QApplication>
#include <QtGui>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkDICOMImageReader.h>
#include <vtkImageConstantPad.h>
#include <vtkPlaneSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkWindowLevelLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkTexture.h>
#include <vtkActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkWorldPointPicker.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkCamera.h>
#include <vtkImageThreshold.h>
#include <vtkProperty.h>
#include <vtkImageSeedConnectivity.h>
#include <vtkWindowedSincPolyDataFilter.h>

#include <vtkContourFilter.h>
#include <vtkImageMarchingCubes.h>
#include <vtkMarchingCubes.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkDecimatePro.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkStripper.h>
#include <vtkLODActor.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkCellPicker.h>
#include <vtkTextMapper.h>

#include <vtkSTLWriter.h>
#include <vtkTriangleFilter.h>
#include <vtkImageReslice.h>

#include <vtkImageDilateErode3D.h>


#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkVTKImageExport.h> 

#include <itkGDCMImageIO.h> 
#include <itkImageSeriesReader.h> 
#include <itkGDCMSeriesFileNames.h> 

#include <itkImageIOBase.h> 
#include <itkImageIORegion.h> 

#include <vtkImageImport.h>
#include <vtkXMLImageDataWriter.h>

#include "DlgSelectDirectory.h"

#include "vtkAnnotatePick.h"
#include "vtkProgress.h"

#include "vtkImageToStructuredPoints.h"
#include <vtkGeometryFilter.h>
#include <vtkImageChangeInformation.h>
#include <vtkSmartPointer.h>

#include "ActorObject.h"

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAngleRepresentation3D.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkAnglewidget.h>
#include "MoveCommand.h"
#include "QtSignal.h"
#include <vtkMath.h>
#include <time.h>

//#include "volumereader.h"
//#include "dicomloader.h"
//#include "visualizationloadedresult.h"

class CAxialFor3DView :
	public QObject
{
	Q_OBJECT

public:
	CAxialFor3DView(void);
public:
	~CAxialFor3DView(void);

public:
	vtkRenderWindow *renWin;
	vtkRenderer *ren;
	vtkRenderWindowInteractor *iren;

	QList<QActorObject *> m_Actorlist;
	int actorCount;

	double *m_fPixelSpacing, fScale;
	int m_nWidth;
	int m_nHeight;
	int m_nSliceSum,m_nSlice;

	int		m_nThreshMin;
	int		m_nThreshMax;
	
	vtkSmartPointer<vtkInteractorStyleImage> interactor;
	
	vtkWorldPointPicker *picker;
	vtkAnnotatePick *annotatePick;
	vtkSmartPointer<MoveCommand> mCommand;
	vtkTextMapper *textMapper;
	vtkActor2D *textActor;

	vtkTextMapper *SliceNumMapper,*SliceZvalueMapper;
	vtkActor2D *SliceNumActor,*SliceZvalueActor;

	vtkSmartPointer<vtkTransform> Transform;
	vtkSmartPointer<vtkMatrix4x4> Matrix;

	vtkSmartPointer<vtkImageConstantPad> greyPadder;	
	vtkSmartPointer<vtkWindowLevelLookupTable> greylut;
	vtkSmartPointer<vtkActor> greyActor;

	vtkSmartPointer<vtkImageConstantPad> segmentPadder;
	vtkSmartPointer<vtkActor> segmentOverlayActor;

	vtkSmartPointer<vtkImageConstantPad> thresholdPadder;
	vtkSmartPointer<vtkActor> thresholdOverlayActor;	
	
	vtkSmartPointer<vtkImageSeedConnectivity> connect;

	vtkSmartPointer<vtkActor> CoronalLineActor;
	vtkSmartPointer<vtkActor> SagittalLineActor;

	vtkSmartPointer<vtkPoints> FramePts;
	vtkSmartPointer<vtkCellArray> FrameRect;
	vtkSmartPointer<vtkPolyData> FrameSelectRect;
	vtkSmartPointer<vtkPolyDataMapper2D> FrameRectMapper;
	vtkSmartPointer<vtkActor2D> FrameRectActor;
	//vtkSmartPointer<vtkImageDilateErode3D> DilateErode3D;
	//vtkSmartPointer<vtkMarchingCubes> iso;
	//vtkSmartPointer<vtkDecimatePro> decimator;
	//vtkSmartPointer<vtkImageConstantPad> isoPad;
	//vtkSmartPointer<vtkWindowedSincPolyDataFilter> smootherSinc;
	//vtkSmartPointer<vtkTransformPolyDataFilter> transformer;
	vtkSmartPointer<vtkImageToStructuredPoints> imageToStructuredPoints;

	vtkSmartPointer<vtkProgress> ProgressCommand;

	typedef itk::Image<short, 3> ImageType;	
	typedef ImageType::Pointer ImagePointer ;

	typedef itk::VTKImageExport<ImageType> VTKExportType;
	typedef itk::SmartPointer<VTKExportType> VTKExportPointer;
	VTKExportPointer itkVTKExporter;
	VTKExportPointer itkVTKResampleExporter;

	vtkImageImport *VTKImporter;
	vtkImageImport *VTKResampleImporter;
	void itk2vtk(VTKExportPointer itkVTKExporterPara,vtkImageImport *VTKImporterPara);

	typedef itk::MemberCommand< CAxialFor3DView >  CommandType;
	CommandType::Pointer         m_CommandObserver;
	void ProgressUpdateFunc( itk::Object * caller, const itk::EventObject & event );

	int m_nProgress;
	QString m_sPatientName;

	ImageType::Pointer ROIImage;
	typedef itk::ImageSeriesReader< ImageType >     SeriesReaderType;
	SeriesReaderType::Pointer seriesReader;

	bool m_bCropImage;

signals:
    void ProgressUpdate(int nProgress);

public:
	void Initialize();
	bool FileOpen(QString directory);
	void ShowSlice();
	void ImportImageData(vtkImageData *ImageData);
	void SetCamera();
	void DoThreshold();
	void Connect(int,int,int);
	void Reconstru();
	//void ExportStl();
	void ReverseSlice();
	void cropImage(ImagePointer source , ImagePointer target,int nStartSlice, int nEndSlice);
	void SetActorColor(int i,QActorObject* pActorObject);

	void DrawSagittalLine(double);
	void DrawCoronalLine(double);
	void DrawFrameRect(vtkRenderWindow*);

	
	//void ShowSlice();
};

#endif
