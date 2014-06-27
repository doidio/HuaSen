#include "windows.h"
#include "shellapi.h"

#include "ReconstruMainWindow.h"
#include <vtkSTLReader.h>
#include <vtkClipClosedSurface.h>
#include <vtkPlaneCollection.h>
#include <math.h>
//#include "DlgSelectSurgeryType.h"

//extern bool m_bMandiblePlan;

ReconstruMainWindow::ReconstruMainWindow(void)
{
	ui.setupUi(this);
	ui.progressBar->hide();

	pInfoDlg = new ProjectInfo;
	pInfoDlg->setParent(this,Qt::Dialog);

	pAxialView=new CAxialFor3DView();
	ui.AxialView->SetRenderWindow(pAxialView->renWin);
	ui.AxialView->GetRenderWindow()->AddRenderer(pAxialView->ren);
	pAxialView->Initialize();

	pSagittalView=new SagittalFor3DView();
	ui.SagittalView->SetRenderWindow(pSagittalView->renWin);
	ui.SagittalView->GetRenderWindow()->AddRenderer(pSagittalView->ren);
	pSagittalView->Initialize();

	pCoronalView=new CoronalFor3DView();
	ui.CoronalView->SetRenderWindow(pCoronalView->renWin);
	ui.CoronalView->GetRenderWindow()->AddRenderer(pCoronalView->ren);
	pCoronalView->Initialize();

	pReconstruView=new CReconstruView();
	ui.ReconstruView->SetRenderWindow(pReconstruView->renWin);
	ui.ReconstruView->GetRenderWindow()->AddRenderer(pReconstruView->ren);
	pReconstruView->Initialize();

	iniTemplate = new InitialTemplate(pReconstruView->renWin,
									  pReconstruView->ren,
								   	  pReconstruView->iren,
									  pReconstruView->StyleTrackballCamera,
									  pReconstruView->picker);
	pReconstruView->StyleTrackballCamera->AddObserver(vtkCommand::InteractionEvent,iniTemplate->SaveCurveCommand);
	connect(iniTemplate->SaveCurveCommand->qSaveCurve, SIGNAL(saveCurrentCurve()),this, SLOT(SaveCurve()));

	contourWidgetRep = vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	temPts = vtkSmartPointer<vtkPoints>::New();
	punchRadius = 1.0;

	Progress = vtkSmartPointer<vtkProgress>::New();
	meaCount = 0;
	delMeaCount = 0;
	m_bTransferSucceed=false;
	//m_bViewMaximize=false;

	connect(ui.FILE_OPEN, SIGNAL(triggered()), this, SLOT(OnFileOpen()));
	connect(ui.STL_OPEN, SIGNAL(triggered()), this, SLOT(OnStlFileOpen()));
	connect(ui.INFORMATION, SIGNAL(triggered()),this, SLOT(OnInformation()));

	connect(ui.VIEW_MODE, SIGNAL(triggered()), this, SLOT(OnSetViewMode()));
	connect(this, SIGNAL(shift()),this, SLOT(OnSetViewMode()));
	connect(ui.ReconstruView, SIGNAL(shift()),this, SLOT(OnSetViewMode()));
	connect(ui.AxialView, SIGNAL(shift()),this, SLOT(OnSetViewMode()));

	connect(pAxialView->ProgressCommand, SIGNAL(ProgressUpdate(int)), this, SLOT(advanceProgressBar(int)));
	connect(pAxialView->ProgressCommand, SIGNAL(TextUpdate(QString)), this, SLOT(advanceStatusBar(QString)));
	connect(pAxialView, SIGNAL(ProgressUpdate(int)), this, SLOT(advanceProgressBar(int)));
	connect(pAxialView, SIGNAL(TextUpdate(QString)), this, SLOT(advanceStatusBar(QString)));

	connect(iniTemplate->Progress, SIGNAL(ProgressUpdate(int)), this, SLOT(advanceProgressBar(int)));
	connect(iniTemplate->Progress, SIGNAL(TextUpdate(QString)), this, SLOT(advanceStatusBar(QString)));

	connect(this->Progress, SIGNAL(ProgressUpdate(int)), this, SLOT(advanceProgressBar(int)));
	connect(this->Progress, SIGNAL(TextUpdate(QString)), this, SLOT(advanceStatusBar(QString)));

	connect(ui.delModelButton, SIGNAL(clicked()),this, SLOT(OnDeleteModel()));
	connect(ui.delMeasureButton, SIGNAL(clicked()),this, SLOT(OnDeleteMeasure()));
	connect(ui.RECONSTRU3D, SIGNAL(triggered()),this, SLOT(OnReconstru()));

	//connect(ui.EXPORT_STL, SIGNAL(triggered()),this, SLOT(OnExportStl()));
	connect(ui.TO_MAINWINDOW, SIGNAL(triggered()),this, SLOT(OnExportToMainWindow()));
	connect(ui.CUT, SIGNAL(triggered()),this, SLOT(OnCut()));
	connect(ui.SHOW_WIDGET, SIGNAL(triggered()),this, SLOT(OnShowWidget()));
	connect(ui.VIEW_MAXIMIZE, SIGNAL(triggered()),this, SLOT(OnViewMaximize()));
	//connect(ui.Crop, SIGNAL(triggered()),this, SLOT(OnCrop()));

	//distance measure
	connect(ui.DISTANCE_MEASURE, SIGNAL(triggered()), this, SLOT(OnChangeDisFlag()));
	connect(ui.ReconstruView, SIGNAL(disMeasure()), this, SLOT(OnRecDistanceMeasure()));
	connect(ui.AxialView, SIGNAL(disMeasure()), this, SLOT(On2DDistanceMeasure()));
	connect(ui.CoronalView, SIGNAL(disMeasure()), this, SLOT(On2DDistanceMeasure()));
	connect(ui.SagittalView, SIGNAL(disMeasure()), this, SLOT(On2DDistanceMeasure()));
	//angle measure
	connect(ui.ANGLE_MEASURE, SIGNAL(triggered()), this, SLOT(OnChangeAngFlag()));
	connect(ui.ReconstruView, SIGNAL(angMeasure()), this, SLOT(OnRecAngleMeasure()));
	connect(ui.AxialView, SIGNAL(angMeasure()), this, SLOT(On2DAngleMeasure()));
	connect(ui.CoronalView, SIGNAL(angMeasure()), this, SLOT(On2DAngleMeasure()));
	connect(ui.SagittalView, SIGNAL(angMeasure()), this, SLOT(On2DAngleMeasure()));

	connect(pAxialView->mCommand->qSignal, SIGNAL(MeaWidgetMove()), this, SLOT(OnUpdateValue()));
	connect(pCoronalView->mCommand->qSignal, SIGNAL(MeaWidgetMove()), this, SLOT(OnUpdateValue()));
	connect(pSagittalView->mCommand->qSignal, SIGNAL(MeaWidgetMove()), this, SLOT(OnUpdateValue()));
	connect(pReconstruView->mCommand->qSignal, SIGNAL(MeaWidgetMove()), this, SLOT(OnUpdateValue()));

	ui.DISTANCE_MEASURE->setEnabled(true);	ui.ANGLE_MEASURE->setEnabled(true);

	//template generator
	connect(ui.SELECT_PTS, SIGNAL(triggered()), this, SLOT(OnSelectPts()));
	connect(ui.ReconstruView, SIGNAL(fixPoint()), this, SLOT(OnContourWidget()));
	connect(ui.ReconstruView, SIGNAL(EndSelect()), this, SLOT(OnEndSelect()));
	connect(ui.ReconstruView, SIGNAL(DeleteSelect()), this, SLOT(OnDeleteSelect()));
	connect(ui.CLOSE_CONTOURS, SIGNAL(triggered()), this, SLOT(OnEndSelect()));
	connect(ui.DELETE_PTS, SIGNAL(triggered()), this, SLOT(OnDeleteSelect()));
	connect(ui.TEM_GENERATE, SIGNAL(triggered()), this, SLOT(OnTemGenerate()));
	connect(ui.IMPORT_AXIS, SIGNAL(triggered()), this, SLOT(OnImportAxis()));
	connect(ui.PUNCH, SIGNAL(triggered()), this, SLOT(OnPunching()));
	connect(ui.UNDO_PUNCHING, SIGNAL(triggered()), this, SLOT(OnUndoPunching()));
}

void ReconstruMainWindow::OnSetViewMode()
{
	if (ui.VIEW_MODE->isEnabled())
	{
		if(ui.VIEW_MODE->isChecked())
		{
			ui.VIEW_MODE->setChecked(false);
		}
		else
		{
			ui.VIEW_MODE->setChecked(true);
		}

		if(pReconstruView->m_bStyleTrackballCamera)
		{
			pReconstruView->m_bStyleTrackballCamera=false;
			pReconstruView->iren->SetInteractorStyle(pReconstruView->StyleTrackballActor);
			ui.VIEW_MODE->setChecked(false);
		}
		else
		{
			pReconstruView->m_bStyleTrackballCamera=true;
			pReconstruView->iren->SetInteractorStyle(pReconstruView->StyleTrackballCamera);
			ui.VIEW_MODE->setChecked(true);
		}
	}
}

void ReconstruMainWindow::OnFileOpen()
{
	DlgSelectDirectory *dialog=new DlgSelectDirectory;
	dialog->setParent(this,Qt::Dialog);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted) 
	{
		ui.statusbar->showMessage("	Opening files...");
		/*QItemSelectionModel *selectionModel = dialog->ui.SelectDirectoryTreeView->selectionModel();

		QModelIndex index=selectionModel->currentIndex();
		QString directory=model->filePath(index);*/

		ui.progressBar->show();

		if (pAxialView->FileOpen(dialog->m_sDirectory))
		{			
			pCoronalView->ImportImageData(vtkImageData::SafeDownCast(pAxialView->greyPadder->GetInput()));
			pSagittalView->ImportImageData(vtkImageData::SafeDownCast(pAxialView->greyPadder->GetInput()));

			connect(ui.AxialViewScrollBar, SIGNAL(valueChanged(int)),this, SLOT(OnShowSlice()));
			connect(ui.AxialView, SIGNAL(wheelScrolled()),this, SLOT(OnAxialSliceUpdate()));
			connect(ui.CoronalViewScrollBar, SIGNAL(valueChanged(int)),this, SLOT(OnShowSlice()));
			connect(ui.CoronalView, SIGNAL(wheelScrolled()),this, SLOT(OnCoronalSliceUpdate()));
			connect(ui.SagittalViewScrollBar, SIGNAL(valueChanged(int)),this, SLOT(OnShowSlice()));
			connect(ui.SagittalView, SIGNAL(wheelScrolled()),this, SLOT(OnSagittalSliceUpdate()));

			connect(ui.verticalSlider_center, SIGNAL(valueChanged(int)),this, SLOT(OnSetImageWindow(int)));
			connect(ui.verticalSlider_width, SIGNAL(valueChanged(int)),this, SLOT(OnSetImageLevel(int)));
			connect(ui.verticalSlider_threshold_min, SIGNAL(valueChanged(int)),this, SLOT(OnDoThreshold()));
			connect(ui.verticalSlider_threshold_max, SIGNAL(valueChanged(int)),this, SLOT(OnDoThreshold()));

			connect(pAxialView->annotatePick->signal, SIGNAL(Connect()),this, SLOT(OnConnectAxi()));
			connect(pCoronalView->annotatePick->signal, SIGNAL(Connect()),this, SLOT(OnConnectCor()));
			connect(pSagittalView->annotatePick->signal, SIGNAL(Connect()),this, SLOT(OnConnectSag()));
			connect(ui.CONNECT, SIGNAL(triggered()),this, SLOT(OnConnect()));

			ui.AxialViewScrollBar->setRange(0, pAxialView->m_nSliceSum);
			ui.AxialViewScrollBar->setValue(pAxialView->m_nSliceSum/2.0);
			ui.CoronalViewScrollBar->setRange(0, pCoronalView->m_nHeight);
			ui.CoronalViewScrollBar->setValue(pCoronalView->m_nHeight/2.0);
			ui.SagittalViewScrollBar->setRange(0, pSagittalView->m_nWidth);
			ui.SagittalViewScrollBar->setValue(pSagittalView->m_nWidth/2.0);

			pInfoDlg->ui.lineEdit_PatientName->setText(pAxialView->m_sPatientName);
			QString str;
			str.sprintf("%d PXL", pAxialView->m_nWidth);
			pInfoDlg->ui.lineEdit_Width->setText(str);
			str.sprintf("%d PXL", pAxialView->m_nHeight);
			pInfoDlg->ui.lineEdit_Height->setText(str);
			str.sprintf("%.4f mm * %.4f mm", pAxialView->m_fPixelSpacing[0],pAxialView->m_fPixelSpacing[1]);
			pInfoDlg->ui.lineEdit_PixelSpacing->setText(str);
			str.sprintf("%d", pAxialView->m_nSliceSum+1);
			pInfoDlg->ui.lineEdit_SlicesNum->setText(str);
			str.sprintf("%.4f mm", pAxialView->m_fPixelSpacing[2]);
			pInfoDlg->ui.lineEdit_SliceInc->setText(str);

			ui.FILE_OPEN->setEnabled(false);
			ui.SELECT_PTS->setEnabled(true);
			//ui.Crop->setEnabled(true);
		}
		ui.progressBar->hide();
	}



	//delete model;
	//if(dialog!=NULL) 
	//dialog->setAttribute(Qt::WA_DeleteOnClose);
	delete dialog;
}

void ReconstruMainWindow::OnInformation()
{
	if (pInfoDlg->exec() == QDialog::Accepted)
	{
		punchRadius = pInfoDlg->ui.DoubleSpinBox->value()/2.0;
		iniTemplate->thickness = pInfoDlg->ui.DoubleSpinBox_2->value();
		iniTemplate->tubeRadius = pInfoDlg->ui.DoubleSlider_3->value();
	}
	else
	{
		pInfoDlg->ui.DoubleSpinBox->setValue(punchRadius*2);
		pInfoDlg->ui.DoubleSpinBox_2->setValue(iniTemplate->thickness);
		pInfoDlg->ui.DoubleSpinBox_3->setValue(iniTemplate->tubeRadius);
	}
}

void ReconstruMainWindow::OnStlFileOpen()
{
	QString sStl = QFileDialog::getOpenFileName(0,
                                   tr("Open File"), QDir::currentPath(),"*.stl");
       
	if(!sStl.isEmpty())
	{
		ui.progressBar->show();
		QActorObject* pActorObject=new QActorObject();

		pActorObject->isoActor=vtkSmartPointer<vtkActor>::New();
		pActorObject->isoMapper=vtkSmartPointer<vtkPolyDataMapper>::New();			

		vtkSmartPointer<vtkSTLReader> sr=vtkSmartPointer<vtkSTLReader>::New();
		sr->SetFileName(sStl.toAscii ());
		Progress->text="	Opening files...";
		sr->AddObserver(vtkCommand::ProgressEvent, Progress);
		sr->Update();
		sr->ReleaseDataFlagOn();

		//vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		//connectivityFilter->SetInputConnection(sr->GetOutputPort());
		//connectivityFilter->SetExtractionModeToLargestRegion();
		//connectivityFilter->Update();

		//vtkSmartPointer<vtkCleanPolyData>clean = vtkSmartPointer<vtkCleanPolyData>::New();
		//clean->SetInputConnection(connectivityFilter->GetOutputPort());
		//clean->Update();
		//简化骨小梁模型
		//vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
		//mesh->DeepCopy(sr->GetOutput());
		//recut(mesh);

		//vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
		//decimator->SetInput(normals->GetOutput());
		//decimator->SetTargetReduction(0.9);
		//decimator->SplittingOff();
		//decimator->PreserveTopologyOff();
		////Progress->text="	Decimating...";
		////decimator->AddObserver(vtkCommand::ProgressEvent, Progress);
		//decimator->Update();

		//vtkSmartPointer<vtkSTLWriter> stl=vtkSmartPointer<vtkSTLWriter>::New();		
		//stl->SetFileName("AfterDecimating.stl");
		//stl->SetInput(decimator->GetOutput());
		//stl->SetFileType(2);
		////Progress->text="	Writing STL files...";
		////decimator->AddObserver(vtkCommand::ProgressEvent, Progress);
		//stl->Write();

		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInput(sr->GetOutput());
		Progress->text="	Normals generating...";
		normals->AddObserver(vtkCommand::ProgressEvent, Progress);
		normals->Update();

		pActorObject->isoMapper->SetInput(normals->GetOutput());
		pActorObject->isoMapper->ScalarVisibilityOff();
		pActorObject->isoMapper->ImmediateModeRenderingOn();
		pActorObject->isoMapper->Update();
		//stlActor->GetProperty()->SetColor(1, 1, 0.501961);
		//stlActor->GetProperty()->SetColor(0,1,0);
		pActorObject->isoActor->SetMapper(pActorObject->isoMapper);		

		pAxialView->m_Actorlist.append(pActorObject);

		addModel();
		++pAxialView->actorCount;

		ui.TO_MAINWINDOW->setEnabled(true);
		ui.VIEW_MAXIMIZE->setEnabled(true);
		ui.SHOW_WIDGET->setEnabled(true);
		ui.SELECT_PTS->setEnabled(true);

		//pAxialView->SetActorColor(pAxialView->m_Actorlist.count(), pActorObject);
		
		//int nCount=pAxialView->m_Actorlist.count();
		//for (int i=0;i < nCount;i++)
		//{
		//	pActorObject=(QActorObject*) pAxialView->m_Actorlist.at( i );
		//	pAxialView->SetActorColor(i, pActorObject);
		//}

		//pActorObject=(QActorObject*) pAxialView->m_Actorlist.last();
		//pActorObject->isoActor->GetProperty()->SetColor(1, 1, 0.501961);	
		//
		//pReconstruView->ren->AddActor(stlActor);
		pReconstruView->ren->ResetCamera();
		pReconstruView->renWin->Render();

		ui.progressBar->hide();
		ui.statusbar->clearMessage();
		//QString str;
		//str.sprintf("%d",pAxialView->m_Actorlist.count());					
		//ui.modelTable->addItem(str);
		//ui.modelTable->setCurrentRow(pAxialView->m_Actorlist.count()-1);
	}
}

void ReconstruMainWindow::OnSetImageWindow(int nWindow)
{
	pAxialView->greylut->SetWindow(nWindow);
	pCoronalView->greylut->SetWindow(nWindow);
	pSagittalView->greylut->SetWindow(nWindow);
	pAxialView->renWin->Render();
	pCoronalView->renWin->Render();
	pSagittalView->renWin->Render();
}

void ReconstruMainWindow::OnSetImageLevel(int nLevel)
{
	pAxialView->greylut->SetLevel(nLevel);
	pCoronalView->greylut->SetLevel(nLevel);
	pSagittalView->greylut->SetLevel(nLevel);
	pAxialView->renWin->Render();
	pCoronalView->renWin->Render();
	pSagittalView->renWin->Render();
}

void ReconstruMainWindow::OnDoThreshold()
{
	pAxialView->m_nThreshMin=ui.verticalSlider_threshold_min->value();
	pAxialView->m_nThreshMax=ui.verticalSlider_threshold_max->value();
	pAxialView->DoThreshold();
	pCoronalView->m_nThreshMin=ui.verticalSlider_threshold_min->value();
	pCoronalView->m_nThreshMax=ui.verticalSlider_threshold_max->value();
	pCoronalView->DoThreshold(vtkImageData::SafeDownCast(pAxialView->thresholdPadder->GetInput()));
	pSagittalView->m_nThreshMin=ui.verticalSlider_threshold_min->value();
	pSagittalView->m_nThreshMax=ui.verticalSlider_threshold_max->value();
	pSagittalView->DoThreshold(vtkImageData::SafeDownCast(pAxialView->thresholdPadder->GetInput()));
	ui.CONNECT->setEnabled(true);
	//ui.RECONSTRU3D->setEnabled(true);

	//ui.Crop->setEnabled(false);
}

void ReconstruMainWindow::OnShowSlice()
{
	pAxialView->m_nSlice=ui.AxialViewScrollBar->value();
	pAxialView->ShowSlice();
	pCoronalView->m_nSlice=ui.CoronalViewScrollBar->value();
	pCoronalView->ShowSlice();
	pSagittalView->m_nSlice=ui.SagittalViewScrollBar->value();
	pSagittalView->ShowSlice();

	double SagittalPosition = pSagittalView->m_nSlice/(double)pSagittalView->m_nWidth;
	double CoronalPosition = pCoronalView->m_nSlice/(double)pCoronalView->m_nHeight;
	double AxialPosition = pAxialView->m_nSlice/(double)pAxialView->m_nSliceSum;
	double AxiScale = pAxialView->fScale;
	double CorScale = pCoronalView->fScale;
	double SagScale = pSagittalView->fScale;

	pAxialView->CoronalLineActor->SetPosition(0,CoronalPosition*AxiScale,0);
	pAxialView->SagittalLineActor->SetPosition(SagittalPosition,0,0);
	pCoronalView->AxialLineActor->SetPosition(0,AxialPosition*CorScale,0);
	pCoronalView->SagittalLineActor->SetPosition(SagittalPosition,0,0);
	pSagittalView->AxialLineActor->SetPosition(0,AxialPosition*SagScale,0);
	pSagittalView->CoronalLineActor->SetPosition(CoronalPosition,0,0);

	OnUpdate2DMeasure();
}

void ReconstruMainWindow::OnConnect()
{
	if (ui.CONNECT->isChecked())
	{
	}
	else
	{
		//pAxialView->ren->RemoveActor(pAxialView->segmentOverlayActor);
		//pCoronalView->ren->RemoveActor(pCoronalView->segmentOverlayActor);
		//pSagittalView->ren->RemoveActor(pSagittalView->segmentOverlayActor);
		//pAxialView->renWin->Render();
		//pCoronalView->renWin->Render();
		//pSagittalView->renWin->Render();
	}
}

void ReconstruMainWindow::OnConnectAxi()
{
	if (ui.DISTANCE_MEASURE->isChecked())return;
	if (ui.ANGLE_MEASURE->isChecked())return;
	if (ui.CONNECT->isChecked())
	{
		ui.progressBar->show();
		int xp,yp,zp;
		double *pPickPoint=pAxialView->picker->GetPickPosition();
		double fScale=pAxialView->fScale;
		xp = (int)((pPickPoint[0]+0.5)*pAxialView->m_nWidth);
		yp = (int)((pPickPoint[1]+0.5*fScale)*pAxialView->m_nHeight/fScale);
		zp = ui.AxialViewScrollBar->value();
		pAxialView->Connect(xp,yp,zp);
		pCoronalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));
		pSagittalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));

		ui.RECONSTRU3D->setEnabled(true);
		//ui.DELETE_ACTOR->setEnabled(true);
		ui.progressBar->hide();
		ui.statusbar->clearMessage();
	}
	else
	{
		double fScale=pAxialView->fScale;
		double *pPickPoint=pAxialView->picker->GetPickPosition();
		ui.SagittalViewScrollBar->setValue((int)((pPickPoint[0]+0.5)*pAxialView->m_nWidth));
		ui.CoronalViewScrollBar->setValue((int)((pPickPoint[1]+0.5*fScale)*pAxialView->m_nHeight/fScale));
	}
}
void ReconstruMainWindow::OnConnectCor()
{
	if (ui.DISTANCE_MEASURE->isChecked())return;
	if (ui.ANGLE_MEASURE->isChecked())return;
	if (ui.CONNECT->isChecked())
	{
		ui.progressBar->show();
		int xp,yp,zp;
		double *pPickPoint=pCoronalView->picker->GetPickPosition();
		double fScale=pCoronalView->fScale;
		xp=(int)((pPickPoint[0]+0.5)*pCoronalView->m_nWidth);
		yp=ui.CoronalViewScrollBar->value();
		zp=(int)((pPickPoint[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale);
		pAxialView->Connect(xp,yp,zp);
		pCoronalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));
		pSagittalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));

		ui.RECONSTRU3D->setEnabled(true);
		//ui.DELETE_ACTOR->setEnabled(true);
		ui.progressBar->hide();
		ui.statusbar->clearMessage();
	}
	else
	{
		double fScale=pCoronalView->fScale;
		double *pPickPoint=pCoronalView->picker->GetPickPosition();
		ui.SagittalViewScrollBar->setValue((int)((pPickPoint[0]+0.5)*pCoronalView->m_nWidth));
		ui.AxialViewScrollBar->setValue((int)((pPickPoint[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale));
	}
}
void ReconstruMainWindow::OnConnectSag()
{
	if (ui.DISTANCE_MEASURE->isChecked())return;
	if (ui.ANGLE_MEASURE->isChecked())return;
	if (ui.CONNECT->isChecked())
	{
		ui.progressBar->show();
		int xp,yp,zp;
		double *pPickPoint=pSagittalView->picker->GetPickPosition();
		double fScale=pSagittalView->fScale;
		xp=ui.SagittalViewScrollBar->value();
		yp=(int)((pPickPoint[0]+0.5)*pSagittalView->m_nHeight);
		zp=(int)((pPickPoint[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale);
		pAxialView->Connect(xp,yp,zp);
		pCoronalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));
		pSagittalView->Connect(vtkImageData::SafeDownCast(pAxialView->segmentPadder->GetInput()));

		ui.RECONSTRU3D->setEnabled(true);
		//ui.DELETE_ACTOR->setEnabled(true);
		ui.progressBar->hide();
		ui.statusbar->clearMessage();
	}
	else
	{
		double fScale=pSagittalView->fScale;
		double *pPickPoint=pSagittalView->picker->GetPickPosition();
		ui.CoronalViewScrollBar->setValue((int)((pPickPoint[0]+0.5)*pSagittalView->m_nHeight));
		ui.AxialViewScrollBar->setValue((int)((pPickPoint[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale));
	}
}

void ReconstruMainWindow::OnDeleteModel()
{
	if(!pAxialView->m_Actorlist.isEmpty())	
	{
		int row = ui.modelTable->currentRow();
		if(row >= 0)
		{
			//remove actors
			pReconstruView->ren->RemoveActor(pAxialView->m_Actorlist.at(row)->isoActor);
			pReconstruView->renWin->Render();
			pAxialView->m_Actorlist.removeAt(row);
			//update table
			ui.modelTable->removeRow(row);
		}
	}
}

void ReconstruMainWindow::OnDeleteMeasure()
{
	if(!meaList.isEmpty())	
	{
		int row = ui.measureTable->currentRow();
		if(row >= 0)
		{
			//remove measurement
			meaList.at(row)->disWidget3D->Off();
			meaList.at(row)->disWidget3D->Off();
			meaList.at(row)->angWidget3D->Off();
			meaList.at(row)->angWidget3D->Off();
			pReconstruView->renWin->Render();
			meaList.removeAt(row);
			//update table
			ui.measureTable->removeRow(row);
			++delMeaCount;
		}
	}
}

void ReconstruMainWindow::OnAxialSliceUpdate()
{	
	if(ui.AxialView->m_value<0)
		ui.AxialViewScrollBar->setValue(ui.AxialViewScrollBar->value()+1);
	else
		ui.AxialViewScrollBar->setValue(ui.AxialViewScrollBar->value()-1);
}
void ReconstruMainWindow::OnCoronalSliceUpdate()
{	
	if(ui.CoronalView->m_value<0)
		ui.CoronalViewScrollBar->setValue(ui.CoronalViewScrollBar->value()+1);
	else
		ui.CoronalViewScrollBar->setValue(ui.CoronalViewScrollBar->value()-1);
}
void ReconstruMainWindow::OnSagittalSliceUpdate()
{	
	if(ui.SagittalView->m_value<0)
		ui.SagittalViewScrollBar->setValue(ui.SagittalViewScrollBar->value()+1);
	else
		ui.SagittalViewScrollBar->setValue(ui.SagittalViewScrollBar->value()-1);
}

void ReconstruMainWindow::OnReconstru()
{
	ui.progressBar->show();
	pAxialView->Reconstru();

	ui.TO_MAINWINDOW->setEnabled(true);
	ui.VIEW_MAXIMIZE->setEnabled(true);
	ui.SHOW_WIDGET->setEnabled(true);

	//ui.CONNECT->setEnabled(false);
	ui.RECONSTRU3D->setEnabled(false);

	//QString str;
	//str.sprintf("%d",pAxialView->m_Actorlist.count());					
	//ui.modelTable->addItem(str);
	//ui.modelTable->setCurrentRow(nCount-1);

	addModel();
	++pAxialView->actorCount;

	pAxialView->ren->RemoveActor(pAxialView->segmentOverlayActor);
	pCoronalView->ren->RemoveActor(pCoronalView->segmentOverlayActor);
	pSagittalView->ren->RemoveActor(pSagittalView->segmentOverlayActor);
	pAxialView->renWin->Render();
	pCoronalView->renWin->Render();
	pSagittalView->renWin->Render();

	pReconstruView->ren->ResetCamera();
	pReconstruView->renWin->Render();
	ui.progressBar->hide();
	ui.statusbar->clearMessage();
	ui.CONNECT->setChecked(false);
	OnConnect();
}
void ReconstruMainWindow::addModel()
{
	int nCount=pAxialView->m_Actorlist.count();
	QActorObject *pActorObject = (QActorObject*) pAxialView->m_Actorlist.last();

	pAxialView->SetActorColor(pAxialView->actorCount+1, pActorObject);
	pReconstruView->ren->AddActor(pActorObject->isoActor);

	//visibility
	pActorObject->checkBox = new QCheckBox();
	QWidget *checkBoxWidget = new QWidget();
	QHBoxLayout *checkBoxLayout = new QHBoxLayout();
	checkBoxLayout->addWidget(pActorObject->checkBox);  
	checkBoxLayout->setMargin(0);
	checkBoxLayout->setAlignment(pActorObject->checkBox, Qt::AlignCenter);
	checkBoxWidget->setLayout(checkBoxLayout);
	pActorObject->checkBox->setChecked(true);

	//color
	pActorObject->colorPickerButton = new ctkColorPickerButton();
	QWidget *colorWidget = new QWidget();
	QHBoxLayout *colorLayout = new QHBoxLayout();
	colorLayout->addWidget(pActorObject->colorPickerButton);  
	colorLayout->setMargin(0);
	colorLayout->setAlignment(pActorObject->colorPickerButton, Qt::AlignCenter);
	colorWidget->setLayout(colorLayout);
	pActorObject->colorPickerButton->setEnabled(true);
	pActorObject->colorPickerButton->setFixedSize(13,13);
	pActorObject->colorPickerButton->setCheckable(true);
	pActorObject->colorPickerButton->setChecked(false);
	pActorObject->colorPickerButton->setAutoDefault(false);
	pActorObject->colorPickerButton->setFlat(true);
	pActorObject->colorPickerButton->setDisplayColorName(false);
	
	//opacity
	pActorObject->opacSpin = new QSpinBox();// the spinbox is the parent of the popup
	QWidget *opacWidget = new QWidget();// the popup is placed relative to the spinbox
	QHBoxLayout *opacLayout = new QHBoxLayout();
	opacLayout->addWidget(pActorObject->opacSpin);
	opacLayout->setMargin(0);
	opacLayout->setContentsMargins(0,0,0,0);
	opacWidget->setLayout(opacLayout);
	pActorObject->opacSpin->setAlignment(Qt::AlignCenter);
	pActorObject->opacSpin->setAccelerated(true);
	pActorObject->opacSpin->setMaximum(100);
	pActorObject->opacSpin->setValue(100);
	pActorObject->opacSpin->setFrame(false);
	pActorObject->opacPopup = new ctkPopupWidget(pActorObject->opacSpin);
	QHBoxLayout* popupLayout = new QHBoxLayout(pActorObject->opacPopup);
	popupLayout->setContentsMargins(0,0,0,0);
	// populate the popup with a vertical QSlider
	pActorObject->popupSlider = new QSlider(Qt::Horizontal,pActorObject->opacPopup);
	pActorObject->popupSlider->setMaximum(100);
	pActorObject->popupSlider->setValue(100);
	pActorObject->popupSlider->setFixedSize(50,10);
	// add here the signal/slot connection between the slider and the spinbox
	connect(pActorObject->popupSlider, SIGNAL(valueChanged(int)), pActorObject->opacSpin, SLOT(setValue(int)));
	connect(pActorObject->opacSpin, SIGNAL(valueChanged(int)), pActorObject->popupSlider, SLOT(setValue(int)));
	popupLayout->addWidget(pActorObject->popupSlider);
	pActorObject->opacPopup->setAlignment(Qt::AlignBottom | Qt::AlignHCenter); // at the top and center
	pActorObject->opacPopup->setHorizontalDirection( Qt::LeftToRight ); // open outside the parent
	pActorObject->opacPopup->setVerticalDirection(ctkBasePopupWidget::TopToBottom); // at the left of the spinbox sharing the top border
	pActorObject->opacPopup->setFixedSize(60,20);
	pActorObject->opacPopup->setFrameShadow(QFrame::Sunken);
	// Control the animation
	pActorObject->opacPopup->setAnimationEffect(ctkBasePopupWidget::ScrollEffect); // could also be ScrollEffect/FadeEffect
	pActorObject->opacPopup->setOrientation(Qt::Vertical); // how to animate, could be Qt::Vertical or Qt::Horizontal|Qt::Vertical
	pActorObject->opacPopup->setEasingCurve(QEasingCurve::InCubic); // how to accelerate the animation, QEasingCurve::Type/OutBounce/OutInBounce
	pActorObject->opacPopup->setEffectDuration(500); // how long in ms.
	// Control the behavior
	pActorObject->opacPopup->setAutoShow(true); // automatically open when the mouse is over the spinbox
	pActorObject->opacPopup->setAutoHide(true); // automatically hide when the mouse leaves the popup or the spinbox.

	//add to model table
	ui.modelTable->insertRow(nCount-1);
	ui.modelTable->setCellWidget(nCount-1,0,checkBoxWidget);
	ui.modelTable->setCellWidget(nCount-1,1,colorWidget);
	ui.modelTable->setCellWidget(nCount-1,2,opacWidget);

	//connect(ui.modelTable,SIGNAL(cellClicked(int,int)),this,SLOT(OnModelTable(int,int)));
	connect(pActorObject->checkBox,SIGNAL(clicked()),this,SLOT(OnUpdateModelVisib()));
	connect(pActorObject->colorPickerButton,SIGNAL(colorChanged(QColor)),this,SLOT(OnUpdateColorOfModel(QColor)));
	connect(pActorObject->opacSpin, SIGNAL(valueChanged(int)), this, SLOT(OnUpdateOpacity(int)));

	//update the color of the last model actor
	double color[3];
	pActorObject->isoActor->GetProperty()->GetColor(color);
	pActorObject->colorPickerButton->setColor(QColor::fromRgb(color[0]*255,color[1]*255,color[2]*255));

	ui.modelTable->setCurrentCell(nCount-1,0);
}
void ReconstruMainWindow::OnUpdateModelVisib()
{
	int nCount = pAxialView->m_Actorlist.count();
	for(int i = 0; i < nCount; ++i)
	{
		if (pAxialView->m_Actorlist.at(i)->checkBox->isChecked())
		{
			pAxialView->m_Actorlist.at(i)->isoActor->VisibilityOn();
		}
		else
		{
			pAxialView->m_Actorlist.at(i)->isoActor->VisibilityOff();
		}
	}
	pReconstruView->renWin->Render();
}
void ReconstruMainWindow::OnUpdateColorOfModel(QColor)
{
	int nCount = pAxialView->m_Actorlist.count();
	for (int i = 0; i < nCount; ++i)
	{
		double color[3];
		color[0] = (double)pAxialView->m_Actorlist.at(i)->colorPickerButton->color().red()/255.0;
		color[1] = (double)pAxialView->m_Actorlist.at(i)->colorPickerButton->color().green()/255.0;
		color[2] = (double)pAxialView->m_Actorlist.at(i)->colorPickerButton->color().blue()/255.0;
		pAxialView->m_Actorlist.at(i)->isoActor->GetProperty()->SetColor(color);
	}
	pReconstruView->renWin->Render();
}
void ReconstruMainWindow::OnUpdateColorList()
{
	int nCount = pAxialView->m_Actorlist.count();
	for (int i = 0; i < nCount; ++i)
	{
		double *color;
		color = pAxialView->m_Actorlist.at(i)->isoActor->GetProperty()->GetColor();
		pAxialView->m_Actorlist.at(i)->colorPickerButton->setColor(QColor::fromRgb(color[0]*255,color[1]*255,color[2]*255));
	}
}
void ReconstruMainWindow::OnUpdateOpacity(int opc)
{
	int nCount = pAxialView->m_Actorlist.count();
	for (int i = 0; i < nCount; ++i)
	{
		if(pAxialView->m_Actorlist.at(i)->opacSpin->value() == opc)
		//double opc = (double)pAxialView->m_Actorlist.at(i)->opacSpin->value();
		pAxialView->m_Actorlist.at(i)->isoActor->GetProperty()->SetOpacity(opc/100.0);
	}
	pReconstruView->renWin->Render();
}

void ReconstruMainWindow::initializePlaneWidget()
{
	//double fW=(pAxialView->m_nWidth-1)*pAxialView->m_fPixelSpacing[0];
	//double fH=(pAxialView->m_nHeight-1)*pAxialView->m_fPixelSpacing[1];
	//double fZ=pAxialView->m_nSliceSum*pAxialView->m_fPixelSpacing[2];
	//pReconstruView->planeWidget->SetCenter(fW/2.0f,2*fH/3.0f,fZ/2.0f);
	double *center;
	if (!pAxialView->m_Actorlist.isEmpty())
	{
		center = pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoActor->GetCenter();
	}

	double *bounds = pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoActor->GetBounds();
	//double *fActorBounds=pAxialView->isoActor->GetBounds() ;
	//pReconstruView->planeWidget->SetPoint1(fW,2*fH/3.0f,0);
	//pReconstruView->planeWidget->SetPoint2(0,2*fH/3.0f,fZ);
	//pReconstruView->planeWidget->NormalToYAxisOn();
	//pReconstruView->planeWidget->PlaceWidget(pAxialView->isoActor->GetBounds()); 

	double lenth = (bounds[1]-bounds[0]+bounds[3]-bounds[2])/4.0;
	pReconstruView->planeWidget->SetOrigin(bounds[0]-lenth,bounds[2]-lenth,center[2]);
	pReconstruView->planeWidget->SetPoint1(bounds[1]+lenth,bounds[2]-lenth,center[2]);
	pReconstruView->planeWidget->SetPoint2(bounds[0]-lenth,bounds[3]+lenth,center[2]);
	pReconstruView->planeWidget->SetResolution(200);
	pReconstruView->planeWidget->SetPlaceFactor(0.001f);
	pReconstruView->planeWidget->SetHandleSize(0.01f);
	pReconstruView->planeWidget->SetRepresentationToSurface();
	pReconstruView->planeWidget->PlaceWidget();
}

void ReconstruMainWindow::OnSetCurrentActorListItem()
{}

void ReconstruMainWindow::OnShowWidget()
{
	if(ui.SHOW_WIDGET->isChecked())
	{
		//ui.SHOW_WIDGET->setChecked(true);
		ui.CUT->setEnabled(true);
		initializePlaneWidget();
		pReconstruView->planeWidget->On ();
	}
	else
	{
		//ui.SHOW_WIDGET->setChecked(false);
		ui.CUT->setEnabled(false);
		pReconstruView->planeWidget->Off ();
	}

	
	//QKeyEvent *eKeyEvent=new QKeyEvent(QEvent::KeyPress,Qt::Key_I,Qt::NoModifier,QString(QChar(Qt::Key_I)));
	//ui.ReconstruView->setFocus();
	//ui.ReconstruView->keyPressEvent(eKeyEvent);
	////ui.ReconstruView->GetRenderWindow()->keyPressEvent(eKeyEvent);
	//delete eKeyEvent;
}

void ReconstruMainWindow::OnViewMaximize()
{
	if(ui.VIEW_MAXIMIZE->isChecked())
	{
		ui.VIEW_MAXIMIZE->setChecked(true);
		ui.widget_panel->setEnabled(false);
		ui.AxialView->hide();
		ui.AxialViewScrollBar->hide();
		ui.CoronalView->hide();
		ui.CoronalViewScrollBar->hide();
		ui.SagittalView->hide();
		ui.SagittalViewScrollBar->hide();
	}
	else
	{
		ui.VIEW_MAXIMIZE->setChecked(false);
		ui.widget_panel->setEnabled(true);
		ui.AxialView->show();
		ui.AxialViewScrollBar->show();
		ui.CoronalView->show();
		ui.CoronalViewScrollBar->show();
		ui.SagittalView->show();
		ui.SagittalViewScrollBar->show();
	}
}

void ReconstruMainWindow::OnExportStl()
{
	ExportStl();	
}

void ReconstruMainWindow::OnCut()
{
	if(!pAxialView->m_Actorlist.isEmpty())	
	{
		int row = ui.modelTable->currentRow();
		if(row >= 0)
		{
			QActorObject* pActorObject=(QActorObject*) pAxialView->m_Actorlist.at(row);
			vtkSmartPointer<vtkMatrix4x4> matrix4 = vtkSmartPointer<vtkMatrix4x4>::New();
			matrix4 = pActorObject->isoActor->GetMatrix();

			vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
			transform->SetMatrix(matrix4);
			vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
			transformPolyData->SetInput((vtkPolyData *) pActorObject->isoActor->GetMapper()->GetInput());
			transformPolyData->SetTransform(transform);

			vtkSmartPointer<vtkPlane> planeOfWidget = vtkSmartPointer<vtkPlane>::New();
			pReconstruView->planeWidget->GetPlane(planeOfWidget);

			////clipper
			//vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
			//clipper->SetInput(Normals->GetOutput());
			//clipper->SetClipFunction(planeOfWidget);
			//clipper->GenerateClipScalarsOn();
			//clipper->SetValue(0.1);

			//closed clipper
			vtkSmartPointer<vtkPlaneCollection> planeList = vtkSmartPointer<vtkPlaneCollection>::New();
			planeList->AddItem(planeOfWidget);
			vtkSmartPointer<vtkClipClosedSurface> closeClip = vtkSmartPointer<vtkClipClosedSurface>::New();
			closeClip->SetClippingPlanes(planeList);
			closeClip->SetInput(transformPolyData->GetOutput());
			closeClip->TriangulationErrorDisplayOff();

			vtkSmartPointer<vtkPolyDataNormals> Normals=vtkSmartPointer<vtkPolyDataNormals>::New();	
			Normals->SetInput(closeClip->GetOutput());
			Normals->SetFeatureAngle(60.0);
			Normals->Update();

			//cutter
			//vtkSmartPointer<vtkCutter> cutEdges = vtkSmartPointer<vtkCutter>::New();
			//cutEdges->SetInput(Normals->GetOutput());
			//cutEdges->SetCutFunction(planeOfWidget);
			//cutEdges->GenerateCutScalarsOn();
			//cutEdges->SetValue(0,0.1);
			////cutter->Update();

			//vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
			//stripper->SetInput(cutEdges->GetOutput());
			//stripper->Update();

			//vtkSmartPointer<vtkPolyData> cutPoly = vtkSmartPointer<vtkPolyData>::New();
			//cutPoly->SetPoints(stripper->GetOutput()->GetPoints());
			//cutPoly->SetPolys(stripper->GetOutput()->GetLines());
			//cutPoly->Update();

			//vtkSmartPointer<vtkSurfaceReconstructionFilter> del = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
			//del->SetInput(cutPoly);
			//vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
			//contour->SetInput(del->GetOutput());
			//contour->SetValue(0,0.0);

			//vtkSmartPointer<vtkTriangleFilter> cutTriangles = vtkSmartPointer<vtkTriangleFilter>::New();
			//cutTriangles->SetInput(cutPoly);
			//triCut->Update();

			//vtkSmartPointer<vtkAppendPolyData> appendPolyData = vtkSmartPointer<vtkAppendPolyData>::New();
			//appendPolyData->AddInput(clipper->GetOutput());
			//appendPolyData->AddInput(cutTriangles->GetOutput());

			//vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanData = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
			//booleanData->SetInput(0,clipper->GetOutput());
			//booleanData->SetInput(1,triCut->GetOutput());
			//booleanData->SetOperationToUnion();

			pActorObject->isoMapper->RemoveAllInputs();
			pActorObject->isoMapper->SetInput(Normals->GetOutput());
			pActorObject->isoMapper->Update();

			pActorObject->isoActor->SetMapper(pActorObject->isoMapper);
			matrix4->Invert();
			pActorObject->isoActor->SetUserMatrix(matrix4);
		}
	}
	pReconstruView->renWin->Render();
}

void ReconstruMainWindow::ExportStl()
{
	QString fileName="test.stl";

    if (fileName.isEmpty())
        return;
    else 
	{
		if(!pAxialView->m_Actorlist.isEmpty())	
		{	
			int row = ui.modelTable->currentRow();
			if(row >= 0)
			{
			vtkSTLWriter *stl=vtkSTLWriter::New();
			vtkTransformPolyDataFilter *TransformPolyDataFilter = vtkTransformPolyDataFilter::New();
			vtkTransform *Transform=vtkTransform::New();
			vtkMatrix4x4 *Matrix;
			vtkTriangleFilter *triangles=vtkTriangleFilter::New();			

			QActorObject* pActorObject=(QActorObject*) pAxialView->m_Actorlist.at(row);
			TransformPolyDataFilter->SetInput((vtkPolyData *) pActorObject->isoActor->GetMapper()->GetInput());
			
			stl->SetFileName(qPrintable(fileName));

			Matrix=pActorObject->isoActor->GetMatrix();
			Transform->SetMatrix(Matrix);
			TransformPolyDataFilter->SetTransform(Transform);
			triangles->SetInput(TransformPolyDataFilter->GetOutput());

			stl->SetInput(triangles->GetOutput());
			stl->SetFileType(2);
			stl->Write();

			//QMessageBox::information(0, tr("Information"),tr("The stl files are successfully exported!"));
			stl->Delete();
			TransformPolyDataFilter->Delete();
			Transform->Delete();
			//Matrix->Delete();
			triangles->Delete();
			}
		}
	}
}

void ReconstruMainWindow::OnExportToMainWindow()
{
	ExportStl();

	QString PAth = QDir::currentPath()+"/test.stl";
	PAth.prepend("\"").append("\"");
	LPCWSTR path = PAth.utf16();

	/*HINSTANCE nFlag = */ShellExecute(NULL,L"open",L"C:\\Program Files\\Cura_14.01\\cura.bat",
		path,L"C:\\Program Files\\Cura_14.01",SW_HIDE);

	//if(nFlag)
	//{
	//	QTime t; 
	//	t.start(); 
	//	while(t.elapsed()<15000) 
	//		QCoreApplication::processEvents();

	//	QFile f;
	//	f.remove("test.stl");
	//}


	//DlgSelectSurgeryType *dialog=new DlgSelectSurgeryType;
	//dialog->setParent(this,Qt::Dialog);	
	//
	//dialog->show();

	//if (dialog->exec() == QDialog::Accepted) 
	//{
	//	dialog->AcceptData();
	//	//m_bMandiblePlan=dialog->m_bMandiblePlan;
	//	//pAxialView->ren->RemoveActor(pAxialView->segmentOverlayActor);
	//	//pAxialView->ren->RemoveActor(pAxialView->thresholdOverlayActor);
	//	//pAxialView->ren->RemoveActor(pAxialView->greyActor);
	//	////pAxialView->renWin->Render();

	//	//pReconstruView->ren->RemoveActor(pAxialView->isoActor);	
	//	////pReconstruView->renWin->Render();

	//	//pAxialView->Transform->Delete();
	//	//pAxialView->Matrix->Delete();

	//	//pAxialView->greyPadder->Delete();		
	//	//pAxialView->greylut->Delete();		
	//	//pAxialView->greyActor->Delete();

	//	//pAxialView->segmentPadder->Delete();		
	//	//pAxialView->segmentOverlayActor->Delete();

	//	//pAxialView->thresholdPadder->Delete();		
	//	//pAxialView->thresholdOverlayActor->Delete();

	//	//pAxialView->SliceNumMapper->Delete();pAxialView->SliceZvalueMapper->Delete();
	//	//pAxialView->SliceNumActor->Delete();pAxialView->SliceZvalueActor->Delete();	
	//	//pAxialView->Threshold->Delete();		
	//	//pAxialView->DilateErode3D->Delete();		

	//	//pAxialView->isoActor->Delete();
	//	m_bTransferSucceed=true;
	//	this->close();

	//	if(pAxialView->m_bCropImage)
	//		emit dataTransfer((vtkImageImport *)pAxialView->VTKResampleImporter,(vtkPolyDataMapper *)pAxialView->isoMapper,(QString) pAxialView->m_sPatientName);
	//	else
	//		emit dataTransfer((vtkImageImport *)pAxialView->VTKImporter,(vtkPolyDataMapper *)pAxialView->isoMapper,(QString) pAxialView->m_sPatientName);
	//}

	//delete dialog;	
}

void ReconstruMainWindow::advanceProgressBar(int nProgress)
{    
    ui.progressBar->setValue(nProgress);
}
void ReconstruMainWindow::advanceStatusBar(QString text)
{
	ui.statusbar->showMessage(text);
}

void ReconstruMainWindow::On2DDistanceMeasure()
{
	if (ui.FILE_OPEN->isEnabled())
		return;

	if(ui.DISTANCE_MEASURE->isChecked())
	{
		if (ui.AxialView->disFlag)
		{
			if (ui.AxialView->dpTimes == 1)
			{
				double *pt = pAxialView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.AxialView->dpTimes == 2)
			{
				double *pt = pAxialView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];

				pMeasureObj->disWidget3D->SetInteractor(pAxialView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->disWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->disRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->disRep3D->SetPoint2WorldPosition(pMeasureObj->p2);

				pMeasureObj->disWidget3D->AddObserver(vtkCommand::InteractionEvent ,pAxialView->mCommand);
				pMeasureObj->disWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pAxialView->mCommand);

				double fScale=(double)pAxialView->m_nHeight/(double)pAxialView->m_nWidth;
				double *r1;	double *r2;
				r1 = pMeasureObj->disRep3D->GetPoint1WorldPosition();
				double xp1 = (r1[0]+0.5)*pAxialView->m_nWidth*pAxialView->m_fPixelSpacing[0];
				double yp1 = (r1[1]+0.5*fScale)*pAxialView->m_nHeight/fScale*pAxialView->m_fPixelSpacing[1];
				r2 = pMeasureObj->disRep3D->GetPoint2WorldPosition();
				double xp2 = (r2[0]+0.5)*pAxialView->m_nWidth*pAxialView->m_fPixelSpacing[0];
				double yp2 = (r2[1]+0.5*fScale)*pAxialView->m_nHeight/fScale*pAxialView->m_fPixelSpacing[1];
				pMeasureObj->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));

				QString str;	str.sprintf("D_%d: %.2lf", pMeasureObj->num, pMeasureObj->disValue);
				QByteArray ba = str.toLatin1();
				char *lable = ba.data();
				pMeasureObj->disRep3D->SetLabelFormat(lable);

				pMeasureObj->disWidget3D->On();

				pMeasureObj->is2D = true;
				pMeasureObj->isAxi = true;
				pMeasureObj->isAngle = false;
				pMeasureObj->nSlice = pAxialView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.AxialView->dpTimes = 0;
				ui.AxialView->disFlag = false;
				ui.DISTANCE_MEASURE->setChecked(false);
			}
		}
		if (ui.CoronalView->disFlag)
		{
			if (ui.CoronalView->dpTimes == 1)
			{
				double *pt = pCoronalView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.CoronalView->dpTimes == 2)
			{
				double *pt = pCoronalView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];

				pMeasureObj->disWidget3D->SetInteractor(pCoronalView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->disWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->disRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->disRep3D->SetPoint2WorldPosition(pMeasureObj->p2);

				pMeasureObj->disWidget3D->AddObserver(vtkCommand::InteractionEvent ,pCoronalView->mCommand);
				pMeasureObj->disWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pCoronalView->mCommand);

				double fScale = pCoronalView->fScale;
				double *r1;	double *r2;
				r1 = pMeasureObj->disRep3D->GetPoint1WorldPosition();
				double xp1 = (r1[0]+0.5)*pCoronalView->m_nWidth*pCoronalView->m_fPixelSpacing[0];
				double yp1 = (r1[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale*pCoronalView->m_fPixelSpacing[2];
				r2 = pMeasureObj->disRep3D->GetPoint2WorldPosition();
				double xp2 = (r2[0]+0.5)*pCoronalView->m_nWidth*pCoronalView->m_fPixelSpacing[0];
				double yp2 = (r2[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale*pCoronalView->m_fPixelSpacing[2];
				pMeasureObj->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));


				QString str;	str.sprintf("D_%d: %.2lf", pMeasureObj->num, pMeasureObj->disValue);
				QByteArray ba = str.toLatin1();
				char *lable = ba.data();
				pMeasureObj->disRep3D->SetLabelFormat(lable);

				pMeasureObj->disWidget3D->On();

				pMeasureObj->is2D = true;
				pMeasureObj->isCor = true;
				pMeasureObj->isAngle = false;
				pMeasureObj->nSlice = pCoronalView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.CoronalView->dpTimes = 0;
				ui.CoronalView->disFlag = false;
				ui.DISTANCE_MEASURE->setChecked(false);
			}
		}
		if (ui.SagittalView->disFlag)
		{
			if (ui.SagittalView->dpTimes == 1)
			{
				double *pt = pSagittalView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.SagittalView->dpTimes == 2)
			{
				double *pt = pSagittalView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];

				pMeasureObj->disWidget3D->SetInteractor(pSagittalView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->disWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->disRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->disRep3D->SetPoint2WorldPosition(pMeasureObj->p2);

				pMeasureObj->disWidget3D->AddObserver(vtkCommand::InteractionEvent ,pSagittalView->mCommand);
				pMeasureObj->disWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pSagittalView->mCommand);

				double fScale = pSagittalView->fScale;
				double *r1;	double *r2;
				r1 = pMeasureObj->disRep3D->GetPoint1WorldPosition();
				double xp1 = (r1[0]+0.5)*pSagittalView->m_nHeight*pSagittalView->m_fPixelSpacing[1];
				double yp1 = (r1[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale*pSagittalView->m_fPixelSpacing[2];
				r2 = pMeasureObj->disRep3D->GetPoint2WorldPosition();
				double xp2 = (r2[0]+0.5)*pSagittalView->m_nHeight*pSagittalView->m_fPixelSpacing[1];
				double yp2 = (r2[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale*pSagittalView->m_fPixelSpacing[2];
				pMeasureObj->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));

				QString str;	str.sprintf("D_%d: %.2lf", pMeasureObj->num, pMeasureObj->disValue);
				QByteArray ba = str.toLatin1();
				char *lable = ba.data();
				pMeasureObj->disRep3D->SetLabelFormat(lable);

				pMeasureObj->disWidget3D->On();

				pMeasureObj->is2D = true;
				pMeasureObj->isSag = true;
				pMeasureObj->isAngle = false;
				pMeasureObj->nSlice = pSagittalView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.SagittalView->dpTimes = 0;
				ui.SagittalView->disFlag = false;
				ui.DISTANCE_MEASURE->setChecked(false);
			}
		}
	}
}
void ReconstruMainWindow::OnRecDistanceMeasure()
{
	if(ui.DISTANCE_MEASURE->isChecked())
	{
		if (ui.ReconstruView->disFlag)
		{
			if (ui.ReconstruView->dpTimes == 1)
			{
				double *pt = pReconstruView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2];
			}
			if (ui.ReconstruView->dpTimes == 2)
			{
				double *pt = pReconstruView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2];

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];

				pMeasureObj->disWidget3D->SetInteractor(pReconstruView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->disWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->disRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->disRep3D->SetPoint2WorldPosition(pMeasureObj->p2);

				pMeasureObj->disWidget3D->AddObserver(vtkCommand::InteractionEvent ,pAxialView->mCommand);
				pMeasureObj->disWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pAxialView->mCommand);

				char lable[] = "D_0: %.2lf";
				lable[2] = '0' + pMeasureObj->num;
				pMeasureObj->disRep3D->SetLabelFormat(lable);
				pMeasureObj->disWidget3D->On();

				pMeasureObj->disValue = pMeasureObj->disRep3D->GetDistance();
				pMeasureObj->is2D = false;
				pMeasureObj->isAngle = false;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.ReconstruView->dpTimes = 0;
				ui.ReconstruView->disFlag = false;
				ui.DISTANCE_MEASURE->setChecked(false);
			}
		}
	}
}
void ReconstruMainWindow::OnChangeDisFlag()
{
	if (ui.DISTANCE_MEASURE->isChecked())
	{
		ui.ReconstruView->disFlag = true;
		ui.ReconstruView->angFlag = false;

		ui.AxialView->disFlag = true;
		ui.AxialView->angFlag = false;
		ui.CoronalView->disFlag = true;
		ui.CoronalView->angFlag = false;
		ui.SagittalView->disFlag = true;
		ui.SagittalView->angFlag = false;

		ui.ANGLE_MEASURE->setChecked(false);
	} 
	else
	{
		ui.ReconstruView->disFlag = false;
		ui.ReconstruView->dpTimes = 0;

		ui.AxialView->disFlag = false;
		ui.AxialView->dpTimes = 0;
		ui.CoronalView->disFlag = false;
		ui.CoronalView->dpTimes = 0;
		ui.SagittalView->disFlag = false;
		ui.SagittalView->dpTimes = 0;
	}
}
void ReconstruMainWindow::OnChangeAngFlag()
{
	if (ui.ANGLE_MEASURE->isChecked())
	{
		ui.ReconstruView->angFlag = true;
		ui.ReconstruView->disFlag = false;

		ui.AxialView->angFlag = true;
		ui.AxialView->disFlag = false;
		ui.CoronalView->angFlag = true;
		ui.CoronalView->disFlag = false;
		ui.SagittalView->angFlag = true;
		ui.SagittalView->disFlag = false;

		ui.DISTANCE_MEASURE->setChecked(false);
	} 
	else
	{
		ui.ReconstruView->angFlag = false;
		ui.ReconstruView->dpTimes = 0;

		ui.AxialView->angFlag = false;
		ui.AxialView->dpTimes = 0;
		ui.CoronalView->angFlag = false;
		ui.CoronalView->dpTimes = 0;
		ui.SagittalView->angFlag = false;
		ui.SagittalView->dpTimes = 0;
	}
}
void ReconstruMainWindow::On2DAngleMeasure()
{
	if (ui.FILE_OPEN->isEnabled())
		return;

	if(ui.ANGLE_MEASURE->isChecked())
	{
		if (ui.AxialView->angFlag)
		{
			if (ui.AxialView->dpTimes == 1)
			{
				double *pt = pAxialView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.AxialView->dpTimes == 2)
			{
				double *pt = pAxialView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;
			}
			if (ui.AxialView->dpTimes == 3)
			{
				double *pt = pAxialView->picker->GetPickPosition();
				p3[0] = pt[0];	p3[1] = pt[1];	p3[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];
				pMeasureObj->p3[0] = p3[0];	pMeasureObj->p3[1] = p3[1];	pMeasureObj->p3[2] = p3[2];

				pMeasureObj->angWidget3D->SetInteractor(pAxialView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->angWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->angRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->angRep3D->SetCenterWorldPosition(pMeasureObj->p2);
				pMeasureObj->angRep3D->SetPoint2WorldPosition(pMeasureObj->p3);

				pMeasureObj->angWidget3D->AddObserver(vtkCommand::InteractionEvent ,pAxialView->mCommand);
				pMeasureObj->angWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pAxialView->mCommand);

				char lable[] = "A_0: %.2lf";
				lable[2] = '0' + pMeasureObj->num;
				pMeasureObj->angRep3D->SetLabelFormat(lable);
				pMeasureObj->angWidget3D->On();

				pMeasureObj->angValue = (pMeasureObj->angRep3D->GetAngle())*180/vtkMath::DoublePi();
				pMeasureObj->is2D = true;
				pMeasureObj->isAxi = true;
				pMeasureObj->isAngle = true;
				pMeasureObj->nSlice = pAxialView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.AxialView->dpTimes = 0;
				ui.AxialView->angFlag = false;
				ui.ANGLE_MEASURE->setChecked(false);
			}
		}
		if (ui.CoronalView->angFlag)
		{
			if (ui.CoronalView->dpTimes == 1)
			{
				double *pt = pCoronalView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.CoronalView->dpTimes == 2)
			{
				double *pt = pCoronalView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;
			}
			if (ui.CoronalView->dpTimes == 3)
			{
				double *pt = pCoronalView->picker->GetPickPosition();
				p3[0] = pt[0];	p3[1] = pt[1];	p3[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];
				pMeasureObj->p3[0] = p3[0];	pMeasureObj->p3[1] = p3[1];	pMeasureObj->p3[2] = p3[2];

				pMeasureObj->angWidget3D->SetInteractor(pCoronalView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->angWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->angRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->angRep3D->SetCenterWorldPosition(pMeasureObj->p2);
				pMeasureObj->angRep3D->SetPoint2WorldPosition(pMeasureObj->p3);

				pMeasureObj->angWidget3D->AddObserver(vtkCommand::InteractionEvent ,pCoronalView->mCommand);
				pMeasureObj->angWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pCoronalView->mCommand);

				char lable[] = "A_0: %.2lf";
				lable[2] = '0' + pMeasureObj->num;
				pMeasureObj->angRep3D->SetLabelFormat(lable);
				pMeasureObj->angWidget3D->On();

				pMeasureObj->angValue = (pMeasureObj->angRep3D->GetAngle())*180/vtkMath::DoublePi();
				pMeasureObj->is2D = true;
				pMeasureObj->isCor = true;
				pMeasureObj->isAngle = true;
				pMeasureObj->nSlice = pCoronalView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.CoronalView->dpTimes = 0;
				ui.CoronalView->angFlag = false;
				ui.ANGLE_MEASURE->setChecked(false);
			}
		}
		if (ui.SagittalView->angFlag)
		{
			if (ui.SagittalView->dpTimes == 1)
			{
				double *pt = pSagittalView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2]+0.025;
			}
			if (ui.SagittalView->dpTimes == 2)
			{
				double *pt = pSagittalView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2]+0.025;
			}
			if (ui.SagittalView->dpTimes == 3)
			{
				double *pt = pSagittalView->picker->GetPickPosition();
				p3[0] = pt[0];	p3[1] = pt[1];	p3[2] = pt[2]+0.025;

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];
				pMeasureObj->p3[0] = p3[0];	pMeasureObj->p3[1] = p3[1];	pMeasureObj->p3[2] = p3[2];

				pMeasureObj->angWidget3D->SetInteractor(pSagittalView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->angWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->angRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->angRep3D->SetCenterWorldPosition(pMeasureObj->p2);
				pMeasureObj->angRep3D->SetPoint2WorldPosition(pMeasureObj->p3);

				pMeasureObj->angWidget3D->AddObserver(vtkCommand::InteractionEvent ,pSagittalView->mCommand);
				pMeasureObj->angWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pSagittalView->mCommand);

				char lable[] = "A_0: %.2lf";
				lable[2] = '0' + pMeasureObj->num;
				pMeasureObj->angRep3D->SetLabelFormat(lable);
				pMeasureObj->angWidget3D->On();

				pMeasureObj->angValue = (pMeasureObj->angRep3D->GetAngle())*180/vtkMath::DoublePi();
				pMeasureObj->is2D = true;
				pMeasureObj->isSag = true;
				pMeasureObj->isAngle = true;
				pMeasureObj->nSlice = pSagittalView->m_nSlice;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.SagittalView->dpTimes = 0;
				ui.SagittalView->angFlag = false;
				ui.ANGLE_MEASURE->setChecked(false);
			}
		}
	}
}

void ReconstruMainWindow::OnRecAngleMeasure()
{
	if(ui.ANGLE_MEASURE->isChecked())
	{
		if (ui.ReconstruView->angFlag)
		{
			if (ui.ReconstruView->dpTimes == 1)
			{
				double *pt = pReconstruView->picker->GetPickPosition();
				p1[0] = pt[0];	p1[1] = pt[1];	p1[2] = pt[2];
			}
			if (ui.ReconstruView->dpTimes == 2)
			{
				double *pt = pReconstruView->picker->GetPickPosition();
				p2[0] = pt[0];	p2[1] = pt[1];	p2[2] = pt[2];
			}
			if (ui.ReconstruView->dpTimes == 3)
			{
				double *pt = pReconstruView->picker->GetPickPosition();
				p3[0] = pt[0];	p3[1] = pt[1];	p3[2] = pt[2];

				MeasureObject *pMeasureObj = new MeasureObject();

				pMeasureObj->p1[0] = p1[0];	pMeasureObj->p1[1] = p1[1];	pMeasureObj->p1[2] = p1[2];
				pMeasureObj->p2[0] = p2[0];	pMeasureObj->p2[1] = p2[1];	pMeasureObj->p2[2] = p2[2];
				pMeasureObj->p3[0] = p3[0];	pMeasureObj->p3[1] = p3[1];	pMeasureObj->p3[2] = p3[2];

				pMeasureObj->angWidget3D->SetInteractor(pReconstruView->iren);
				pMeasureObj->num = meaCount+1;

				pMeasureObj->angWidget3D->SetWidgetStateToManipulate();
				pMeasureObj->angRep3D->SetPoint1WorldPosition(pMeasureObj->p1);
				pMeasureObj->angRep3D->SetCenterWorldPosition(pMeasureObj->p2);
				pMeasureObj->angRep3D->SetPoint2WorldPosition(pMeasureObj->p3);

				pMeasureObj->angWidget3D->AddObserver(vtkCommand::InteractionEvent ,pAxialView->mCommand);
				pMeasureObj->angWidget3D->AddObserver(vtkCommand::EndInteractionEvent ,pAxialView->mCommand);

				char lable[] = "A_0:%.2lf";
				lable[2] = '0' + pMeasureObj->num;
				pMeasureObj->angRep3D->SetLabelFormat(lable);

				pMeasureObj->angWidget3D->On();

				pMeasureObj->angValue = (pMeasureObj->angRep3D->GetAngle())*180/vtkMath::DoublePi();
				pMeasureObj->is2D = false;
				pMeasureObj->isAngle = true;
				meaList.append(pMeasureObj);
				addMeasure();

				ui.ReconstruView->dpTimes = 0;
				ui.ReconstruView->angFlag = false;
				ui.ANGLE_MEASURE->setChecked(false);
			}
		}
	}
}
void ReconstruMainWindow::addMeasure()
{
	int nCount = meaList.count();
	MeasureObject *pMeasureObj = meaList.last();

	//visibility
	QWidget *checkBoxWidget = new QWidget();
	QHBoxLayout *checkBoxLayout = new QHBoxLayout();
	checkBoxLayout->addWidget(pMeasureObj->checkBox);  
	checkBoxLayout->setMargin(0);
	checkBoxLayout->setAlignment(pMeasureObj->checkBox, Qt::AlignCenter);
	checkBoxWidget->setLayout(checkBoxLayout);
	pMeasureObj->checkBox->setChecked(true);

	ui.measureTable->insertRow(nCount-1);
	ui.measureTable->setCellWidget(nCount-1,0,checkBoxWidget);

	connect(pMeasureObj->checkBox,SIGNAL(clicked()),this,SLOT(OnUpdateMeasureVisib()));

	//value
	QTableWidgetItem *valueItem = new QTableWidgetItem();
	valueItem->setTextAlignment(Qt::AlignCenter);

	ui.measureTable->setItem(nCount-1,1,valueItem);
	OnUpdateValue();

	ui.measureTable->setCurrentCell(nCount-1,0);

	++meaCount;
}
void ReconstruMainWindow::OnUpdateMeasureVisib()
{
	int nCount = meaList.count();
	for(int i = 0; i < nCount; ++i)
	{
		if (meaList.at(i)->checkBox->isChecked())
		{
			if (!meaList.at(i)->is2D)
			{
				if (!meaList.at(i)->isAngle)
				{
					meaList.at(i)->disWidget3D->On();
				} 
				else
				{
					meaList.at(i)->angWidget3D->On();
				}
			} 
			else
			{
				if (!meaList.at(i)->isAngle)
				{
					meaList.at(i)->disWidget3D->On();
				} 
				else
				{
					meaList.at(i)->angWidget3D->On();
				}
			}
		}
		else
		{
			if (!meaList.at(i)->is2D)
			{
				if (!meaList.at(i)->isAngle)
				{
					meaList.at(i)->disWidget3D->Off();
				} 
				else
				{
					meaList.at(i)->angWidget3D->Off();
				}
			} 
			else
			{
				if (!meaList.at(i)->isAngle)
				{
					meaList.at(i)->disWidget3D->Off();
				} 
				else
				{
					meaList.at(i)->angWidget3D->Off();
				}
			}
		}
	}
}

void ReconstruMainWindow::OnUpdateValue()
{
	int nCount = meaList.count();
	if (nCount > 0)
	{
		for(int i = 0; i < nCount; ++i)
		{
			QString valueText;
			if (!meaList.at(i)->isAngle)
			{
				if (!meaList.at(i)->is2D)
				{
					meaList.at(i)->disValue = meaList.at(i)->disRep3D->GetDistance();
				} 
				else
				{
					if (meaList.at(i)->isAxi)
					{
						double fScale = pAxialView->fScale;
						double *r1;	double *r2;
						r1 = meaList.at(i)->disRep3D->GetPoint1WorldPosition();
						double xp1 = (r1[0]+0.5)*pAxialView->m_nWidth*pAxialView->m_fPixelSpacing[0];
						double yp1 = (r1[1]+0.5*fScale)*pAxialView->m_nHeight/fScale*pAxialView->m_fPixelSpacing[1];
						r2 = meaList.at(i)->disRep3D->GetPoint2WorldPosition();
						double xp2 = (r2[0]+0.5)*pAxialView->m_nWidth*pAxialView->m_fPixelSpacing[0];
						double yp2 = (r2[1]+0.5*fScale)*pAxialView->m_nHeight/fScale*pAxialView->m_fPixelSpacing[1];
						meaList.at(i)->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));

						QString str;	str.sprintf("D_%d: %.2lf", meaList.at(i)->num, meaList.at(i)->disValue);
						QByteArray ba = str.toLatin1();
						char *lable = ba.data();
						meaList.at(i)->disRep3D->SetLabelFormat(lable);
					}
					if (meaList.at(i)->isCor)
					{
						double fScale = pCoronalView->fScale;
						double *r1;	double *r2;
						r1 = meaList.at(i)->disRep3D->GetPoint1WorldPosition();
						double xp1 = (r1[0]+0.5)*pCoronalView->m_nWidth*pCoronalView->m_fPixelSpacing[0];
						double yp1 = (r1[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale*pCoronalView->m_fPixelSpacing[2];
						r2 = meaList.at(i)->disRep3D->GetPoint2WorldPosition();
						double xp2 = (r2[0]+0.5)*pCoronalView->m_nWidth*pCoronalView->m_fPixelSpacing[0];
						double yp2 = (r2[1]+0.5*fScale)*pCoronalView->m_nSliceSum/fScale*pCoronalView->m_fPixelSpacing[2];
						meaList.at(i)->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));

						QString str;	str.sprintf("D_%d: %.2lf", meaList.at(i)->num, meaList.at(i)->disValue);
						QByteArray ba = str.toLatin1();
						char *lable = ba.data();
						meaList.at(i)->disRep3D->SetLabelFormat(lable);
					}
					if (meaList.at(i)->isSag)
					{
						double fScale = pSagittalView->fScale;
						double *r1;	double *r2;
						r1 = meaList.at(i)->disRep3D->GetPoint1WorldPosition();
						double xp1 = (r1[0]+0.5)*pSagittalView->m_nHeight*pSagittalView->m_fPixelSpacing[1];
						double yp1 = (r1[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale*pSagittalView->m_fPixelSpacing[2];
						r2 = meaList.at(i)->disRep3D->GetPoint2WorldPosition();
						double xp2 = (r2[0]+0.5)*pSagittalView->m_nHeight*pSagittalView->m_fPixelSpacing[1];
						double yp2 = (r2[1]+0.5*fScale)*pSagittalView->m_nSliceSum/fScale*pSagittalView->m_fPixelSpacing[2];
						meaList.at(i)->disValue = sqrt((xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2));

						QString str;	str.sprintf("D_%d: %.2lf", meaList.at(i)->num, meaList.at(i)->disValue);
						QByteArray ba = str.toLatin1();
						char *lable = ba.data();
						meaList.at(i)->disRep3D->SetLabelFormat(lable);
					}
				}
				//char *chr = meaList.at(i)->disRep3D->GetLabelFormat();
				valueText.sprintf("D_%d: %.2lf mm", meaList.at(i)->num, meaList.at(i)->disValue);
				ui.measureTable->item(i,1)->setText(valueText);
			} 
			else
			{
				meaList.at(i)->angValue = meaList.at(i)->angRep3D->GetAngle()*180/vtkMath::DoublePi();
				valueText.sprintf("A_%d: %.2lf", meaList.at(i)->num, meaList.at(i)->angValue).append(QString::fromLocal8Bit(" °"));
				ui.measureTable->item(i,1)->setText(valueText);
			}
		}
	}
}
void ReconstruMainWindow::OnUpdate2DMeasure()
{
	int nCount = meaList.count();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; ++i)
		{
			MeasureObject *pMeasureObj = meaList.at(i);
			if (pMeasureObj->is2D && pMeasureObj->checkBox->isChecked())
			{
				int slice;
				if (pMeasureObj->isAxi)slice = pAxialView->m_nSlice;
				if (pMeasureObj->isCor)slice = pCoronalView->m_nSlice;
				if (pMeasureObj->isSag)slice = pSagittalView->m_nSlice;

				if (pMeasureObj->nSlice == slice)
				{
					if (pMeasureObj->isAngle)
					{
						pMeasureObj->angWidget3D->On();
					} 
					else
					{
						pMeasureObj->disWidget3D->On();
					}
				}
				else
				{
					if (pMeasureObj->isAngle)
					{
						pMeasureObj->angWidget3D->Off();
					} 
					else
					{
						pMeasureObj->disWidget3D->Off();
					}
				}
			}
		}
	}
}
void ReconstruMainWindow::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key()==Qt::Key_Shift)
	{
		emit shift();
	}
}

void ReconstruMainWindow::OnSelectPts()
{
	if (ui.SELECT_PTS->isChecked())
	{
		pReconstruView->iren->SetInteractorStyle(pReconstruView->StyleTrackballCamera);
		ui.VIEW_MODE->setEnabled(false);
	}
	else
	{
		disconnect(iniTemplate->SaveCurveCommand->qSaveCurve, SIGNAL(saveCurrentCurve()),this, SLOT(SaveCurve()));

		delete iniTemplate;
		iniTemplate = new InitialTemplate(pReconstruView->renWin,
			pReconstruView->ren,
			pReconstruView->iren,
			pReconstruView->StyleTrackballCamera,
			pReconstruView->picker);
		pReconstruView->StyleTrackballCamera->AddObserver(vtkCommand::InteractionEvent,iniTemplate->SaveCurveCommand);
		connect(iniTemplate->SaveCurveCommand->qSaveCurve, SIGNAL(saveCurrentCurve()),this, SLOT(SaveCurve()));

		pReconstruView->renWin->Render();
		ui.VIEW_MODE->setEnabled(true);
	}
}

void ReconstruMainWindow::OnContourWidget()
{
	if (ui.SELECT_PTS->isChecked())
	{
		if (pAxialView->m_Actorlist.count()>0)
		{
			iniTemplate->FixPoint(pReconstruView->picker->GetPickPosition());
			//contourWidget = vtkSmartPointer<vtkContourWidget>::New();
			//contourWidget->SetInteractor(pReconstruView->iren);

			//contourWidgetRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(contourWidget->GetRepresentation());

			//vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer = vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
			//pointPlacer->AddProp(pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoActor);
			//pointPlacer->GetPolys()->AddItem(pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoMapper->GetInput());

			//contourWidgetRep->GetLinesProperty()->SetColor(1, 0, 0);
			//contourWidgetRep->GetLinesProperty()->SetLineWidth(3.0);
			//contourWidgetRep->SetPointPlacer(pointPlacer);
			//contourWidget->EnabledOn();

			ui.TEM_GENERATE->setEnabled(true);
			ui.CLOSE_CONTOURS->setEnabled(true);
			ui.DELETE_PTS->setEnabled(true);
		}
	} 
	else
	{
		//if (contourWidget)
		//{
		//	contourWidget->EnabledOff();
		//}
		pReconstruView->renWin->Render();
		ui.TEM_GENERATE->setEnabled(false);
		ui.CLOSE_CONTOURS->setEnabled(false);
		ui.DELETE_PTS->setEnabled(false);
	}
}
//生成接骨板
void ReconstruMainWindow::OnTemGenerate()
{
	ui.progressBar->show();

	int row = ui.modelTable->currentRow();
	int ff = iniTemplate->GetInitialTemplate(pAxialView->m_Actorlist.at(row)->isoMapper->GetInput(), iniTemplate->m_AllSubdivisionPts);

	pReconstruView->renWin->Render();

	if(ff)
	{
		QActorObject *pActorObject = new QActorObject();

		pActorObject->isTem = true;
		pActorObject->templateData = new InitialTemplate;

		pActorObject->templateData->DeepCopy(iniTemplate);

		vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
		stripper->SetInput(pActorObject->templateData->m_OutputIni);
		iniTemplate->Progress->text="	(8/8)	Stripping...";
		stripper->AddObserver(vtkCommand::ProgressEvent, iniTemplate->Progress);
		stripper->Update();

		vtkSmartPointer<vtkPolyDataNormals> Normals=vtkSmartPointer<vtkPolyDataNormals>::New();	
		Normals->SetInput(stripper->GetOutput());
		iniTemplate->Progress->text="	(8/8)	Normals generating...";
		Normals->AddObserver(vtkCommand::ProgressEvent, iniTemplate->Progress);
		Normals->Update();

		//iniTemplate->ClearContours();
		disconnect(iniTemplate->SaveCurveCommand->qSaveCurve, SIGNAL(saveCurrentCurve()),this, SLOT(SaveCurve()));
		//QMessageBox::information(0, "","0");
		delete iniTemplate;
		//QMessageBox::information(0, "","1");
		iniTemplate = new InitialTemplate(pReconstruView->renWin,
			pReconstruView->ren,
			pReconstruView->iren,
			pReconstruView->StyleTrackballCamera,
			pReconstruView->picker);
		pReconstruView->StyleTrackballCamera->AddObserver(vtkCommand::InteractionEvent,iniTemplate->SaveCurveCommand);
		connect(iniTemplate->SaveCurveCommand->qSaveCurve, SIGNAL(saveCurrentCurve()),this, SLOT(SaveCurve()));

		ui.VIEW_MODE->setEnabled(true);
		//int num = iniTemplate->normalPts->GetNumberOfPoints();
		//for (int i=0;i<num;++i)
		//{
		//	vtkSmartPointer<vtkSphereSource> sp = vtkSmartPointer<vtkSphereSource>::New();
		//	sp->SetCenter(iniTemplate->normalPts->GetPoint(i));
		//	sp->SetRadius(1);
		//	vtkSmartPointer<vtkPolyDataMapper> mapp = vtkSmartPointer<vtkPolyDataMapper>::New();
		//	mapp->SetInput(sp->GetOutput());
		//	vtkSmartPointer<vtkActor> actorr = vtkSmartPointer<vtkActor>::New();
		//	actorr->SetMapper(mapp);
		//	pReconstruView->ren->AddActor(actorr);
		//}
		

		
		pActorObject->isoMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
		pActorObject->isoMapper->SetInput(Normals->GetOutput());
		pActorObject->isoMapper->ScalarVisibilityOff();
		pActorObject->isoMapper->ImmediateModeRenderingOn();
		pActorObject->isoMapper->Update();

		pActorObject->isoActor=vtkSmartPointer<vtkActor>::New();
		pActorObject->isoActor->SetMapper(pActorObject->isoMapper);

		pAxialView->m_Actorlist.append(pActorObject);
		addModel();
		++pAxialView->actorCount;
		pReconstruView->renWin->Render();

		temPts->Reset();
		ui.TEM_GENERATE->setChecked(false);
		ui.TEM_GENERATE->setEnabled(false);
		ui.SELECT_PTS->setChecked(false);
		OnContourWidget();
		ui.IMPORT_AXIS->setEnabled(true);
	}
	ui.progressBar->hide();
	ui.statusbar->clearMessage();
}
//取打孔点
void ReconstruMainWindow::OnImportAxis()
{
	//OnXMLTube();
//-------------------------------------------------------------------------------------------
	if (ui.IMPORT_AXIS->isChecked())
	{
		if (pAxialView->m_Actorlist.count()>0)
		{
			contourWidget = vtkSmartPointer<vtkContourWidget>::New();
			contourWidget->SetInteractor(pReconstruView->iren);

			contourWidgetRep = vtkOrientedGlyphContourRepresentation::SafeDownCast(contourWidget->GetRepresentation());

			vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer = vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
			pointPlacer->AddProp(pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoActor);
			pointPlacer->GetPolys()->AddItem(pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isoMapper->GetInput());

			contourWidgetRep->GetLinesProperty()->SetColor(1, 0, 0);
			contourWidgetRep->GetLinesProperty()->SetLineWidth(3.0);
			contourWidgetRep->SetPointPlacer(pointPlacer);
			contourWidgetRep->GetLinesProperty()->SetOpacity(0.0);
			contourWidget->EnabledOn();

			ui.PUNCH->setEnabled(true);
		}
	} 
	else
	{
		if (contourWidget)
		{
			contourWidget->EnabledOff();
		}
		ui.PUNCH->setEnabled(false);
		pReconstruView->renWin->Render();
	}
}

void ReconstruMainWindow::OnPunching()
{
	if (pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isTem)
	{
		ui.progressBar->show();
		int row = ui.modelTable->currentRow();

		//输入点
		int nodesCount = contourWidgetRep->GetNumberOfNodes();

		for (int i = 0; i < nodesCount; ++i)
		{
			if (!contourWidgetRep->GetNthNodeWorldPosition(i,node))
			{
				QMessageBox::information(0, tr("Error"),tr("Please reselect points!"));
				temPts->Reset();
				ui.TEM_GENERATE->setChecked(false);
				ui.SELECT_PTS->setChecked(false);
				return;
			}
			//保证任意两点距离大于孔直径
			if (i == 0)
			{
				pAxialView->m_Actorlist.at(row)->templateData->m_AxisXMLPoints->InsertNextPoint(node);
			}
			else
			{
				int j;
				for (j = 0; j < i; ++j)
				{
					double pp[3];
					contourWidgetRep->GetNthNodeWorldPosition(j,pp);
					if ((pp[0]-node[0])*(pp[0]-node[0])+(pp[1]-node[1])*(pp[1]-node[1])+(pp[2]-node[2])*(pp[2]-node[2]) < 4*punchRadius*punchRadius)
					{
						break;
					}
				}
				if (j == i)
				{
					pAxialView->m_Actorlist.at(row)->templateData->m_AxisXMLPoints->InsertNextPoint(node);
				}
			}
		}

		//生成孔
		if (!pAxialView->m_Actorlist.at(row)->templateData->GetFinalPlateWithPunching(punchRadius,pAxialView->m_Actorlist.at(row)->templateData->m_AxisXMLPoints))
		{
			return;
		}
		QActorObject* pActorObject=(QActorObject*) pAxialView->m_Actorlist.at(ui.modelTable->currentRow());
		pActorObject->isoMapper->RemoveAllInputs();
		pActorObject->isoMapper->SetInput(pAxialView->m_Actorlist.at(row)->templateData->m_OutputFin);
		pActorObject->isoMapper->Update();

		ui.IMPORT_AXIS->setChecked(false);
		OnImportAxis();
		ui.PUNCH->setEnabled(false);
		ui.UNDO_PUNCHING->setEnabled(true);
		pAxialView->m_Actorlist.at(row)->templateData->m_AxisXMLPoints->Reset();

		ui.progressBar->hide();
		ui.statusbar->clearMessage();
	}
	else
	{
		QMessageBox::information(0, tr("Error"),tr("Please verify the selected model is a TEMPLATE!"));
		if (contourWidget)
		{
			contourWidget->EnabledOff();
		}
		ui.IMPORT_AXIS->setChecked(false);
		ui.PUNCH->setEnabled(false);
	}
}

void ReconstruMainWindow::OnXMLTube()
{
	if (pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isTem)
	{
		ui.progressBar->show();
		int row = ui.modelTable->currentRow();
		InitialTemplate *pTem = pAxialView->m_Actorlist.at(row)->templateData;

		//输入点
		//vtkSmartPointer<vtkPoints>AxesPts = vtkSmartPointer<vtkPoints>::New();
		XMLFileImport(pTem->m_AxisXMLPoints);

		int NumofAxes = 0.5*pTem->m_AxisXMLPoints->GetNumberOfPoints();
		double p0[3], p1[3];
		double AxisRadius=0.2;

		for(int i=0;i<NumofAxes;i++)
		{
			vtkSmartPointer<vtkPolyData>AxisPD = vtkSmartPointer<vtkPolyData>::New();
			vtkSmartPointer<vtkPolyDataMapper>AxisMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			vtkSmartPointer<vtkActor>AxisActor = vtkSmartPointer<vtkActor>::New();
			pTem->m_AxisXMLPoints->GetPoint(2*i,p0);
			pTem->m_AxisXMLPoints->GetPoint(2*i+1,p1);
			CreateTube(p0,p1,AxisRadius,AxisPD,0);
			AxisMapper->SetInput(AxisPD);
			AxisActor->SetMapper(AxisMapper);
			//AxisActor->GetProperty()->SetColor(0,1,0);
			AxisActor->GetProperty()->SetColor(18./255.,18./255,201./255);
			pReconstruView->ren->AddActor(AxisActor);
		}
		pReconstruView->renWin->Render();

		//生成导向圆柱
		pTem->GetFinalTemplate(pTem->m_AxisXMLPoints);

		QActorObject* pActorObject=(QActorObject*) pAxialView->m_Actorlist.at(ui.modelTable->currentRow());
		pActorObject->isoMapper->RemoveAllInputs();
		pActorObject->isoMapper->SetInput(pTem->m_OutputFin);
		pActorObject->isoMapper->Update();

		ui.IMPORT_AXIS->setChecked(false);
		ui.PUNCH->setEnabled(false);
		ui.UNDO_PUNCHING->setEnabled(true);
		pTem->m_AxisXMLPoints->Reset();

		ui.progressBar->hide();
		ui.statusbar->clearMessage();
	}
	else
	{
		QMessageBox::information(0, tr("Error"),tr("Please verify the selected model is a TEMPLATE!"));
		if (contourWidget)
		{
			contourWidget->EnabledOff();
		}
		ui.IMPORT_AXIS->setChecked(false);
		ui.PUNCH->setEnabled(false);
	}
}

void ReconstruMainWindow::OnUndoPunching()
{
	if (pAxialView->m_Actorlist.at(ui.modelTable->currentRow())->isTem)
	{
		int row = ui.modelTable->currentRow();

		pAxialView->m_Actorlist.at(row)->templateData->UndoPunching();

		QActorObject* pActorObject=(QActorObject*) pAxialView->m_Actorlist.at(ui.modelTable->currentRow());
		pActorObject->isoMapper->RemoveAllInputs();
		pActorObject->isoMapper->SetInput(pAxialView->m_Actorlist.at(row)->templateData->m_OutputIni);
		pActorObject->isoMapper->Update();

		pReconstruView->renWin->Render();
	}
	else
	{
		QMessageBox::information(0, tr("Error"),tr("Please verify that the selected model is a TEMPLATE!"));
		ui.IMPORT_AXIS->setChecked(false);
		ui.PUNCH->setEnabled(false);
	}
}

void ReconstruMainWindow::XMLFileImport(vtkPoints* XMLPoints) 
{
	QString sXML = QFileDialog::getOpenFileName(0,
                                   tr("Open File"), QDir::currentPath(),"*.xml");
       
	if(!sXML.isEmpty())
	{		
		QFile file(sXML);  
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
			return;         //不能打开文件

		QDomDocument dom;   
		if(!dom.setContent(&file))   
		{   
			file.close();         //加载XML文件出错  
			return;   
		}   

		QDomElement           root   =   dom.documentElement();   
		QDomNodeList         nodelist=root.childNodes();   
		int   count=nodelist.length();
		XMLPoints->SetNumberOfPoints(count);

		double *fTipPoint=new double[3] ;
		for(int i=0; i<count;i++)
		{
			for(int j=0; j<3;j++)
			{
				QString tem;
				tem = nodelist.item(i).childNodes().item(j).firstChild().nodeValue();		
				fTipPoint[j]=tem.toDouble();			
			}
			XMLPoints->InsertPoint(i,fTipPoint);
		}
		delete[] fTipPoint;
		file.close();
	}
}
//画圆柱
void ReconstruMainWindow::CreateTube(double x0[3],double x1[3], double Radius,vtkPolyData*OutputPD, bool IsCappingOn)
{
		vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
		lineSource->SetPoint1(x0);
		lineSource->SetPoint2(x1);

		vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter->SetInputConnection(lineSource->GetOutputPort());
		tubeFilter->SetRadius(Radius);
		tubeFilter->SetNumberOfSides(100);
		if(IsCappingOn)
		{
			tubeFilter->CappingOn();     
		}
		tubeFilter->Update();

		vtkSmartPointer<vtkTriangleFilter> TriangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
		TriangleFilter->SetInput(tubeFilter->GetOutput());
		TriangleFilter->Update();

		vtkSmartPointer<vtkLinearSubdivisionFilter>SubdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
		SubdivisionFilter->SetNumberOfSubdivisions(3);
		SubdivisionFilter->SetInput(TriangleFilter->GetOutput());
		SubdivisionFilter->Update();

		OutputPD->DeepCopy(SubdivisionFilter->GetOutput());
}

ReconstruMainWindow::~ReconstruMainWindow(void)
{
	pAxialView->SliceNumMapper->Delete();pAxialView->SliceZvalueMapper->Delete();
	pAxialView->SliceNumActor->Delete();pAxialView->SliceZvalueActor->Delete();	

	pReconstruView->planeWidget->Delete();

	if(!m_bTransferSucceed)
	{
		if(pAxialView->m_bCropImage)
			pAxialView->VTKResampleImporter->Delete();
		else
			pAxialView->VTKImporter->Delete();
	}

	delete iniTemplate;

	delete pAxialView;	
	delete pReconstruView;
}

void ReconstruMainWindow::recut(vtkPolyData* mesh)
{
	vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
	decimator->SetInput(mesh);
	decimator->SetTargetReduction(0.9);
	decimator->SplittingOff();
	decimator->PreserveTopologyOff();
	Progress->text="	Decimating...";
	decimator->AddObserver(vtkCommand::ProgressEvent, Progress);
	decimator->Update();

	vtkSmartPointer<vtkMatrix4x4> resize_matrix4 = vtkSmartPointer<vtkMatrix4x4>::New();
	//放大倍数
	double resize_value = 2;
	for (int i=0;i<4;++i)
	{
		for (int j=0;j<4;++j)
		{
			if (i==j)
			{
				resize_matrix4->SetElement(i,j,resize_value);
			}
			else
			{
				resize_matrix4->SetElement(i,j,0);
			}
		}
	}
	resize_matrix4->SetElement(3,3,1);
	vtkSmartPointer<vtkMatrix4x4> rotate_matrix4 = vtkSmartPointer<vtkMatrix4x4>::New();
	for (int i=0;i<4;++i)
	{
		for (int j=0;j<4;++j)
		{
			rotate_matrix4->SetElement(i,j,0);
		}
	}
	rotate_matrix4->SetElement(0,0,1);
	rotate_matrix4->SetElement(1,2,1);
	rotate_matrix4->SetElement(2,1,-1);
	rotate_matrix4->SetElement(3,3,1);
	vtkSmartPointer<vtkMatrix4x4> final_matrix4 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(resize_matrix4,rotate_matrix4,final_matrix4);

	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->SetMatrix(final_matrix4);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformPolyData->SetInput(decimator->GetOutput());
	transformPolyData->SetTransform(transform);
	transformPolyData->Update();

	//double cyMatrix[16] = {	1, 0, 0, 0,
	//						0, 0, 1, 0,
	//						0,-1, 0, 0,
	//						0, 0, 0, 1	};

	vtkSmartPointer<vtkCylinder> cylinder = vtkSmartPointer<vtkCylinder>::New();
	cylinder->SetCenter(transformPolyData->GetOutput()->GetCenter());
	cylinder->SetRadius(5);

	//clipper
	vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
	clipper->SetInput(transformPolyData->GetOutput());
	clipper->SetClipFunction(cylinder);
	clipper->GenerateClipScalarsOn();
	clipper->InsideOutOn();
	clipper->Update();

	double* origin = clipper->GetOutput()->GetCenter();
	vtkSmartPointer<vtkPlane> planeUp = vtkSmartPointer<vtkPlane>::New();
	planeUp->SetOrigin(origin[0],origin[1]+3.5,origin[2]);
	planeUp->SetNormal(0,-1,0);

	vtkSmartPointer<vtkClipPolyData> clipper_up = vtkSmartPointer<vtkClipPolyData>::New();
	clipper_up->SetInput(clipper->GetOutput());
	clipper_up->SetClipFunction(planeUp);
	clipper_up->GenerateClipScalarsOn();
	clipper_up->Update();

	vtkSmartPointer<vtkPlane> planeDown = vtkSmartPointer<vtkPlane>::New();
	planeDown->SetOrigin(origin[0],origin[1]-3.5,origin[2]);
	planeDown->SetNormal(0,1,0);

	vtkSmartPointer<vtkClipPolyData> clipper_down = vtkSmartPointer<vtkClipPolyData>::New();
	clipper_down->SetInput(clipper_up->GetOutput());
	clipper_down->SetClipFunction(planeDown);
	clipper_down->GenerateClipScalarsOn();
	clipper_down->Update();

	vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
	cleaner->SetInput(clipper_down->GetOutput());
	cleaner->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(cleaner->GetOutputPort());
	connectivityFilter->SetExtractionModeToLargestRegion();

	vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
	cylinderSource->CappingOff();
	cylinderSource->SetCenter(origin);
	cylinderSource->SetHeight(7);
	cylinderSource->SetRadius(5.01);
	cylinderSource->SetResolution(100);

	vtkSmartPointer<vtkTriangleFilter> tri = vtkSmartPointer<vtkTriangleFilter>::New();
	tri->SetInput(cylinderSource->GetOutput());

	vtkSmartPointer<vtkAppendPolyData> appendCylinder = vtkSmartPointer<vtkAppendPolyData>::New();
	appendCylinder->AddInput(tri->GetOutput());
	appendCylinder->AddInput(connectivityFilter->GetOutput());
	appendCylinder->Update();

	rotate_matrix4->Invert();
	vtkSmartPointer<vtkTransform> tf = vtkSmartPointer<vtkTransform>::New();
	tf->SetMatrix(rotate_matrix4);
	vtkSmartPointer<vtkTransformPolyDataFilter> tpd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	tpd->SetInput(appendCylinder->GetOutput());
	tpd->SetTransform(tf);
	tpd->Update();

	mesh->DeepCopy(tpd->GetOutput());
}
void ReconstruMainWindow::SaveCurve()
{
	iniTemplate->SaveCurrentCurve();
}
void ReconstruMainWindow::OnEndSelect()
{
	iniTemplate->CloseContour();

	ui.CLOSE_CONTOURS->setEnabled(false);
	//ui.DELETE_PTS->setEnabled(false);
}

void ReconstruMainWindow::OnDeleteSelect()
{
	ui.CLOSE_CONTOURS->setEnabled(true);
	ui.DELETE_PTS->setEnabled(true);

	iniTemplate->OnUndoPoint();
}