#include "ReconstruView.h"

CReconstruView::CReconstruView(void)
{
	renWin= vtkRenderWindow::New();	
	ren = vtkRenderer::New();
	//StyleSwitch=vtkInteractorStyleSwitch::New();
	StyleTrackballCamera = vtkInteractorStyleTrackballCamera::New();
	//StyleTrackballActor = vtkInteractorStyleTrackballActor::New();
	StyleTrackballActor = interactorStyleTrackballActor::New();
	iren=vtkRenderWindowInteractor::New();
}

void CReconstruView::Initialize() 
{
	//picker=vtkCellPicker::New();
	picker=vtkWorldPointPicker::New();
	
	iren->SetRenderWindow(renWin);
	iren->SetPicker(picker);
	iren->Initialize();

	//iren->SetInteractorStyle(StyleSwitch);
	iren->SetInteractorStyle(StyleTrackballCamera);
	m_bStyleTrackballCamera=true;

	//stlPolyData=vtkPolyData::New() ;
	//clipActor=vtkActor::New();
	//clipLeftActor=vtkActor::New();
		
	textMapper=vtkTextMapper::New();
	textActor=vtkActor2D::New();  
	annotatePick=vtkAnnotatePick::New();
	iren->AddObserver(vtkCommand::EndPickEvent,annotatePick);
	mCommand = vtkSmartPointer<MoveCommand>::New();
	mCommand->qSignal = new QtSignal();


	annotatePick->textMapper=textMapper;
	annotatePick->textActor=textActor;	

	textActor->SetMapper(textMapper);
	ren->AddActor2D(textActor);

	ren->SetBackground(109.0/255.0,90.0/255.0,205.0/255.0);

	//myCallback=vtkTPWCallback::New();

	planeWidget=vtkPlaneWidget::New();	
	planeWidget->SetInteractor(iren);

	//planeWidget->SetInput(stlActor->GetMapper()->GetInput());
	//planeWidget->NormalToXAxisOn();
	//planeWidget->SetResolution(20);
	//planeWidget->SetPlaceFactor(0.25);
	//planeWidget->SetRepresentationToSurface();
	//planeWidget->PlaceWidget();
	//planeWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
}

CReconstruView::~CReconstruView(void)
{
	textMapper->Delete();
	textActor->Delete();

	StyleTrackballActor->Delete();
	StyleTrackballCamera->Delete();

	renWin->Delete();
	ren->Delete();
	iren->Delete();		
	//StyleTrackballCamera->Delete();
		
	picker->Delete();
	annotatePick->Delete();
}
