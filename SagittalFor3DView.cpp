#include "SagittalFor3DView.h"

SagittalFor3DView::SagittalFor3DView()
	: QObject()
{
	renWin= vtkSmartPointer<vtkRenderWindow>::New();	
	ren = vtkSmartPointer<vtkRenderer>::New();
	interactor=vtkSmartPointer<vtkInteractorStyleImage>::New();
	iren=vtkSmartPointer<vtkRenderWindowInteractor>::New();
}

SagittalFor3DView::~SagittalFor3DView()
{

}

void SagittalFor3DView::Initialize()
{
	picker = vtkSmartPointer<vtkWorldPointPicker>::New();

	iren->SetRenderWindow(renWin);
	iren->SetPicker(picker);
	iren->Initialize();
	iren->SetInteractorStyle(interactor);	

	textMapper=vtkTextMapper::New();
	textActor=vtkActor2D::New();  
	annotatePick=vtkAnnotatePick::New();
	//iren->AddObserver(vtkCommand::EndPickEvent,annotatePick);
	iren->AddObserver(vtkCommand::LeftButtonPressEvent,annotatePick);

	mCommand = vtkSmartPointer<MoveCommand>::New();
	mCommand->qSignal = new QtSignal();

	annotatePick->textMapper=textMapper;
	annotatePick->textActor=textActor;

	textActor->SetMapper(textMapper);
	ren->AddActor2D(textActor);

	m_nSlice=0;
	m_nThreshMin = 115;
	m_nThreshMax = 3000;

	Transform=vtkSmartPointer<vtkTransform>::New();
	Matrix=vtkSmartPointer<vtkMatrix4x4>::New();

	greylut=vtkSmartPointer<vtkWindowLevelLookupTable>::New();
	greyActor=vtkSmartPointer<vtkActor>::New();
	greyPadder=vtkSmartPointer<vtkImageConstantPad>::New();

	segmentPadder=vtkSmartPointer<vtkImageConstantPad>::New();
	segmentOverlayActor=vtkSmartPointer<vtkActor>::New();

	thresholdPadder=vtkSmartPointer<vtkImageConstantPad>::New();
	thresholdOverlayActor=vtkSmartPointer<vtkActor>::New();

	SliceNumMapper=vtkSmartPointer<vtkTextMapper>::New();
	SliceNumActor=vtkSmartPointer<vtkActor2D>::New(); 

	SliceZvalueMapper=vtkSmartPointer<vtkTextMapper>::New();
	SliceZvalueActor=vtkSmartPointer<vtkActor2D>::New();
}

void SagittalFor3DView::ImportImageData(vtkImageData *ImageData)
{
	m_fPixelSpacing=ImageData->GetSpacing();
	int *nDataExtent=ImageData->GetExtent();

	m_nWidth=nDataExtent[1];
	m_nHeight=nDataExtent[3];
	m_nSliceSum=nDataExtent[5];	

	//QString str;
	//str.sprintf("%d ,%d, %d",  m_nWidth,m_nHeight,m_nSliceSum);
	//QMessageBox::information(0, tr("Information"),str);

	Matrix->Identity(); 
	Transform->SetMatrix(Matrix);

	//ReverseSlice();
	//resliceAxial->SetInput(ImageData);
	//resliceAxial->Update();

	vtkSmartPointer<vtkPlaneSource> greyPlane=vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> greyTransform=vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	vtkSmartPointer<vtkPolyDataNormals> greyNormals=vtkSmartPointer<vtkPolyDataNormals>::New();	
	vtkSmartPointer<vtkPolyDataMapper> greyMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkTexture> greyTexture=vtkSmartPointer<vtkTexture>::New();
	//greyActor=vtkActor::New();
	//vtkImageConstantPad *greyPadder=vtkImageConstantPad::New();

	greyPadder->SetInput(ImageData);
	greyPadder->SetOutputWholeExtent(0,0,0,m_nHeight,0,m_nSliceSum);
	greyPadder->SetConstant(0);

	greyTransform->SetTransform(Transform);
	greyTransform->SetInput(greyPlane->GetOutput());


	greyNormals->SetInput(greyTransform->GetOutput());

	greyNormals->FlipNormalsOff();

	greylut->SetWindow(400);
	greylut->SetLevel(40);
	greylut->SetTableRange(-160,240);
	greylut->Build();

	greyMapper->SetInput(greyPlane->GetOutput());

	greyMapper->ImmediateModeRenderingOn();

	greyTexture->SetInput(greyPadder->GetOutput());

	greyTexture->SetLookupTable(greylut);
	greyTexture->MapColorScalarsThroughLookupTableOn();
	greyTexture->InterpolateOn();

	greyActor->SetMapper(greyMapper);
	greyActor->SetTexture(greyTexture);

	fScale=(m_nSliceSum*m_fPixelSpacing[2])/((m_nHeight-1)*m_fPixelSpacing[1]);
	greyActor->SetScale(1, fScale,1);
	ren->AddActor(greyActor);

	//greyPlane->Delete();
	//greyTransform->Delete();
	//greyNormals->Delete();
	//greyMapper->Delete();
	//greyTexture->Delete();	

	SliceNumMapper->GetTextProperty()->SetFontFamilyToArial();
	SliceNumMapper->GetTextProperty()->SetFontSize(20);
	SliceNumMapper->GetTextProperty()->BoldOn();
	SliceNumMapper->GetTextProperty()->ShadowOff();
	SliceNumMapper->GetTextProperty()->ItalicOff();
	SliceNumMapper->GetTextProperty()->SetVerticalJustificationToBottom();
	SliceNumMapper->GetTextProperty()->SetColor(1,1,0);

	SliceNumActor->SetMapper(SliceNumMapper);

	QString sSliceNum;
	sSliceNum.sprintf("%d", 0);

	SliceNumMapper->SetInput(sSliceNum.toAscii ());

	SliceNumActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay() ; 
	SliceNumActor->GetPositionCoordinate()->SetValue( 0.85, 0.05, 0);

	//	SliceNumActor->SetPosition(479,25)	;
	//			textActor->SetPosition(Cutpicker->GetSelectionPoint()[0],
	//								   Cutpicker->GetSelectionPoint()[1]);
	//	double* fSelectionPoint=picker->GetSelectionPoint();
	//	textActor->VisibilityOn();	
	ren->AddActor2D(SliceNumActor);

	SliceZvalueMapper->GetTextProperty()->SetFontFamilyToArial();
	SliceZvalueMapper->GetTextProperty()->SetFontSize(20);
	SliceZvalueMapper->GetTextProperty()->BoldOn();
	SliceZvalueMapper->GetTextProperty()->ShadowOff();
	SliceZvalueMapper->GetTextProperty()->ItalicOff();
	SliceZvalueMapper->GetTextProperty()->SetVerticalJustificationToBottom();
	SliceZvalueMapper->GetTextProperty()->SetColor(1,1,0);

	SliceZvalueActor->SetMapper(SliceZvalueMapper);

	QString sSliceZvalue;
	sSliceZvalue.sprintf("CT Sagittal: %.3f", 0.0f);

	SliceZvalueMapper->SetInput(sSliceZvalue.toAscii ());

	SliceZvalueActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay() ; 
	SliceZvalueActor->GetPositionCoordinate()->SetValue( 0.05, 0.05, 0);
	ren->AddActor2D(SliceZvalueActor);

	DrawAxialLine(fScale);
	DrawCoronalLine(fScale);
	DrawFrameRect(renWin);

	SetCamera();
	renWin->Render();

	//greyPlane->GetOutput()->ReleaseDataFlagOn();
	greyTransform->GetOutput()->ReleaseDataFlagOn();
	greyPadder->GetOutput()->ReleaseDataFlagOn();
}

void SagittalFor3DView::SetCamera() 
{
	this->ren->GetActiveCamera()->SetPosition(0,0,2.93);
	this->ren->GetActiveCamera()->SetViewUp(0,1,0);

	this->ren->GetActiveCamera()->SetDistance(2.93);
	this->ren->GetActiveCamera()->SetFocalPoint(0,0,0);
	this->ren->GetActiveCamera()->SetParallelScale(1);

	this->ren->GetActiveCamera()->SetClippingRange(2.90,2.98);
	this->ren->GetActiveCamera()->SetThickness(0.07);
}

void SagittalFor3DView::ShowSlice()
{
	greyPadder->SetOutputWholeExtent(m_nSlice,m_nSlice,0,m_nHeight,0,m_nSliceSum);
	thresholdPadder->SetOutputWholeExtent(m_nSlice,m_nSlice,0,m_nHeight,0,m_nSliceSum);
	segmentPadder->SetOutputWholeExtent(m_nSlice,m_nSlice,0,m_nHeight,0,m_nSliceSum);	

	QString sSliceNum;
	sSliceNum.sprintf("%d", m_nSlice);
	SliceNumMapper->SetInput(sSliceNum.toAscii ());

	QString sSliceZvalue;
	sSliceZvalue.sprintf("CT Sagittal: %.3f", m_nSlice*m_fPixelSpacing[0]);
	SliceZvalueMapper->SetInput(sSliceZvalue.toAscii ());	

	renWin->Render();
}

void SagittalFor3DView::DoThreshold(vtkImageData* imageData)
{
	thresholdPadder->SetInput(imageData);

	thresholdPadder->SetOutputWholeExtent(m_nSlice,m_nSlice,0,m_nHeight,0,m_nSliceSum); 
	thresholdPadder->SetConstant(0);

	vtkSmartPointer<vtkPlaneSource> thresholdPlane=vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> thresholdTransform=vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	vtkSmartPointer<vtkPolyDataNormals> thresholdNormals=vtkSmartPointer<vtkPolyDataNormals>::New();
	vtkSmartPointer<vtkLookupTable> thresholdlut=vtkSmartPointer<vtkLookupTable>::New();
	vtkSmartPointer<vtkPolyDataMapper> thresholdMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkTexture> thresholdTexture=vtkSmartPointer<vtkTexture>::New();

	thresholdTransform->SetTransform(Transform);
	thresholdTransform->SetInput(thresholdPlane->GetOutput());


	thresholdNormals->SetInput(thresholdTransform->GetOutput());

	thresholdNormals->FlipNormalsOn();

	thresholdlut->SetNumberOfColors(256);
	thresholdlut->SetTableValue(0,0,0,0,1.0); 
	thresholdlut->SetTableValue(255,0,1,0,1.0);
	thresholdlut->Build();

	thresholdMapper->SetInput(thresholdPlane->GetOutput());

	thresholdMapper->ImmediateModeRenderingOn();

	thresholdTexture->SetInput(thresholdPadder->GetOutput());

	thresholdTexture->SetLookupTable(thresholdlut);
	thresholdTexture->MapColorScalarsThroughLookupTableOn();
	thresholdTexture->InterpolateOff();

	thresholdOverlayActor->SetMapper(thresholdMapper);
	thresholdOverlayActor->SetTexture(thresholdTexture);
	thresholdOverlayActor->GetProperty()->SetOpacity(0.5);

	fScale=(m_nSliceSum*m_fPixelSpacing[2])/((m_nHeight-1)*m_fPixelSpacing[1]);
	thresholdOverlayActor->SetScale(1, fScale,1);

	ren->AddActor(thresholdOverlayActor);
	thresholdOverlayActor->SetPosition(0, 0, 0.01);

	renWin->Render();

	thresholdPlane->GetOutput()->ReleaseDataFlagOn();
	thresholdTransform->GetOutput()->ReleaseDataFlagOn();
}

void SagittalFor3DView::Connect(vtkImageData* imageData)
{
	segmentPadder->SetInput(imageData);
	segmentPadder->SetOutputWholeExtent(m_nSlice,m_nSlice,0,m_nHeight,0,m_nSliceSum);
	segmentPadder->SetConstant(0);
	segmentPadder->Update();
	segmentPadder->Update();

	vtkSmartPointer<vtkPlaneSource>				segmentPlane = vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> segmentTransform = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	vtkSmartPointer<vtkPolyDataNormals>			segmentNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	vtkSmartPointer<vtkLookupTable>				segmentlut = vtkSmartPointer<vtkLookupTable>::New();
	vtkSmartPointer<vtkPolyDataMapper>			segmentMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkTexture>					segmentTexture = vtkSmartPointer<vtkTexture>::New();

	segmentTransform->SetTransform(Transform);
	segmentTransform->SetInput(segmentPlane->GetOutput());
	segmentTransform->Update();

	segmentNormals->SetInput(segmentTransform->GetOutput());
	segmentNormals->FlipNormalsOn();
	segmentNormals->Update();

	segmentlut->SetNumberOfColors(256);
	segmentlut->SetTableValue(0,0,0,0,1.0); 
	segmentlut->SetTableValue(255,1,0,0,1.0);
	//segmentlut->SetTableValue(255,1, 1, 0.501961,1.0);
	segmentlut->Build();

	segmentPlane->Update();

	segmentMapper->SetInput(segmentPlane->GetOutput());
	segmentMapper->ImmediateModeRenderingOn();
	segmentMapper->Update();

	segmentTexture->SetInput(segmentPadder->GetOutput());
	segmentTexture->SetLookupTable(segmentlut);
	segmentTexture->MapColorScalarsThroughLookupTableOn();
	segmentTexture->InterpolateOff();
	segmentTexture->Update();

	segmentOverlayActor->SetMapper(segmentMapper);
	segmentOverlayActor->SetTexture(segmentTexture);	
	segmentOverlayActor->GetProperty()->SetOpacity(0.5);

	double fScale=(m_nSliceSum*m_fPixelSpacing[2])/((m_nHeight-1)*m_fPixelSpacing[1]);
	segmentOverlayActor->SetScale(1, fScale,1);

	ren->AddActor(segmentOverlayActor);
	segmentOverlayActor->SetPosition(0, 0, 0.00);
	renWin->Render();
}

void SagittalFor3DView::DrawAxialLine(double fScale)
{
	vtkSmartPointer<vtkPoints> AxialPoints=vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkLine> AxialLine=vtkSmartPointer<vtkLine>::New();
	vtkSmartPointer<vtkUnstructuredGrid> AxialLineGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkDataSetMapper> AxialLineMapper=vtkSmartPointer<vtkDataSetMapper>::New() ;
	AxialLineActor=vtkSmartPointer<vtkActor>::New() ;

	AxialPoints->SetNumberOfPoints(2);
	AxialPoints->InsertPoint(0,-0.5,-0.5*fScale,0);
	AxialPoints->InsertPoint(1,0.5,-0.5*fScale,0);
	AxialLineActor->GetProperty()->SetDiffuseColor(255,0,0);

	AxialLine->GetPointIds()->SetNumberOfIds(2);

	AxialLine->GetPointIds()->SetId(0, 0);
	AxialLine->GetPointIds()->SetId(1, 1);

	AxialLine->GetPoints()->SetNumberOfPoints(2);

	AxialLineGrid->Allocate(1, 1);
	AxialLineGrid->InsertNextCell(AxialLine->GetCellType(),
		AxialLine->GetPointIds());
	AxialLineGrid->SetPoints(AxialPoints);

	AxialLineMapper->SetInput(AxialLineGrid);		
	AxialLineActor->SetMapper(AxialLineMapper);
	this->ren->AddActor(AxialLineActor);
	renWin->Render();
}

void SagittalFor3DView::DrawCoronalLine(double fScale)
{
	vtkSmartPointer<vtkPoints> CoronalPoints=vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkLine> CoronalLine=vtkSmartPointer<vtkLine>::New();
	vtkSmartPointer<vtkUnstructuredGrid> CoronalLineGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkDataSetMapper> CoronalLineMapper=vtkSmartPointer<vtkDataSetMapper>::New() ;
	CoronalLineActor=vtkSmartPointer<vtkActor>::New() ;

	CoronalPoints->SetNumberOfPoints(2);
	CoronalPoints->InsertPoint(0,-0.5,-0.5*fScale,0);
	CoronalPoints->InsertPoint(1,-0.5,0.5*fScale,0);
	CoronalLineActor->GetProperty()->SetDiffuseColor(0,255,0);

	CoronalLine->GetPointIds()->SetNumberOfIds(2);

	CoronalLine->GetPointIds()->SetId(0, 0);
	CoronalLine->GetPointIds()->SetId(1, 1);

	CoronalLine->GetPoints()->SetNumberOfPoints(2);

	CoronalLineGrid->Allocate(1, 1);
	CoronalLineGrid->InsertNextCell(CoronalLine->GetCellType(),
		CoronalLine->GetPointIds());
	CoronalLineGrid->SetPoints(CoronalPoints);

	CoronalLineMapper->SetInput(CoronalLineGrid);		
	CoronalLineActor->SetMapper(CoronalLineMapper);
	this->ren->AddActor(CoronalLineActor);
	renWin->Render();
}

void SagittalFor3DView::DrawFrameRect(vtkRenderWindow* renWin)
{
	FramePts = vtkSmartPointer<vtkPoints>::New();
	FrameRect = vtkSmartPointer<vtkCellArray>::New();
	FrameSelectRect = vtkSmartPointer<vtkPolyData>::New();
	FrameRectMapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
	FrameRectActor = vtkSmartPointer<vtkActor2D>::New();

	int* sz = renWin->GetSize();

	FramePts->InsertPoint(0, 5, 5, 0);
	FramePts->InsertPoint(1, sz[0]-5, 5, 0);
	FramePts->InsertPoint(2, sz[0]-5, sz[1]-5, 0);
	FramePts->InsertPoint(3, 5, sz[1]-5, 0);

	FrameRect->InsertNextCell(5);
	FrameRect->InsertCellPoint(0);
	FrameRect->InsertCellPoint(1);
	FrameRect->InsertCellPoint(2);
	FrameRect->InsertCellPoint(3);
	FrameRect->InsertCellPoint(0);

	FrameSelectRect->SetPoints(FramePts);
	FrameSelectRect->SetLines(FrameRect);
	FrameSelectRect->Update();

	FrameRectMapper->SetInput(FrameSelectRect);

	FrameRectActor->SetMapper(FrameRectMapper);

	ren->AddActor2D(FrameRectActor);
	FrameRectActor->GetProperty()->SetColor(1, 1, 0);

	renWin->Render();
}