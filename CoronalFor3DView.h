#ifndef CORONALFOR3DVIEW_H
#define CORONALFOR3DVIEW_H

#include <QObject>
#include "AxialFor3DView.h"

class CoronalFor3DView : public QObject
{
	Q_OBJECT

public:
	CoronalFor3DView();
	~CoronalFor3DView();

	void Initialize();
	void ImportImageData(vtkImageData *ImageData);
	void SetCamera();
	void ShowSlice();
	void DoThreshold(vtkImageData* imageData);
	void Connect(vtkImageData* imageData);

	void DrawAxialLine(double);
	void DrawSagittalLine(double);
	void DrawFrameRect(vtkRenderWindow*);

	vtkSmartPointer<vtkRenderWindow> renWin;
	vtkSmartPointer<vtkRenderer> ren;
	vtkSmartPointer<vtkRenderWindowInteractor> iren;
	vtkSmartPointer<vtkInteractorStyleImage> interactor;

	vtkSmartPointer<vtkWorldPointPicker> picker;
	vtkSmartPointer<vtkAnnotatePick> annotatePick;
	vtkSmartPointer<MoveCommand> mCommand;
	vtkSmartPointer<vtkTextMapper> textMapper;
	vtkSmartPointer<vtkActor2D> textActor;

	vtkSmartPointer<vtkTextMapper> SliceNumMapper, SliceZvalueMapper;
	vtkSmartPointer<vtkActor2D> SliceNumActor, SliceZvalueActor;

	vtkSmartPointer<vtkTransform> Transform;
	vtkSmartPointer<vtkMatrix4x4> Matrix;

	vtkSmartPointer<vtkImageConstantPad> greyPadder;	
	vtkSmartPointer<vtkWindowLevelLookupTable> greylut;
	vtkSmartPointer<vtkActor> greyActor;

	vtkSmartPointer<vtkImageConstantPad> segmentPadder;
	vtkSmartPointer<vtkActor> segmentOverlayActor;

	vtkSmartPointer<vtkImageConstantPad> thresholdPadder;
	vtkSmartPointer<vtkActor> thresholdOverlayActor;

	vtkSmartPointer<vtkActor> SagittalLineActor;
	vtkSmartPointer<vtkActor> AxialLineActor;

	vtkSmartPointer<vtkPoints> FramePts;
	vtkSmartPointer<vtkCellArray> FrameRect;
	vtkSmartPointer<vtkPolyData> FrameSelectRect;
	vtkSmartPointer<vtkPolyDataMapper2D> FrameRectMapper;
	vtkSmartPointer<vtkActor2D> FrameRectActor;

	double* m_fPixelSpacing, fScale;
	int m_nWidth;
	int m_nHeight;
	int m_nSliceSum,m_nSlice;
	int	m_nThreshMin;
	int	m_nThreshMax;

private:
	
};

#endif // CORONALFOR3DVIEW_H
