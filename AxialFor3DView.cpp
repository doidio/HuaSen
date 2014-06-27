#include "AxialFor3DView.h"

bool IsChineseIncluded(QString str);

CAxialFor3DView::CAxialFor3DView(void)
{
	renWin= vtkRenderWindow::New();	
	ren = vtkRenderer::New();
	interactor=vtkInteractorStyleImage::New();
	iren=vtkRenderWindowInteractor::New();

	ProgressCommand=vtkSmartPointer<vtkProgress>::New();

	itkVTKExporter = VTKExportType::New();	
	VTKImporter = vtkImageImport::New();

	itkVTKResampleExporter = VTKExportType::New();	
	VTKResampleImporter = vtkImageImport::New();

	itk2vtk(itkVTKExporter,VTKImporter);
	itk2vtk(itkVTKResampleExporter,VTKResampleImporter);

	m_CommandObserver    = CommandType::New();
	m_CommandObserver->SetCallbackFunction( this, &CAxialFor3DView::ProgressUpdateFunc);
	m_nProgress=0;

	actorCount=0;

	m_bCropImage=false;
}

void CAxialFor3DView::Initialize() 
{	
	picker=vtkWorldPointPicker::New();
	
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

	SliceNumMapper=vtkTextMapper::New();
	SliceNumActor=vtkActor2D::New(); 

	SliceZvalueMapper=vtkTextMapper::New();
	SliceZvalueActor=vtkActor2D::New();

	//Threshold=vtkImageThreshold::New();
	//connect=vtkImageSeedConnectivity::New();
	//DilateErode3D=vtkSmartPointer<vtkImageDilateErode3D>::New();

	//isoMapper=vtkPolyDataMapper::New();
	//isoActor=vtkActor::New();

	ROIImage = ImageType::New();

	//resliceAxial=vtkImageReslice::New();	

	//reader=NULL;
}

//QStandardItemModel* MainWindow::createSourceModel(QObject *parent)
//{
//}
//bool IsChina(QString aSpeech)
//{
//	Buf16<512> textSen;
//	textSen.Zero();
//	textSen.Copy(aSpeech);
//	Bool bChnChar = EFalse;
//	TInt iLen = 0;
//	for (iLen = 0; iLen < textSen.Length(); iLen++)
//	{
//		TUint16 iInt16 = textSen[iLen];
//		if ((iInt16 >= 0x4E00) && (iInt16 <= 0x9FA5)	)
//		{
//			bChnChar = ETrue;
//
//			break;
//		}
//	}
//}


bool CAxialFor3DView::FileOpen(QString directory)
{
	if (!directory.isEmpty())
	{
		if(IsChineseIncluded(directory))
		{
			QMessageBox::information(0, tr("Information"),tr("The string contains characters in Chinese!"));
			return false;
		}
		else
		{
			typedef itk::GDCMImageIO                  GDCMImageIOType;
			typedef itk::GDCMSeriesFileNames          GDCMNamesGeneratorType;
			typedef std::vector< std::string >        FileNamesContainerType;

			GDCMImageIOType::Pointer dicomIO = GDCMImageIOType::New();
			
			GDCMNamesGeneratorType::Pointer nameGenerator = GDCMNamesGeneratorType::New();
			nameGenerator->SetInputDirectory( directory.toAscii());
			FileNamesContainerType fileNames = nameGenerator->GetInputFileNames();

			
			//typedef itk::ImageSeriesReader< ImageType >     SeriesReaderType;
			//SeriesReaderType::Pointer seriesReader = SeriesReaderType::New();

			seriesReader = SeriesReaderType::New();
			seriesReader->SetImageIO( dicomIO );
			seriesReader->SetFileNames( fileNames );
			
			seriesReader->AddObserver(itk::ProgressEvent(), m_CommandObserver);
			seriesReader->AddObserver(itk::EndEvent(), m_CommandObserver);
			seriesReader->Update();

			char *sPatientName = new char;
			dicomIO->GetPatientName(sPatientName);
			m_sPatientName=QString(sPatientName);

			itkVTKExporter->SetInput(seriesReader->GetOutput());

			VTKImporter->SetDataOrigin(0,0,0); 
			VTKImporter->Update();			

			ImportImageData(VTKImporter->GetOutput());

			//Threshold->SetInput(VTKImporter->GetOutput());
			
			//seriesReader->Delete();
			//nameGenerator->Delete();
			//dicomIO->Delete();
			return true;
		}
	}	
	else
		return false;


	//seriesReader->GetOutput()->ReleaseDataFlagOn();
	//VTKImporter->GetOutput()->ReleaseDataFlagOn();

	//if (!directory.isEmpty())
	//{
	//	reader=vtkDICOMImageReader::New();
	//	reader->SetDirectoryName(directory.toAscii());
	//	reader->Update();
	//	ImportImageData(reader->GetOutput());
	//	Threshold->SetInput(resliceAxial->GetOutput());		
	//	return true;		
	//}	
	//else
	//	return false;



	//DICOMLoader dicomLoader;
	//
	//if(dicomLoader.exec() == QDialog::Accepted)
	//{
	//	if(dicomLoader.isLoaded())
	//	{
	//		// get the imported result
	//		std::cout << "start to visualize the loaded image(s)..." << std::endl;

	//		VisualizationLoadedResult visualization;

	//		std::cout << "set the image source..." << std::endl;
	//		visualization.setImageSourceForVTKDICOMReader(dicomLoader.getVolumeReader()->getSlices());
 //           std::cout << "set the image source successfully!" << std::endl;

	//		if(visualization.isReady())
	//		{				
	//			ImportImageData(visualization.getImageData());
	//			Threshold->SetInput(resliceAxial->GetOutput());
	//		}
	//	}
	//	return true;		
	//}	
	//else
	//	return false;	
}

void CAxialFor3DView::itk2vtk(VTKExportPointer itkVTKExporterPara,vtkImageImport *VTKImporterPara)
{
	itkVTKExporterPara->ReleaseDataFlagOn();
	VTKImporterPara->ReleaseDataFlagOn();

	VTKImporterPara->SetUpdateInformationCallback(
		itkVTKExporterPara->GetUpdateInformationCallback());
	VTKImporterPara->SetPipelineModifiedCallback(
		itkVTKExporterPara->GetPipelineModifiedCallback());
	VTKImporterPara->SetWholeExtentCallback(
		itkVTKExporterPara->GetWholeExtentCallback());
	VTKImporterPara->SetSpacingCallback(
		itkVTKExporterPara->GetSpacingCallback());
	//VTKImporterPara->SetOriginCallback(
	//	itkVTKExporterPara->GetOriginCallback());
	VTKImporterPara->SetScalarTypeCallback(
		itkVTKExporterPara->GetScalarTypeCallback());
	VTKImporterPara->SetNumberOfComponentsCallback(
		itkVTKExporterPara->GetNumberOfComponentsCallback());
	VTKImporterPara->SetPropagateUpdateExtentCallback(
		itkVTKExporterPara->GetPropagateUpdateExtentCallback());
	VTKImporterPara->SetUpdateDataCallback(
		itkVTKExporterPara->GetUpdateDataCallback());
	VTKImporterPara->SetDataExtentCallback(
		itkVTKExporterPara->GetDataExtentCallback());
	VTKImporterPara->SetBufferPointerCallback(
		itkVTKExporterPara->GetBufferPointerCallback());  
	VTKImporterPara->SetCallbackUserData(
		itkVTKExporterPara->GetCallbackUserData());
}

void CAxialFor3DView::ImportImageData(vtkImageData *ImageData)
{
	//ImageData->SetOrigin(0,0,0); 
	//ImageData->Update();

	m_fPixelSpacing=ImageData->GetSpacing();
	int *nDataExtent=ImageData->GetExtent();

	m_nWidth=nDataExtent[1]+1;
	m_nHeight=nDataExtent[3]+1;
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
	greyPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,0,0);
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

	fScale=(m_nHeight*m_fPixelSpacing[1])/(m_nWidth*m_fPixelSpacing[0]);
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
	SliceNumMapper->GetTextProperty()->SetColor(1,0,0);

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
	SliceZvalueMapper->GetTextProperty()->SetColor(1,0,0);

	SliceZvalueActor->SetMapper(SliceZvalueMapper);

	QString sSliceZvalue;
	sSliceZvalue.sprintf("CT Axial: %.3f", 0.0f);

	SliceZvalueMapper->SetInput(sSliceZvalue.toAscii ());

	SliceZvalueActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay() ; 
	SliceZvalueActor->GetPositionCoordinate()->SetValue( 0.05, 0.05, 0);
	ren->AddActor2D(SliceZvalueActor);

	DrawCoronalLine(fScale);
	DrawSagittalLine(fScale);
	DrawFrameRect(renWin);

	SetCamera();
	renWin->Render();

	//greyPlane->GetOutput()->ReleaseDataFlagOn();
	greyTransform->GetOutput()->ReleaseDataFlagOn();
	greyPadder->GetOutput()->ReleaseDataFlagOn();
}

void CAxialFor3DView::cropImage(ImagePointer source , ImagePointer target,int nStartSlice, int nEndSlice)
{
	const unsigned int Dimension = 3;

	typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
	typedef itk::ImageRegionIterator< ImageType>       IteratorType;

	ImageType::RegionType inputRegion;

	ImageType::RegionType::IndexType inputStart;
	ImageType::RegionType::SizeType  size;

	inputStart[0] = 0;//174 m_nCropStart[0]
	inputStart[1] = 0;//160 m_nCropStart[1]
	inputStart[2] = nStartSlice;

	size[0]  = m_nWidth;//193 m_nCropSize[0]
	size[1]  = m_nHeight;//193 m_nCropSize[1]
	size[2]  = nEndSlice-nStartSlice+1;//350 746

	inputRegion.SetSize( size );
	inputRegion.SetIndex( inputStart );

	ImageType::RegionType outputRegion;

	ImageType::RegionType::IndexType outputStart;

	outputStart[0] = 0;
	outputStart[1] = 0;
	outputStart[2] = 0;

	outputRegion.SetSize( size );
	outputRegion.SetIndex( outputStart );
	
	target->SetRegions( outputRegion );
	const ImageType::SpacingType& spacing = source->GetSpacing();
	const ImageType::PointType& inputOrigin = source->GetOrigin();
	double   outputOrigin[ Dimension ];

	for(unsigned int i=0; i< Dimension; i++)
	{
		outputOrigin[i] = inputOrigin[i] + spacing[i] * inputStart[i];
	}

	target->SetSpacing( spacing );
	target->SetOrigin(  outputOrigin );
	target->Allocate();

	ConstIteratorType inputIt(   source, inputRegion  );
	IteratorType      outputIt(  target, outputRegion );

	for ( inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();
		++inputIt, ++outputIt)
	{
		outputIt.Set(  inputIt.Get()  );
	}	

	itkVTKResampleExporter->SetInput(target);
	VTKResampleImporter->SetDataOrigin(0,0,0);
	itkVTKResampleExporter->Update();

	greyPadder->SetInput(VTKResampleImporter->GetOutput());

	greyPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,0,0);
	greyPadder->SetConstant(0);
	m_nSliceSum=nEndSlice-nStartSlice;

	m_bCropImage=true;
	VTKImporter->Delete();
	source->ReleaseData();
	//seriesReader->Delete();
	//itkVTKExporter->RemoveInput();
	//itkVTKExporter->RemoveOutput();
}

//void CAxialFor3DView::ReverseSlice()
//{	
//	vtkMatrix4x4 *RawMatrix=vtkMatrix4x4::New();
//	vtkMatrix4x4 *NormalMatrix=vtkMatrix4x4::New();	
//
//	vtkMatrix4x4 *Raw2NormalMatrix=vtkMatrix4x4::New();
//	vtkTransform *Raw2NormalTransform=vtkTransform::New();
//
//	double fZHeight=m_nSliceSum*m_fPixelSpacing[2];
//	double fImageWidth=(m_nWidth-1)*m_fPixelSpacing[0];
//	double fImageHeight=(m_nHeight-1)*m_fPixelSpacing[1];
//
//	RawMatrix->SetElement(0,0,0);
//	RawMatrix->SetElement(1,0,0);
//	RawMatrix->SetElement(2,0,0);
//	RawMatrix->SetElement(3,0,1);
//
//	RawMatrix->SetElement(0,1,fImageWidth);
//	RawMatrix->SetElement(1,1,0);
//	RawMatrix->SetElement(2,1,0);
//	RawMatrix->SetElement(3,1,1);
//
//	RawMatrix->SetElement(0,2,0);
//	RawMatrix->SetElement(1,2,fImageHeight);
//	RawMatrix->SetElement(2,2,0);
//	RawMatrix->SetElement(3,2,1);
//
//	RawMatrix->SetElement(0,3,0);
//	RawMatrix->SetElement(1,3,0);
//	RawMatrix->SetElement(2,3,fZHeight);
//	RawMatrix->SetElement(3,3,1);
//
//	NormalMatrix->SetElement(0,0,0);
//	NormalMatrix->SetElement(1,0,fImageHeight);
//	NormalMatrix->SetElement(2,0,fZHeight);
//	NormalMatrix->SetElement(3,0,1);
//
//	NormalMatrix->SetElement(0,1,fImageWidth);
//	NormalMatrix->SetElement(1,1,fImageHeight);
//	NormalMatrix->SetElement(2,1,fZHeight);
//	NormalMatrix->SetElement(3,1,1);
//
//	NormalMatrix->SetElement(0,2,0);
//	NormalMatrix->SetElement(1,2,0);
//	NormalMatrix->SetElement(2,2,fZHeight);
//	NormalMatrix->SetElement(3,2,1);
//
//	NormalMatrix->SetElement(0,3,0);
//	NormalMatrix->SetElement(1,3,fImageHeight);
//	NormalMatrix->SetElement(2,3,0);
//	NormalMatrix->SetElement(3,3,1);
//
//	NormalMatrix->Invert();
//	Raw2NormalMatrix->Multiply4x4(RawMatrix,NormalMatrix,Raw2NormalMatrix);
//	
//	//if(pDoc->m_bMandiblePlan)
//	//{
//	//	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	//	CImplantSdiView  *pImplantSdiView = (CImplantSdiView*)pMainFrame->m_wndSplitter4.GetPane(1,0);
//	//	Raw2NormalMatrix->Multiply4x4(Raw2NormalMatrix,
//	//		pImplantSdiView->Restru2WorldTransform->GetMatrix(),Raw2NormalMatrix);
//	//}
//
//	//vtkTransform *Raw2NormalTransform=vtkTransform::New();
//	Raw2NormalTransform->SetMatrix(Raw2NormalMatrix);
//
//	resliceAxial->SetResliceTransform(Raw2NormalTransform);	
//
//	RawMatrix->Delete();
//	NormalMatrix->Delete();
//
//	Raw2NormalTransform->Delete();
//	Raw2NormalMatrix->Delete();
//}

void CAxialFor3DView::SetCamera() 
{
	this->ren->GetActiveCamera()->SetPosition(0,0,2.93);
	this->ren->GetActiveCamera()->SetViewUp(0,1,0);

	this->ren->GetActiveCamera()->SetDistance(2.93);
	this->ren->GetActiveCamera()->SetFocalPoint(0,0,0);
	this->ren->GetActiveCamera()->SetParallelScale(1);

	this->ren->GetActiveCamera()->SetClippingRange(2.90,2.98);
	this->ren->GetActiveCamera()->SetThickness(0.07);
}

void CAxialFor3DView::ShowSlice()
{
	greyPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSlice,m_nSlice);
	thresholdPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSlice,m_nSlice);
	segmentPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSlice,m_nSlice);	

	QString sSliceNum;
	sSliceNum.sprintf("%d", m_nSlice);
	SliceNumMapper->SetInput(sSliceNum.toAscii ());

	QString sSliceZvalue;
	sSliceZvalue.sprintf("CT Axial: %.3f", m_nSlice*m_fPixelSpacing[2]);
	SliceZvalueMapper->SetInput(sSliceZvalue.toAscii ());	

	//SetCamera();
	//greyPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSliceSum-m_nSlice,m_nSliceSum-m_nSlice);
	renWin->Render();
}

void CAxialFor3DView::DoThreshold() 
{	
	/*ThresholdImageFilter->SetLowerThreshold( m_nThreshMin );
	ThresholdImageFilter->SetUpperThreshold(m_nThreshMax );
	ThresholdImageFilter->Update();*/

	vtkSmartPointer<vtkImageThreshold> Threshold=vtkSmartPointer<vtkImageThreshold>::New();

	if(m_bCropImage)
	{
		Threshold->SetInput(VTKResampleImporter->GetOutput());

	}
	else
	{
		Threshold->SetInput(VTKImporter->GetOutput());

	}

	Threshold->ThresholdBetween(m_nThreshMin,m_nThreshMax);
	Threshold->SetOutValue(0);
	Threshold->SetInValue(255)	;   
	Threshold->ReplaceOutOn();
	Threshold->ReplaceInOn();
	Threshold->SetOutputScalarTypeToUnsignedChar();

	thresholdPadder->SetInput(Threshold->GetOutput());

	thresholdPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSlice,m_nSlice); 
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
	thresholdOverlayActor->GetProperty()->SetOpacity(0.5)	;

	ren->AddActor(thresholdOverlayActor);
	thresholdOverlayActor->SetPosition(0, 0, 0.01);
	renWin->Render();

	//thresholdPlane->Delete();
	//thresholdTransform->Delete();
	//thresholdNormals->Delete();
	//thresholdlut->Delete();
	//thresholdMapper->Delete();
	//thresholdTexture->Delete();
	//Threshold->Delete();
	Threshold->GetOutput()->ReleaseDataFlagOn();
	thresholdPlane->GetOutput()->ReleaseDataFlagOn();
	thresholdTransform->GetOutput()->ReleaseDataFlagOn();
	thresholdPadder->GetOutput()->ReleaseDataFlagOn();
}

void CAxialFor3DView::Connect(int xp,int yp,int zp) 
{
//330Mb


	vtkSmartPointer<vtkImageThreshold> Threshold = vtkSmartPointer<vtkImageThreshold>::New();

	if(m_bCropImage)
	{
		Threshold->SetInput(VTKResampleImporter->GetOutput());
	}
	else
	{
		Threshold->SetInput(VTKImporter->GetOutput());
	}

	Threshold->ThresholdBetween(m_nThreshMin,m_nThreshMax);
	Threshold->SetOutValue(0);
	Threshold->SetInValue(255)	;   
	Threshold->ReplaceOutOn();
	Threshold->ReplaceInOn();
	Threshold->SetOutputScalarTypeToUnsignedChar();
	Threshold->Update();
//460Mb

	ProgressCommand->text="	(1/3)	Calculating threshold...";
	Threshold->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	Threshold->Update();

	connect=vtkSmartPointer<vtkImageSeedConnectivity>::New();

	connect->SetInput(Threshold->GetOutput());
	Threshold->GetOutput()->ReleaseDataFlagOn();
	connect->SetInputConnectValue(255)	;
	connect->SetOutputConnectedValue(255)	;
	connect->SetOutputUnconnectedValue(0);

	connect->AddSeed(xp,yp,zp);

	ProgressCommand->text="	(2/3)	Threshold segmentation...";
	connect->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	connect->Update();
//600Mb

	//vtkSmartPointer<vtkImageDilateErode3D> DilateErode3D=vtkSmartPointer<vtkImageDilateErode3D>::New();
	//DilateErode3D->SetInput(connect->GetOutput());
	//DilateErode3D->SetDilateValue(255)	;
	//DilateErode3D->SetErodeValue(0);
	//DilateErode3D->SetKernelSize(4, 4, 4);
	//Progress->m_nStage=0;
	//DilateErode3D->AddObserver(vtkCommand::ProgressEvent, Progress);
	//DilateErode3D->Update();
//730Mb

	vtkSmartPointer<vtkPlaneSource>				segmentPlane = vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> segmentTransform = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	vtkSmartPointer<vtkPolyDataNormals>			segmentNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	vtkSmartPointer<vtkLookupTable>				segmentlut = vtkSmartPointer<vtkLookupTable>::New();
	vtkSmartPointer<vtkPolyDataMapper>			segmentMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkTexture>					segmentTexture = vtkSmartPointer<vtkTexture>::New();	
	//vtkSmartPointer<vtkActor> segmentOverlayActor=vtkSmartPointer<vtkActor>::New();	

	segmentPadder->SetInput(connect->GetOutput());
	segmentPadder->SetOutputWholeExtent(0,m_nWidth,0,m_nHeight,m_nSlice,m_nSlice); 
	segmentPadder->SetConstant(0);
	segmentPadder->Update();

	ProgressCommand->text="	(3/3)	Paddering...";
	segmentPadder->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	segmentPadder->Update();

	segmentTransform->SetTransform(Transform);
	segmentTransform->SetInput(segmentPlane->GetOutput());
	segmentTransform->Update();

	ProgressCommand->text="segmentTransform...";
	segmentTransform->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	segmentTransform->Update();

	segmentNormals->SetInput(segmentTransform->GetOutput());
	segmentNormals->FlipNormalsOn();
	segmentNormals->Update();

	ProgressCommand->text="	(3/3)	Normals generating...";
	segmentNormals->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
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

	ProgressCommand->text="	segmentTexture...";
	segmentTexture->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	segmentTexture->Update();
	//segmentTexture->Update();

	segmentOverlayActor->SetMapper(segmentMapper);
	segmentOverlayActor->SetTexture(segmentTexture);	
	segmentOverlayActor->GetProperty()->SetOpacity(0.5);

	ren->AddActor(segmentOverlayActor);
	segmentOverlayActor->SetPosition(0, 0, 0.00);
	renWin->Render();
}

void CAxialFor3DView::Reconstru()
{
	vtkSmartPointer<vtkImageData> ImageData=vtkSmartPointer<vtkImageData>::New();

	ImageData->SetDimensions(m_nWidth,m_nHeight,m_nSliceSum+1);
	ImageData->SetScalarTypeToShort();
	ImageData->SetNumberOfScalarComponents(1);
	ImageData->AllocateScalars();
	ImageData->SetSpacing(m_fPixelSpacing); 
	ImageData->SetOrigin(0,0,0);

	short *ptr1=(short *) ImageData->GetScalarPointer();

	unsigned char *ptr2=(unsigned char *) connect->GetOutput()->GetScalarPointer();


	short *ptr3;
	if(m_bCropImage)
		ptr3=(short *) VTKResampleImporter->GetOutput()->GetScalarPointer();
	else
		ptr3=(short *) VTKImporter->GetOutput()->GetScalarPointer();
	int nIndexNum=m_nWidth*m_nHeight*(m_nSliceSum+1);
	for(int i=0;i<nIndexNum;i++)
	{
		if(ptr2[i]==0)
			ptr1[i]=-1024;
		else
		{
			//ptr1[i-1]=ptr3[i-1];
			ptr1[i]=ptr3[i];
			//ptr1[i+1]=ptr3[i+1];
		}
	}
	ImageData->Update();
	
	vtkSmartPointer<vtkImageChangeInformation> translator=vtkSmartPointer<vtkImageChangeInformation>::New();
	translator->SetInput(ImageData);

	translator->SetExtentTranslation(1, 1, 1);
	translator->SetOriginTranslation(-1.0, -1.0, -1.0);

	vtkSmartPointer<vtkImageConstantPad> isoPad=vtkSmartPointer<vtkImageConstantPad>::New();
	isoPad->SetInput(translator->GetOutput());
	translator->GetOutput()->ReleaseDataFlagOn();
	isoPad->SetConstant(0);

	ProgressCommand->text="	(1/5)	Paddering...";
	isoPad->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	isoPad->Update();

	translator->Update();

	int extent[6];
	ImageData->GetWholeExtent(extent);
	// now set the output extent to the new size, padded by 2 on the
	// positive side
	isoPad->SetOutputWholeExtent(extent[0], extent[1] + 2, extent[2], extent[3] + 2, extent[4], extent[5] + 2);
	isoPad->Update();
//-----------------------------------------------------------------------------------------------------------------------
// Êä³öImageData
//-----------------------------------------------------------------------------------------------------------------------
    vtkSmartPointer<vtkXMLImageDataWriter> imgWriter =
		vtkSmartPointer<vtkXMLImageDataWriter>::New();
    std::string fileName = "imageData.vti";
    imgWriter->SetFileName(fileName.c_str());
    imgWriter->SetInput(ImageData);
    imgWriter->Write();
//-----------------------------------------------------------------------------------------------------------------------
	vtkSmartPointer<vtkMarchingCubes> iso=vtkSmartPointer<vtkMarchingCubes>::New();
	iso->SetInput(isoPad->GetOutput());
	//iso->SetInput(ImageData);
	isoPad->GetOutput()->ReleaseDataFlagOn();
	iso->GenerateValues(1,m_nThreshMin,m_nThreshMax);

	ProgressCommand->text="	(2/5)	MC algorithm...";
	iso->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	iso->Update();

	vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
	decimator->SetInput(iso->GetOutput());
	iso->GetOutput()->ReleaseDataFlagOn();
	connect->GetOutput()->ReleaseDataFlagOn();
	ImageData->ReleaseDataFlagOn();
	decimator->SetTargetReduction(0.2);
	decimator->SplittingOff();
	decimator->PreserveTopologyOff();

	ProgressCommand->text="	(3/5)	Decimating...";
	decimator->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	decimator->Update();

	vtkSmartPointer<vtkWindowedSincPolyDataFilter> smootherSinc = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
	smootherSinc->SetInput(decimator->GetOutput());
	//decimator->GetOutput()->ReleaseDataFlagOn();
	smootherSinc->FeatureEdgeSmoothingOff();
	smootherSinc->BoundarySmoothingOff();

	ProgressCommand->text="	(4/5)	Smoothing...";
	smootherSinc->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	smootherSinc->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformer = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformer->SetInput(smootherSinc->GetOutput());
	smootherSinc->GetOutput()->ReleaseDataFlagOn();
	transformer->SetTransform(Transform);

	ProgressCommand->text="transformer...";
	transformer->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	transformer->Update();

	vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
	stripper->SetInput(transformer->GetOutput());

	ProgressCommand->text="	(5/5)	Stripping...";
	stripper->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	stripper->Update();

	vtkSmartPointer<vtkPolyDataNormals> Normals=vtkSmartPointer<vtkPolyDataNormals>::New();	
	Normals->SetInput(stripper->GetOutput());
	transformer->GetOutput()->ReleaseDataFlagOn();
	//Normals->SetInput(iso->GetOutput());
	Normals->SetFeatureAngle(60.0);

	ProgressCommand->text="	(5/5)	Normals generating...";
	Normals->AddObserver(vtkCommand::ProgressEvent, ProgressCommand);
	Normals->Update();

/**************************************************************************************************/

	QActorObject* pActorObject=new QActorObject();
	//pActorObject->segmentOverlayActor=vtkSmartPointer<vtkActor>::New();
	pActorObject->isoMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	pActorObject->isoActor=vtkSmartPointer<vtkActor>::New();

	//QActorObject* pActorObject=(QActorObject*) m_Actorlist.last();

	pActorObject->isoMapper->SetInput((vtkPolyData *)Normals->GetOutput());
	
	pActorObject->isoMapper->ScalarVisibilityOff();
	pActorObject->isoMapper->ImmediateModeRenderingOn();
	//isoMapper->SetResolveCoincidentTopologyToPolygonOffset();
	pActorObject->isoMapper->Update();

	//iso->Delete();
	//Normals->Delete();	

	pActorObject->isoActor->SetMapper(pActorObject->isoMapper);
	//pActorObject->isoActor->GetProperty()->SetColor(1, 1, 0.501961);

	//vtkSmartPointer<vtkProperty> backFaces = vtkSmartPointer<vtkProperty>::New();
	//backFaces->SetSpecular(0.0);
	//backFaces->SetDiffuse(0.0);
	//backFaces->SetAmbient(1.0);
	////backFaces->SetAmbientColor(1.0000, 0.3882, 0.2784);

	//pActorObject->isoActor->SetBackfaceProperty(backFaces);

	m_Actorlist.append(pActorObject);



	//Normals->GetOutput()->ReleaseDataFlagOn();
}

void CAxialFor3DView::SetActorColor(int ,QActorObject* pActorObject)
{
	vtkMath::RandomSeed(time(NULL));
	double rgb[3];
	for(unsigned int i = 0; i < 3; i++)
	{
		rgb[i] = vtkMath::Random(0.0,1.0);
	}
	pActorObject->isoActor->GetProperty()->SetColor(rgb[0],rgb[1],rgb[2]);
	//switch(i % 10)
	//{
	//case 0:
	//	pActorObject->isoActor->GetProperty()->SetColor(1,99/255.0,71/255.0);
	//	break;
	//case 1:
	//	pActorObject->isoActor->GetProperty()->SetColor(1, 1, 0.501961);//bone
	//	break;
	//case 2:
	//	pActorObject->isoActor->GetProperty()->SetColor(220/255.0,80/255.0,80/255.0); //lavender
	//	break;
	//case 3:
	//	pActorObject->isoActor->GetProperty()->SetColor(0/255.0,255/255.0,0.0/255)	;// dark violet
	//	break;
	//case 4:
	//	pActorObject->isoActor->GetProperty()->SetColor(200/255.0,0/255.0,200/255.0);// dark red
	//	break;
	//case 5:
	//	pActorObject->isoActor->GetProperty()->SetColor(210/255.0,105/255.0,30/255.0);// orange
	//	break;
	//case 6:
	//	pActorObject->isoActor->GetProperty()->SetColor(178/255.0,34/255.0,34/255.0);// dark turquoise
	//	break;
	//case 7:
	//	pActorObject->isoActor->GetProperty()->SetColor(105/255.0,105/255.0,105/255.0); // cyan
	//	break;
	//case 8:
	//	pActorObject->isoActor->GetProperty()->SetColor(138/255.0,43/255.0,226/255.0);
	//	break;
	//case 9:
	//	pActorObject->isoActor->GetProperty()->SetColor(0,139/255.0,139/255.0);
	//	break;
	//default:
	//	break;
	//}	
}



CAxialFor3DView::~CAxialFor3DView(void)
{
	//iren->RemoveObserver(annotatePick);

	itkVTKExporter->Delete();	
	VTKImporter->Delete();

	itkVTKResampleExporter->Delete();	
	VTKResampleImporter->Delete();
	annotatePick->Delete();

	textMapper->Delete();
	textActor->Delete(); 	

	renWin->Delete();
	ren->Delete();
	iren->Delete();
	interactor->Delete();

	picker->Delete();

	ProgressCommand->Delete();
}

void CAxialFor3DView::ProgressUpdateFunc( itk::Object * caller, const itk::EventObject & event )
{
	itk::ProcessObject::Pointer process =
			  dynamic_cast< itk::ProcessObject *>( caller );
	

	if( typeid( itk::EndEvent ) == typeid( event ) )
	{
		m_nProgress=0;
	}

	if( typeid( itk::ProgressEvent ) == typeid( event ) )
	{
		m_nProgress = (int) (process->GetProgress()*100);		
	}

	emit ProgressUpdate(m_nProgress);
}
void CAxialFor3DView::DrawSagittalLine(double fScale)
{
	vtkSmartPointer<vtkPoints> SagittalPoints=vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkLine> SagittalLine=vtkSmartPointer<vtkLine>::New();
	vtkSmartPointer<vtkUnstructuredGrid> SagittalLineGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkDataSetMapper> SagittalLineMapper=vtkSmartPointer<vtkDataSetMapper>::New() ;
	SagittalLineActor=vtkSmartPointer<vtkActor>::New() ;

	SagittalPoints->SetNumberOfPoints(2);
	SagittalPoints->InsertPoint(0,-0.5,0.5*fScale,0);
	SagittalPoints->InsertPoint(1,-0.5,-0.5*fScale,0);
	SagittalLineActor->GetProperty()->SetDiffuseColor(255,255,0);

	SagittalLine->GetPointIds()->SetNumberOfIds(2);

	SagittalLine->GetPointIds()->SetId(0, 0);
	SagittalLine->GetPointIds()->SetId(1, 1);

	SagittalLine->GetPoints()->SetNumberOfPoints(2);

	SagittalLineGrid->Allocate(1, 1);
	SagittalLineGrid->InsertNextCell(SagittalLine->GetCellType(),
		SagittalLine->GetPointIds());
	SagittalLineGrid->SetPoints(SagittalPoints);

	SagittalLineMapper->SetInput(SagittalLineGrid);		
	SagittalLineActor->SetMapper(SagittalLineMapper);
	this->ren->AddActor(SagittalLineActor);
	renWin->Render();

}

void CAxialFor3DView::DrawCoronalLine(double fScale)
{
	vtkSmartPointer<vtkPoints> CoronalPoints=vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkLine> CoronalLine=vtkSmartPointer<vtkLine>::New();
	vtkSmartPointer<vtkUnstructuredGrid> CoronalLineGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkDataSetMapper> CoronalLineMapper=vtkSmartPointer<vtkDataSetMapper>::New() ;
	CoronalLineActor=vtkSmartPointer<vtkActor>::New() ;

	CoronalPoints->SetNumberOfPoints(2);
	CoronalPoints->InsertPoint(0,-0.5,-0.5*fScale,0);
	CoronalPoints->InsertPoint(1,0.5,-0.5*fScale,0);
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

	//AxialLineActor->VisibilityOff();
}


void CAxialFor3DView::DrawFrameRect(vtkRenderWindow* renWin)
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
	FrameRectActor->GetProperty()->SetColor(1, 0, 0);

	renWin->Render();
}