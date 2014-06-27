#ifndef RECONSTRUVIEW_H
#define RECONSTRUVIEW_H

#include <QApplication>
#include <QtGui>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkTexture.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkWorldPointPicker.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkPlaneWidget.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include "vtkAnnotatePick.h"
#include "interactorStyleTrackballActor.h"

#include <vtkSmartPointer.h>

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

class vtkTPWCallback : public vtkCommand
{
public:
  static vtkTPWCallback *New() 
    { return new vtkTPWCallback; }
  virtual void Execute(vtkObject *, unsigned long, void*)
    {
      //vtkPlaneWidget *planeWidget = reinterpret_cast<vtkPlaneWidget*>(caller);
      //planeWidget->GetPolyData(this->PolyData);
      //this->Actor->VisibilityOn();
    }
  //vtkTPWCallback():PolyData(0),Actor(0) {}
  //vtkPolyData *PolyData;
  //vtkActor *Actor;
};

class CReconstruView :
	public QObject
{
	Q_OBJECT

public:
	CReconstruView(void);
public:
	~CReconstruView(void);

public:
	vtkRenderWindow *renWin;
	vtkRenderer *ren;
	vtkRenderWindowInteractor *iren;
	//vtkInteractorStyleSwitch *StyleSwitch;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> StyleTrackballCamera;
	//vtkInteractorStyleTrackballActor *StyleTrackballActor;
	interactorStyleTrackballActor *StyleTrackballActor;
	bool m_bStyleTrackballCamera;

	vtkWorldPointPicker *picker;
	vtkSmartPointer<MoveCommand> mCommand;
	vtkAnnotatePick *annotatePick;
	vtkTextMapper *textMapper;
	vtkActor2D *textActor;

	vtkPlaneWidget *planeWidget;
	//vtkTPWCallback *myCallback;

	void Initialize();


};

#endif