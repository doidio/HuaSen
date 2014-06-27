#include <InitialTemplate.h>

#include "vtkSTLWriter.h"
#include "SelectPolyData.h"
#include <vtkSelectPolyData.h>
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

#include <QtGui>
#include <QtXML>

InitialTemplate::InitialTemplate()
{
	m_Output = vtkSmartPointer<vtkPolyData>::New();
	m_OutputIni = vtkSmartPointer<vtkPolyData>::New();
	m_OutputFin = vtkSmartPointer<vtkPolyData>::New();
	m_Mesh = vtkSmartPointer<vtkPolyData>::New();
	m_Loop = vtkSmartPointer<vtkPoints>::New();
	m_AxisXMLPoints = vtkSmartPointer<vtkPoints>::New();
	m_outSurface = vtkSmartPointer<vtkPolyData>::New();
	m_innSurface = vtkSmartPointer<vtkPolyData>::New();
	m_wall = vtkSmartPointer<vtkPolyData>::New();
	m_innSurf = vtkSmartPointer<vtkPolyData>::New();
	m_outSurf = vtkSmartPointer<vtkPolyData>::New();
	m_mesh = vtkSmartPointer<vtkPolyData>::New();

	TurnOver = false;
	IsFirstPt = true;
	Multi_Angle = false;
	IsContourClosed = false;

	AllControlPts = vtkSmartPointer<vtkPoints>::New();
	m_AllSubdivisionPts = vtkSmartPointer<vtkPoints>::New();
	ControlPts = vtkSmartPointer<vtkPoints>::New();
	SplineFilterPoints = vtkSmartPointer<vtkPoints>::New();	
	ConnectionControlPts = vtkSmartPointer<vtkPoints>::New();
	ConnectionSplineFilterPoints = vtkSmartPointer<vtkPoints>::New();
	ControlPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	SubdivisionPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	ConnectionSubdivisionPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	pContourObject = new QContourObject();
	pConnectionContourObject = new QContourObject();
	SaveCurveCommand = vtkSmartPointer<vtkSaveCurveCommand>::New();

	m_rep = vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	QContourObjectList = new QList<QContourObject *>;

	thickness = 2.;
	tubeRadius = 0.15;

	//normalPts = vtkSmartPointer<vtkPoints>::New();

	Progress = vtkSmartPointer<vtkProgress>::New();
	Progress->iStage = 0;
	Progress->nStage = -1;

	FirstUserPtActor = vtkActor::New();
	IsFirstUserPt = 0;

	m_Output->GlobalWarningDisplayOff();
}
InitialTemplate::InitialTemplate(vtkRenderWindow* rRenWin,
				vtkRenderer* rRen,
				vtkRenderWindowInteractor* rIren,
				vtkInteractorStyleTrackballCamera* rStyleTrackballCamera,
				vtkWorldPointPicker* rPicker)
{
	m_Output = vtkSmartPointer<vtkPolyData>::New();
	m_OutputIni = vtkSmartPointer<vtkPolyData>::New();
	m_OutputFin = vtkSmartPointer<vtkPolyData>::New();
	m_Mesh = vtkSmartPointer<vtkPolyData>::New();
	m_Loop = vtkSmartPointer<vtkPoints>::New();
	m_AxisXMLPoints = vtkSmartPointer<vtkPoints>::New();
	m_outSurface = vtkSmartPointer<vtkPolyData>::New();
	m_innSurface = vtkSmartPointer<vtkPolyData>::New();
	m_wall = vtkSmartPointer<vtkPolyData>::New();
	m_innSurf = vtkSmartPointer<vtkPolyData>::New();
	m_outSurf = vtkSmartPointer<vtkPolyData>::New();
	m_mesh = vtkSmartPointer<vtkPolyData>::New();

	TurnOver = false;
	IsFirstPt = true;
	Multi_Angle = false;
	IsContourClosed = false;

	AllControlPts = vtkSmartPointer<vtkPoints>::New();
	m_AllSubdivisionPts = vtkSmartPointer<vtkPoints>::New();
	ControlPts = vtkSmartPointer<vtkPoints>::New();
	SplineFilterPoints = vtkSmartPointer<vtkPoints>::New();	
	ConnectionControlPts = vtkSmartPointer<vtkPoints>::New();
	ConnectionSplineFilterPoints = vtkSmartPointer<vtkPoints>::New();
	ControlPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	SubdivisionPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	ConnectionSubdivisionPtsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	pContourObject = new QContourObject();
	pConnectionContourObject = new QContourObject();
	SaveCurveCommand = vtkSmartPointer<vtkSaveCurveCommand>::New();

	m_rep = vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	QContourObjectList = new QList<QContourObject *>;

	thickness = 2.;
	tubeRadius = 0.15;

	//normalPts = vtkSmartPointer<vtkPoints>::New();

	Progress = vtkSmartPointer<vtkProgress>::New();
	Progress->iStage = 0;
	Progress->nStage = -1;

	FirstUserPtActor = vtkSmartPointer<vtkActor>::New();
	IsFirstUserPt = 0;

	m_Output->GlobalWarningDisplayOff();

	this->renWin = rRenWin;
	this->ren = rRen;
	this->iren = rIren;
	this->StyleTrackballCamera = rStyleTrackballCamera;
	this->picker = rPicker;
}
//----------------------------------------------------------------------------
InitialTemplate::~InitialTemplate()
{
	int size = QContourObjectList->size();
	for(int i=0; i<size; i++)
	{
		QContourObject* pConObj=(QContourObject*) QContourObjectList->at(i);
		ren->RemoveActor(pConObj->contourActor);

		delete pConObj;
	}
	renWin->Render();

	ren->RemoveActor(pContourObject->contourActor);
	delete pContourObject;
	ren->RemoveActor(pConnectionContourObject->contourActor);
	delete pConnectionContourObject;
}

//----------------------------------------------------------------------------
void InitialTemplate::Initialize()
{
	/*iren->SetRenderWindow(renWin);
	iren->SetPicker(picker);
	iren->Initialize();
	iren->SetInteractorStyle(StyleTrackballCamera);*/
	//StyleTrackballCamera->AddObserver(vtkCommand::InteractionEvent,SaveCurveCommand);
	//ControlPts = vtkSmartPointer<vtkPoints>::New();
}
int InitialTemplate::GetInitialTemplate(vtkPolyData *inputMesh, vtkPoints *Pts)
{
	if (!IsContourClosed)
	{
		QMessageBox::information(0,QObject::tr("Error"),QObject::tr("Please check the contour and click CLOSE CONTOUR button before template generation!"));
		return 0;
	}
	this->m_Mesh = inputMesh;
	this->m_Loop = Pts;

	vtkIdType numPts, numLoopPts;
	int i, j, m, k;

	if ( (numPts = this->m_Mesh->GetNumberOfPoints()) < 1 )
    {
		QMessageBox::information(0,QObject::tr("Error"),QObject::tr("Input contains no points!"));
		return 0;
    }

	if ( this->m_Loop == NULL || 
	(numLoopPts=this->m_Loop->GetNumberOfPoints()) < 3 )
	{
		QMessageBox::information(0,QObject::tr("Error"),
			QObject::tr("Please define a loop with at least three points!"));
		return 0;
	}

	vtkSmartPointer<vtkTriangleFilter> tf = 
		vtkSmartPointer<vtkTriangleFilter>::New();
	tf->SetInput(this->m_Mesh);
	Progress->text="	(1/8)	Triangulating...";
	tf->AddObserver(vtkCommand::ProgressEvent, Progress);
	tf->Update();
	this->m_Mesh->DeepCopy(tf->GetOutput());


//1)Clip inputMesh to get inner surface	
	vtkSmartPointer<vtkPolyData> uselessPD = vtkSmartPointer<vtkPolyData>::New();
	if (!Clip(this->m_Mesh,this->m_Loop,m_innSurface,uselessPD,1))
	{
		QMessageBox::information(0,QObject::tr("Error"),QObject::tr("Clip inputMesh Failed!"));
	     return 0;
	}


	//vtkSmartPointer<vtkSTLWriter> stlWriter_innSurface = 
	//	vtkSmartPointer<vtkSTLWriter>::New();
	//stlWriter_innSurface->SetFileName("testinnSurface.stl");
	//stlWriter_innSurface->SetInput(m_innSurface);
	//stlWriter_innSurface->Write();
	//QMessageBox::information(0,"","Clip inputMesh Finished!");
	m_innSurf->DeepCopy(m_innSurface);

 //2) Entire OuterModel Generation
	vtkSmartPointer<vtkPolyData> outModel = vtkSmartPointer<vtkPolyData>::New();
	if(!OuterModelGeneration(m_innSurface,outModel))
	{
		QMessageBox::information(0,"Error","OuterModel Generation Failed!");
		return 0;
	}
	//vtkSmartPointer<vtkSTLWriter> stlWriter_outModel = vtkSmartPointer<vtkSTLWriter>::New();
	//stlWriter_outModel->SetFileName("testoutModel.stl");
	//stlWriter_outModel->SetInput(outModel);
	//stlWriter_outModel->Write();
	//QMessageBox::information(0,"","OuterModel Generation Finished!");

 //3)Clip OuterModel to get outer surface
	//Firstly, Get edge points of m_innSurface & calculate point normals
	vtkSmartPointer<vtkPoints> EdgePts_inn = vtkSmartPointer<vtkPoints>::New();
	if(!GetEdgePoints(m_innSurface, EdgePts_inn))
	{
		QMessageBox::information(0,"Error","Get edge points failed!");
		return 0;
	}
	//QMessageBox::information(0,"","Get edge points of m_innSurface Finished!");

	vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
	normalGenerator->SetInput(m_innSurface);
	normalGenerator->ComputePointNormalsOn();
	normalGenerator->ComputeCellNormalsOff();
	normalGenerator->SplittingOff();
	normalGenerator->FlipNormalsOff();
	Progress->text="	(1/8)	Inner normals generating...";
	normalGenerator->AddObserver(vtkCommand::ProgressEvent, Progress);
	normalGenerator->Update();

	vtkSmartPointer<vtkDataArray> dataArray = normalGenerator->GetOutput()->GetPointData()->GetNormals();
	if(!dataArray)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}

	//Then, calculate average normals of points
	int samplestep = 10;
	int num = EdgePts_inn->GetNumberOfPoints();
	int Num = num/samplestep;
	if(Num<=20)
	{
		Num = 20;
		samplestep = num/Num;
	}
	double PreEdgePt[3], EdgePt[3], EdgePt_Inn[3];
	double dis=VTK_DOUBLE_MAX;
	int tolerence = 0.2;                               //if distance <0.2, the next point will be deleted      
	int NumofInn=m_innSurface->GetNumberOfPoints();

	vtkSmartPointer<vtkPoints> PtsNormal = 	vtkSmartPointer<vtkPoints>::New(); //point normal array0
	vtkSmartPointer<vtkIdList> PtIdArray = 	vtkSmartPointer<vtkIdList>::New(); //point Id array in EdgePts_inn for outer surface clipping
	Progress->text="	sample...";
	for(i=0; i<Num; i++)
	{
		Progress->m_nProgress = 100*i/Num;
		Progress->Update();

		EdgePts_inn->GetPoint(i*samplestep, EdgePt);
		if(i>0)
		{
			dis=vtkMath::Distance2BetweenPoints(EdgePt, PreEdgePt);
		}
		if(dis>tolerence)
		{
			for(j=0; j<NumofInn; j++)
			{
				m_innSurface->GetPoint(j,EdgePt_Inn);
				if(EdgePt[0]==EdgePt_Inn[0]&&EdgePt[1]==EdgePt_Inn[1]&&EdgePt[2]==EdgePt_Inn[2])
				{
					PtsNormal->InsertNextPoint(dataArray->GetTuple(j));
					PtIdArray->InsertNextId(i*samplestep);
					for(m=0;m<3;m++)
					{
						PreEdgePt[m]=EdgePt[m];			
					}
				 }//if ends
			  }//for ends		
		}//if ends
	}

	vtkSmartPointer<vtkPoints> PtsNormal1 = vtkSmartPointer<vtkPoints>::New(); //new point normal array
	Num = PtIdArray->GetNumberOfIds();
	PtsNormal1->SetNumberOfPoints(Num);
	int  neighbournum = 2;
	int  iteration = 5;

	Progress->text="	iteration...";
	for(m=0; m<iteration; m++)
	{
		Progress->m_nProgress = 100*m/iteration;
		Progress->Update();

		for(i=neighbournum;i<Num+neighbournum;i++)
		{
			double nor[3]={0.,0.,0.};
			for(j=-neighbournum; j<neighbournum+1; j++)
			{
				double Nor[3];
				for(k=0; k<3; k++)
				{
					PtsNormal->GetPoint((i+j)%Num,Nor);
					nor[k]+=Nor[k];
				}
			}
			for(k=0; k<3; k++)
			{
				nor[k]=nor[k]/(2*neighbournum+1);
			}
			double length=sqrt(pow(nor[0],2) + pow(nor[1],2)+ pow(nor[2],2));
			for(k=0; k<3; k++)
			{
				nor[k]=nor[k]/length;
			}

			PtsNormal1->InsertPoint(i%Num,nor);
		}
		PtsNormal->Reset();
		PtsNormal->DeepCopy(PtsNormal1);
	}

	//Thirdly, calculate intersection points for clipping
	double x[3], y[3];
	double tuple[3];
	vtkSmartPointer<vtkPoints> ClipPts = vtkSmartPointer<vtkPoints>::New();      // points for clipping
	vtkSmartPointer<vtkPoints> ResultPts = vtkSmartPointer<vtkPoints>::New();
	double ClipPt[3];

	vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
	decimator->SetInput(outModel);
	decimator->SetTargetReduction(0.9);
	decimator->SplittingOff();
	decimator->PreserveTopologyOff();
	Progress->text="	(4/8)	Decimating...";
	decimator->AddObserver(vtkCommand::ProgressEvent, Progress);
	decimator->Update();

	Progress->text="	(5/8)	Calculate intersection points for clipping...";
	int count=0;
	for(i=0; i<Num; i++)
	{
		Progress->m_nProgress = 100*i/Num;
		Progress->Update();

		EdgePts_inn->GetPoint(PtIdArray->GetId(i),x);						
		PtsNormal1->GetPoint(i,tuple);				
		for(j=0; j<3; j++)
		{
			y[j]=x[j]+4.*tuple[j];
		}
		if(GetIntersectionPoint(x,y,ClipPt,decimator->GetOutput()))
		{
			count++;
			ClipPts->InsertNextPoint(ClipPt);
		}
	}
	Project(outModel,ClipPts,ResultPts);
	if(ResultPts->GetNumberOfPoints()<3)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(count+1);
	for (int i = 0; i<count; ++i)
	{
		lines->InsertCellPoint(i);
	}
	lines->InsertCellPoint(0);
	polyData->SetPoints(ClipPts);
	polyData->SetLines(lines);

	vtkSmartPointer<vtkCardinalSpline> spline = vtkSmartPointer<vtkCardinalSpline>::New();
	vtkSmartPointer<vtkSplineFilter> splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInput(polyData);
	splineFilter->SetNumberOfSubdivisions(/*numOfPts*2*/150); 
	splineFilter->SetSpline(spline);
	Progress->text="	(5/8)	Generating spline...";
	splineFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	splineFilter->Update();

	vtkSmartPointer<vtkPoints> SplineFilterPoints = vtkSmartPointer<vtkPoints>::New();
	SplineFilterPoints=splineFilter->GetOutput()->GetPoints();
	if(SplineFilterPoints->GetNumberOfPoints()<3)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}
//Project the points onto surface
	vtkSmartPointer<vtkPoints> ProjPoints = vtkSmartPointer<vtkPoints>::New();
	//vtkPoints* ProjPoints = vtkPoints::New();                         //projected  Points of spline
	Project(outModel,SplineFilterPoints,ProjPoints);	
	if(ProjPoints->GetNumberOfPoints()<3)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}
	//Last, clip

	vtkSmartPointer<vtkPolyData> PolyData0 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> PolyData1 = vtkSmartPointer<vtkPolyData>::New();
//-------------------------------------------------------------------------------------------------
	if(Clip(outModel,ProjPoints,PolyData0,PolyData1,2))
	{
		vtkSmartPointer<vtkDecimatePro> Decimate = vtkSmartPointer<vtkDecimatePro>::New();
		Decimate->SetInput(PolyData1);
		Decimate->SetTargetReduction(0.9);
		Decimate->SplittingOff();
		Progress->text="	(7/8)	Decimating...";
		Decimate->AddObserver(vtkCommand::ProgressEvent, Progress);
		Decimate->Update();
		vtkSmartPointer<vtkPolyData>PolyData2 = vtkSmartPointer<vtkPolyData>::New();
		PolyData2->DeepCopy(Decimate->GetOutput());

		int PtNumofInnSurface = m_innSurface->GetNumberOfPoints();
		int Step = PtNumofInnSurface/20;
		double Point0[3];
		double Point1[3];
		double PtNormal[3];
		int Count=0;

		for(int i=10; i<PtNumofInnSurface; i+=Step)
		{
			m_innSurface->GetPoint(i,Point0);
			
			dataArray->GetTuple(i,PtNormal);
			for(int j=0; j<3; j++)
			{
				Point1[j]=Point0[j]+10*PtNormal[j];
			}
			if(GetIntersectionPoint(Point0,Point1,PtNormal,PolyData2))
			{
				//normalPts->InsertNextPoint(PtNormal);
				
				
				/*double color[3]={1,0,0};
				DrawSinglePoint(PtNormal,color);*/
				Count++;
			}
		}
		if(Count>10)
		{
			m_outSurface->DeepCopy(PolyData1);
		}
		else
		{
			m_outSurface->DeepCopy(PolyData0);
		}
		m_outSurf->DeepCopy(m_outSurface);
	}
	else
	{
		QMessageBox::information(0,"Error","Clip outModel Failed!");
		return 0;
	}
	outModel->ReleaseDataFlagOn();

	/*if(!Clip(outModel,ProjPoints,m_outSurface,2))
	{ 
		QMessageBox::information(0,QObject::tr("Error"),QObject::tr("Clip outModel Failed!"));
		return 0;
	}
	m_outSurf->DeepCopy(m_outSurface);*/

	//vtkSmartPointer<vtkSTLWriter> stlWriter_outSurface = vtkSmartPointer<vtkSTLWriter>::New();
	//stlWriter_outModel->SetFileName("testoutSurface.stl");
	//stlWriter_outModel->SetInput(m_outSurface);
	//stlWriter_outModel->Write();

//4) Initial template generation
	vtkSmartPointer<vtkPoints> EdgePts_out = vtkSmartPointer<vtkPoints>::New();
	if(!GetEdgePoints(m_outSurface,EdgePts_out))
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}
	
	WallGeneration(EdgePts_out,EdgePts_inn,m_wall);
	if(!m_wall)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}
	m_mesh->DeepCopy(m_wall);

	vtkSmartPointer<vtkAppendPolyData>append = vtkSmartPointer<vtkAppendPolyData>::New();
	append->AddInput(m_outSurface);
	append->AddInput(m_innSurface);
	append->AddInput(m_wall);
	Progress->text="	(7/8)	Appending...";
	append->AddObserver(vtkCommand::ProgressEvent, Progress);
	append->Update();
	vtkSmartPointer<vtkCleanPolyData> Clean = vtkSmartPointer<vtkCleanPolyData>::New();
	Clean->SetInputConnection(append->GetOutputPort());

	Progress->text="	(7/8)	Cleaning...";
	Clean->AddObserver(vtkCommand::ProgressEvent, Progress);
	Clean->Update();
	//this->m_Output->DeepCopy(Clean->GetOutput());

	//vtkSmartPointer<vtkSTLWriter> stlWriter_wall = vtkSmartPointer<vtkSTLWriter>::New();
	//stlWriter_outModel->SetFileName("testwall.stl");
	//stlWriter_outModel->SetInput(Clean->GetOutput());
	//stlWriter_outModel->Write();

	vtkSmartPointer<vtkPolyDataNormals> normalFlip = vtkSmartPointer<vtkPolyDataNormals>::New();
	normalFlip->SetInput(Clean->GetOutput());

	normalFlip->FlipNormalsOn();
	Progress->text="	(7/8)	Normals flipping...";
	normalFlip->AddObserver(vtkCommand::ProgressEvent, Progress);
	normalFlip->Update();

	m_OutputIni->DeepCopy(normalFlip->GetOutput());
	
	//QMessageBox::information(0,"","12!");

	ren->RemoveActor(pConnectionContourObject->contourActor);
	ren->RemoveActor(pContourObject->contourActor);
	int NumofContour = QContourObjectList->size();
	for(int i=0; i<NumofContour; i++)
	{
		QContourObject* pConObj=(QContourObject*) QContourObjectList->at(i);
		ren->RemoveActor(pConObj->contourActor);
	}
	renWin->Render();
//-------------------------------------------------------------------------------------------------
	return 1;
}

//----------------------------------------------------------------------------
void InitialTemplate::GetFinalTemplate(vtkPoints *Pts)
{
		if((!m_innSurface)||(!m_outSurface)||(!m_wall))
		{
			QMessageBox::information(0,"Error","Error: GetInitialTemplate() should be invoked before this!");
			return;
		}
		m_AxisXMLPoints = Pts;
		int NumofAxes = 0.5*m_AxisXMLPoints->GetNumberOfPoints();

		double OuterRadius = 2.5;
		double InnerRadius = 1.75;
		double Length = 3.5;

		vtkSmartPointer<vtkDoubleArray> tOutArray = vtkSmartPointer<vtkDoubleArray>::New();
		tOutArray->SetNumberOfComponents(3);
		tOutArray->SetNumberOfTuples(NumofAxes);
		vtkSmartPointer<vtkDoubleArray> tInnArray = vtkSmartPointer<vtkDoubleArray>::New();
		tInnArray->SetNumberOfComponents(3);
		tInnArray->SetNumberOfTuples(NumofAxes);
		vtkSmartPointer<vtkDoubleArray> vArray = vtkSmartPointer<vtkDoubleArray>::New();
		vArray->SetNumberOfComponents(3);
		vArray->SetNumberOfTuples(NumofAxes);
		//QMessageBox::information(0,"","1");

		double q0[3], q1[3];     //Endpoints of Axis
		double t1[3];       //Intersection points of Axis and outer surfaces
		double t2[3];       //Intersection points of Axis and inner surfaces
		double v[3];       	//Normalized vector

		for(int i=0;i<NumofAxes;i++)
		{
			m_AxisXMLPoints->GetPoint(2*i,q0);
			m_AxisXMLPoints->GetPoint(2*i+1,q1);
			GetIntersectionPoint(q0,q1,t1,m_outSurface);
			tOutArray->SetTuple(i,t1);
			GetIntersectionPoint(q0,q1,t2,m_innSurface);
			tInnArray->SetTuple(i,t2);

			//Normalize			
			for(int j=0;j<3;j++)
			{				
				v[j]=q1[j]-q0[j];
			}
			double length=sqrt(pow(v[0],2) + pow(v[1],2)+ pow(v[2],2));
			for(int k=0;k<3;k++)
			{
				v[k]=v[k]/length;
			}
			vArray->SetTuple(i,v);
		}
		//QMessageBox::information(0,"","2");


		//q0[3]:intersection point of axis and inner surface-3.5
		//q1[3]:intersection point of axis and outer surface
		double q0_s[3], q1_s[3];    //Points for short cylinder
		double ClosestPoint[3]={0.,0.,0.};

		vtkSmartPointer<vtkAppendPolyData >append = 
			vtkSmartPointer<vtkAppendPolyData >::New();
		append->AddInput(m_wall);
		append->Update();
		//QMessageBox::information(0,"","3");

		vtkSmartPointer<vtkPolyData> NewoutSurface = vtkSmartPointer<vtkPolyData>::New(); 
		vtkSmartPointer<vtkPolyData> NewinnSurface = vtkSmartPointer<vtkPolyData>::New();	 
		vtkSmartPointer<vtkPolyData> PD = vtkSmartPointer<vtkPolyData>::New();

		for(int i=0; i<NumofAxes; i++)
		{
//Calculate parameters
				tOutArray->GetTuple(i,q1);
				tInnArray->GetTuple(i,q0);
				vArray->GetTuple(i,v);

				for(int j=0;j<3;j++)          
					{
						q0_s[j]=q1[j]-2.*v[j];     //inner point
						q1_s[j]=q1[j]+Length*v[j];     //outer point
						q0[j]-=4.5*v[j];
						q1[j]=q1_s[j]+v[j];
					}

			    vtkSmartPointer<vtkPolyData> SCylinderPD = vtkSmartPointer<vtkPolyData>::New();				
				CreateTube(q0_s,q1_s,OuterRadius,SCylinderPD,1);      //Draw the shorter cylinder
				//QMessageBox::information(0,"","4");

//CollisionDetection:OutSur and shorter cylinder 
				vtkSmartPointer<vtkPoints> CollisionPts_OS = vtkSmartPointer<vtkPoints>::New();
				vtkSmartPointer<vtkPolyData> SCylinderPD1 = vtkSmartPointer<vtkPolyData>::New();
				Collision(m_outSurface,SCylinderPD,NewoutSurface,SCylinderPD1,
					CollisionPts_OS,1,1,ClosestPoint,ClosestPoint);
				//QMessageBox::information(0,"","5");

//4. Wall of  SCylinderPD1
				vtkSmartPointer<vtkPoints> EdgePts00 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(SCylinderPD1,EdgePts00);
				vtkSmartPointer<vtkPolyData> wall00 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts00,CollisionPts_OS,wall00);
				//QMessageBox::information(0,"","6");

//3. Wall of  NewoutSurface
				append->AddInput(m_innSurface);
				append->AddInput(NewoutSurface);
				Progress->text="	append...";
				append->AddObserver(vtkCommand::ProgressEvent, Progress);
				append->Update();
				vtkSmartPointer<vtkCleanPolyData>clean0 = vtkSmartPointer<vtkCleanPolyData>::New();
				clean0->SetInput(append->GetOutput());
				Progress->text="	clean0...";
				clean0->AddObserver(vtkCommand::ProgressEvent, Progress);
				clean0->Update();
				PD->DeepCopy(clean0->GetOutput());
				//QMessageBox::information(0,"","7");

				vtkSmartPointer<vtkPoints> EdgePts01 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(PD,EdgePts01);
				vtkSmartPointer<vtkPolyData> wall01 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts01,CollisionPts_OS,wall01);
				//QMessageBox::information(0,"","8");

//CollisionDetection:NewShortCylinder and LC
				vtkSmartPointer<vtkPolyData>LCylinderPD = vtkSmartPointer<vtkPolyData>::New();
				CreateTube(q0,q1,InnerRadius,LCylinderPD,1);    //Draw the longer cylinder	
				vtkSmartPointer<vtkPoints>CollisionPts_SL = vtkSmartPointer<vtkPoints>::New();
				vtkSmartPointer<vtkPolyData>SCylinderPD2 = vtkSmartPointer<vtkPolyData>::New();
				vtkSmartPointer<vtkPolyData>LCylinderPD1 = vtkSmartPointer<vtkPolyData>::New();

				Collision(SCylinderPD1,LCylinderPD,SCylinderPD2,LCylinderPD1,
					CollisionPts_SL,1,1,ClosestPoint,ClosestPoint);
				//QMessageBox::information(0,"","9");

//Wall of  LCylinderPD1
				vtkSmartPointer<vtkPoints>EdgePts11 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(LCylinderPD1,EdgePts11);
				vtkSmartPointer<vtkPolyData>wall11 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts11,CollisionPts_SL,wall11);
				//QMessageBox::information(0,"","10");

//Wall of  SCylinderPD2
				append->AddInput(wall00);
				append->AddInput(wall01);
				append->AddInput(SCylinderPD2);
				Progress->text="	append...";
				append->AddObserver(vtkCommand::ProgressEvent, Progress);
				append->Update();
				vtkSmartPointer<vtkCleanPolyData>clean1 = 
					vtkSmartPointer<vtkCleanPolyData>::New();
				clean1->SetInput(append->GetOutput());
				Progress->text="	clean1...";
				clean1->AddObserver(vtkCommand::ProgressEvent, Progress);
				clean1->Update();			
				PD->DeepCopy(clean1->GetOutput());
				//QMessageBox::information(0,"","11");

				vtkSmartPointer<vtkPoints>EdgePts10 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(PD,EdgePts10);
				vtkSmartPointer<vtkPolyData>wall10 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts10,CollisionPts_SL,wall10);
				//QMessageBox::information(0,"","12");

//CollisionDetection:NewLC and InnSurface
				vtkSmartPointer<vtkPoints>CollisionPts_LI = 
					vtkSmartPointer<vtkPoints>::New();
				vtkSmartPointer<vtkPolyData>LCylinderPD2 = 
					vtkSmartPointer<vtkPolyData>::New();
				Collision(LCylinderPD1,m_innSurface,LCylinderPD2,NewinnSurface,
					CollisionPts_LI,0,1,q1,ClosestPoint);
				//QMessageBox::information(0,"","13");

//Wall of  NewLCylinderPD
				append->AddInput(wall10);
				append->AddInput(wall11);
				append->AddInput(LCylinderPD2);
				Progress->text="	append...";
				append->AddObserver(vtkCommand::ProgressEvent, Progress);
				append->Update();
				vtkSmartPointer<vtkCleanPolyData>clean2 = 
					vtkSmartPointer<vtkCleanPolyData>::New();
				clean2->SetInput(append->GetOutput());
				Progress->text="	clean2...";
				clean2->AddObserver(vtkCommand::ProgressEvent, Progress);
				clean2->Update();
				PD->DeepCopy(clean2->GetOutput());
				//QMessageBox::information(0,"","14");

				vtkSmartPointer<vtkPoints>EdgePts21 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(PD,EdgePts21);
				vtkSmartPointer<vtkPolyData>wall21 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts21,CollisionPts_LI,wall21);
				//QMessageBox::information(0,"","15");

//Wall of  NewinnSurface
				append->RemoveInput(m_innSurface);
				append->AddInput(NewinnSurface);
				append->RemoveInput(SCylinderPD2);
				append->AddInput(SCylinderPD1);
				append->RemoveInput(wall10);
				append->RemoveInput(wall11);
				append->RemoveInput(LCylinderPD2);
				append->Update();
				vtkSmartPointer<vtkCleanPolyData>clean3 = 
					vtkSmartPointer<vtkCleanPolyData>::New();
				clean3->SetInput(append->GetOutput());
				clean3->Update();
				PD->DeepCopy(clean3->GetOutput());
				//QMessageBox::information(0,"","16");

				vtkSmartPointer<vtkPoints>EdgePts20 = vtkSmartPointer<vtkPoints>::New();
				GetEdgePoints(PD,EdgePts20);
				vtkSmartPointer<vtkPolyData>wall20 = vtkSmartPointer<vtkPolyData>::New();
				WallGeneration(EdgePts20,CollisionPts_LI,wall20);
				//QMessageBox::information(0,"","17");

				append->AddInput(wall20);
				append->AddInput(wall21);
				append->AddInput(wall10);
				append->AddInput(wall11);
				append->AddInput(SCylinderPD2);
				append->AddInput(LCylinderPD2);
				append->RemoveInput(SCylinderPD1);
				append->RemoveInput(NewinnSurface);
				append->RemoveInput(NewoutSurface);
				append->Update();	
				//QMessageBox::information(0,"","18");

				m_outSurface->DeepCopy(NewoutSurface);
				m_innSurface->DeepCopy(NewinnSurface);
				//QMessageBox::information(0,"","19");

		}

		append->AddInput(m_outSurface);
		append->AddInput(m_innSurface);
		Progress->text="	append...";
		append->AddObserver(vtkCommand::ProgressEvent, Progress);
		append->Update();
		vtkSmartPointer<vtkCleanPolyData>clean = 
			vtkSmartPointer<vtkCleanPolyData>::New();
		clean->SetInput(append->GetOutput());
		Progress->text="	clean...";
		clean->AddObserver(vtkCommand::ProgressEvent, Progress);
		clean->Update();

	m_OutputFin->DeepCopy(clean->GetOutput());
}

//----------------------------------------------------------------------------
int InitialTemplate::GetFinalPlateWithPunching(double Radius, vtkPoints *Pts)
{
	vtkSmartPointer<vtkPolyData> outSurf_Output = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> innSurf_Output = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> mesh_Output = vtkSmartPointer<vtkPolyData>::New();

	vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
	normalGenerator->SetInput(m_outSurf);
	normalGenerator->ComputePointNormalsOff();
	normalGenerator->ComputeCellNormalsOn();
	normalGenerator->SplittingOff();
	Progress->text="	(1/5)	Normal generating...";
	normalGenerator->AddObserver(vtkCommand::ProgressEvent, Progress);
	normalGenerator->Update();

	CellNormalArray = normalGenerator->GetOutput()->GetCellData()->GetNormals();
	if(!CellNormalArray)
	{
		QMessageBox::information(0,"Error","Error!");
		return 0;
	}

	int count = Pts->GetNumberOfPoints();
	if(!count)
	{
		QMessageBox::information(0,"Error","Please position points for punching!");
		return 0;
	}
	double point[3];

	for(int i=0; i<count; i++)
	{
		Pts->GetPoint(i,point);

		if(Punching(m_outSurf, m_innSurf, m_mesh, point, Radius,CellNormalArray,outSurf_Output, innSurf_Output, mesh_Output))
		{
			m_outSurf->DeepCopy(outSurf_Output);
			m_innSurf->DeepCopy(innSurf_Output);
			m_mesh->DeepCopy(mesh_Output);
		}
	}

	vtkSmartPointer<vtkAppendPolyData >append = vtkSmartPointer<vtkAppendPolyData >::New();
	append->AddInput(m_mesh);
	append->AddInput(m_outSurf);
	append->AddInput(m_innSurf);
	append->Update();

	vtkSmartPointer<vtkCleanPolyData>clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInput(append->GetOutput());
	clean->Update();

	m_OutputFin->DeepCopy(clean->GetOutput());
	return 1;
}

//----------------------------------------------------------------------------
int InitialTemplate::Clip(vtkPolyData *inputMesh, vtkPoints *points, 
					 vtkPolyData *outputMesh, vtkPolyData *outputMesh1, int Mode)
{
	vtkSmartPointer<SelectPolyData> Select = vtkSmartPointer<SelectPolyData>::New();
	Select->SetInput(inputMesh);
	Select->SetLoop(points);
	Select->GenerateSelectionScalarsOn();
	if((Mode==0)||(Mode==2))
	{
		Select->SetSelectionModeToSmallestRegion();			
	}
	else
	{
		Select->SetSelectionModeToLargestRegion();	
	}
	Select->Update();

	vtkSmartPointer<vtkClipPolyData> ClipPolyData = vtkSmartPointer<vtkClipPolyData>::New();
	ClipPolyData->SetInputConnection(Select->GetOutputPort());
	
	Progress->text="	(6/8)	Clipping...";
	ClipPolyData->AddObserver(vtkCommand::ProgressEvent, Progress);
	ClipPolyData->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(ClipPolyData->GetOutputPort());

	connectivityFilter->SetExtractionModeToClosestPointRegion();
	connectivityFilter->SetClosestPoint(points->GetPoint(0));
	//connectivityFilter->SetExtractionModeToLargestRegion(); 
	Progress->text="	(6/8)	Screening points...";
	connectivityFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	connectivityFilter->Update();

	int npts = connectivityFilter->GetOutput()->GetNumberOfPoints();
	if(npts&&Mode!=2)
	{
		outputMesh->DeepCopy(connectivityFilter->GetOutput());
		return 1;
	}
	else	if(npts&&Mode==2)
	{
		Select->SetSelectionModeToLargestRegion();
		Select->Update();
		vtkSmartPointer<vtkClipPolyData>ClipPolyData0 = vtkSmartPointer<vtkClipPolyData>::New();
		ClipPolyData0->SetInputConnection(Select->GetOutputPort());
		Progress->text="	(7/8)	Clipping...";
		ClipPolyData0->AddObserver(vtkCommand::ProgressEvent, Progress);
		ClipPolyData0->Update();
		outputMesh->DeepCopy(connectivityFilter->GetOutput());
		outputMesh1->DeepCopy(ClipPolyData0->GetOutput());
		return 1;
	}
	else
	{
		return 0;
	}
}

//----------------------------------------------------------------------------
int InitialTemplate::OuterModelGeneration(vtkPolyData* inputPD,vtkPolyData* outputPD)
{

	vtkSmartPointer<vtkImplicitModeller> implicitModeller = vtkSmartPointer<vtkImplicitModeller>::New();
	implicitModeller->SetSampleDimensions(50,50,50);
	implicitModeller->SetInput(inputPD);
	implicitModeller->AdjustBoundsOn();
	implicitModeller->SetAdjustDistance(.1);         // Adjust by 10%
	implicitModeller->SetMaximumDistance(.1);
	implicitModeller->SetCapping(0);
	Progress->text="	(2/8)	Model generating...";
	implicitModeller->AddObserver(vtkCommand::ProgressEvent, Progress);
	implicitModeller->Update();

	// Create the 0 isosurface
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(implicitModeller->GetOutputPort());

	contourFilter->SetValue(0, thickness); 
	Progress->text="	(3/8)	contourFilter...";
	contourFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	contourFilter->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(contourFilter->GetOutputPort());

	connectivityFilter->SetExtractionModeToLargestRegion();
	Progress->text="	(3/8)	connectivityFilter...";
	connectivityFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	connectivityFilter->Update();

	if(!connectivityFilter->GetOutput()->GetNumberOfPoints())
	{
		return 0;
	}

	vtkSmartPointer<vtkTriangleFilter> TriangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	TriangleFilter->SetInputConnection(connectivityFilter->GetOutputPort());

	Progress->text="	(3/8)	TriangleFilter...";
	TriangleFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	TriangleFilter->Update();

	int numberOfSubdivisions = 2;
	vtkSmartPointer<vtkLoopSubdivisionFilter> subdivisionFilter = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	subdivisionFilter->SetNumberOfSubdivisions(numberOfSubdivisions);
	subdivisionFilter->SetInputConnection(TriangleFilter->GetOutputPort());

	Progress->text="	(3/8)	Subdivision...";
	subdivisionFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	subdivisionFilter->Update();

	vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
	smoothFilter->SetInputConnection(subdivisionFilter->GetOutputPort());

	smoothFilter->SetNumberOfIterations(50);
	smoothFilter->SetRelaxationFactor(.1);    //small relaxation factors and large numbers of iterations are more stable
	smoothFilter->BoundarySmoothingOn();
	Progress->text="	(3/8)	Smoothing...";
	smoothFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	smoothFilter->Update();

	outputPD->DeepCopy(smoothFilter->GetOutput());
	return 1;
}

//----------------------------------------------------------------------------
int InitialTemplate::GetEdgePoints(vtkPolyData* inputPD, vtkPoints* EdgePts)
{
	vtkSmartPointer<vtkFeatureEdges> FE = vtkSmartPointer<vtkFeatureEdges>::New();
	FE->SetInput(inputPD);
	FE->BoundaryEdgesOn();
	FE->FeatureEdgesOff();
	FE->ManifoldEdgesOff();
	FE->NonManifoldEdgesOff();

	FE->AddObserver(vtkCommand::ProgressEvent, Progress);
	FE->Update();

	int Num = FE->GetOutput()->GetNumberOfPoints();
	if(!Num)
	{			
		return 0;
	}
	vtkSmartPointer<vtkPolyData> FEPolyData = FE->GetOutput();

	vtkSmartPointer<vtkIdList>cellidlist = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList>ptidlist = vtkSmartPointer<vtkIdList>::New();	
	vtkSmartPointer<vtkIdList>orderlist = vtkSmartPointer<vtkIdList>::New();	
	orderlist->SetNumberOfIds(Num);
	orderlist->InsertId(0,0);

	int CurrentptID=0;
	int CurrentcellID=-1;
	for(int i=1;i<Num;i++)
	{
		FEPolyData->GetPointCells(CurrentptID,cellidlist);
		if(cellidlist->GetId(0)!=CurrentcellID)
		{
			CurrentcellID=cellidlist->GetId(0);
		}
		else
		{
			CurrentcellID=cellidlist->GetId(1);
		}

		FEPolyData->GetCellPoints(CurrentcellID,ptidlist);
		if(ptidlist->GetId(0)!=CurrentptID)
		{
			orderlist->InsertId(i,ptidlist->GetId(0));
			CurrentptID=ptidlist->GetId(0);
		}
		else
		{
			orderlist->InsertId(i,ptidlist->GetId(1));
			CurrentptID=ptidlist->GetId(1);
		}
	}

	vtkSmartPointer<vtkPoints> EdgePoints = FEPolyData->GetPoints();

	double coo[3];
	for(int i=0;i<Num;i++)
	{
		EdgePoints->GetPoint(orderlist->GetId(i),coo);
		EdgePts->InsertPoint (i, coo);
	}
	return 1;
}

//----------------------------------------------------------------------------
int InitialTemplate::GetIntersectionPoint(double p1[3],double p2[3],double x[3],vtkPolyData* PD)
{
		//x[3]:The coordinate of the intersection
		double tolerance = .001;
		double t; // Parametric coordinate of intersection (0 (corresponding to p1) to 1 (corresponding to p2))	
		double pcoords[3];
		int subId;
		vtkSmartPointer<vtkModifiedBSPTree> bspTree = vtkSmartPointer<vtkModifiedBSPTree>::New();
		bspTree->SetDataSet(PD);
		//Progress->text="bspTree...";
		//bspTree->AddObserver(vtkCommand::ProgressEvent, Progress);
		return bspTree->IntersectWithLine(p1, p2, tolerance, t, x, pcoords, subId);
}

//----------------------------------------------------------------------------
void InitialTemplate::WallGeneration(vtkPoints* EdgePts0,vtkPoints* EdgePts1,vtkPolyData*WallPD)
{
		int OutNum = EdgePts0->GetNumberOfPoints();
		int InnNum = EdgePts1->GetNumberOfPoints();
		if(OutNum == 0||InnNum == 0)
		{
			return;
		}
//Add inner edge points to EdgePts0 in correct order
//1)Find the closest point to Id0 of EdgePts0
		//vtkSmartPointer<vtkPoints>InnEdgePoints = InnSurFE->GetPoints();
		double CooofId0[3];	
		double CooofCur[3];
		float distance=10000.;
		int p;           //closest point ID
		int i, j, index;
		EdgePts0->GetPoint(0,CooofId0);
		for(index=0; index<InnNum; index++)
		{
			EdgePts1->GetPoint(index,CooofCur);
			float NewDis=pow(CooofCur[0] - CooofId0[0],2) + pow(CooofCur[1] - CooofId0[1],2) + pow(CooofCur[2] - CooofId0[2],2);
			if(NewDis<distance)
			{
				distance=NewDis;
				p=index;
			}
		 }

//2)Direction of curve
		double CooofId1[3];
		double VectorOut[3];
		double VectorInn[3];
		EdgePts0->GetPoint(1,CooofId1);
		for(i=0;i<3;i++)
		{
			VectorOut[i]=CooofId1[i]-CooofId0[i];
		}
		int nextID=p+1>=InnNum?0:(p+1);
		double next[3];
		EdgePts1->GetPoint(nextID,next);
		double zero[3];
		EdgePts1->GetPoint(p,zero);
		for(i=0;i<3;i++)
		{
			VectorInn[i]=next[i]-zero[i];
		}

//3)Add to EdgePts0
		double coo[3];
		if(VectorOut[0]*VectorInn[0]+VectorOut[1]*VectorInn[1]+VectorOut[2]*VectorInn[2]<0)
		{
				for(i=OutNum;i<InnNum+OutNum;i++)
			{
				EdgePts1->GetPoint(p,coo);
				EdgePts0->InsertPoint (i, coo);
				p=(p==0)?InnNum-1:p-1;
			}
		}
		else
		{
			for(i=OutNum;i<InnNum+OutNum;i++)
			{
				EdgePts1->GetPoint(p,coo);
				EdgePts0->InsertPoint (i, coo);
				p=(p==InnNum-1)?0:p+1;
			}
		}

		vtkSmartPointer<vtkDenseArray<double>> Array = vtkSmartPointer<vtkDenseArray<double> >::New();
		Array->Resize(OutNum,InnNum,3);
		double OutCoo[3];
		double InnCoo[3];
		double Distance;
		for(i=0;i<OutNum;i++)             //Calculate distance between two points, and put the distance into "0"
		{
			EdgePts0->GetPoint(i,OutCoo);
			for(j=0;j<InnNum;j++)
			{
				EdgePts0->GetPoint(OutNum+j,InnCoo);
				Distance=pow(OutCoo[0] - InnCoo[0],2) + pow(OutCoo[1] - InnCoo[1],2) + pow(OutCoo[2] - InnCoo[2],2);
				Array->SetValue(i,j,0,Distance);
			}
		}

//Calculate shortest path from source to current node, put it into "1"
		Array->SetValue(0,0,1,0.);
		for(j=1;j<InnNum;j++)
		{
			Distance=Array->GetValue(0,j-1,1);
			Distance+=Array->GetValue(0,j,0);
			Array->SetValue(0,j,1,Distance);
			Array->SetValue(0,j,2,1.);
		}
		for(i=1;i<OutNum;i++)
		{
			Distance=Array->GetValue(i-1,0,1);
			Distance+=Array->GetValue(i,0,0);
			Array->SetValue(i,0,1,Distance);
			Array->SetValue(i,0,2,-1.);
		}
		double left;
		double down;
		for(i=1;i<OutNum;i++)
		{
			for(j=1;j<InnNum;j++)
			{
				left=Array->GetValue(i,j-1,1);
				down=Array->GetValue(i-1,j,1);
				if(left<down)
				{
					left+=Array->GetValue(i,j,0);
					Array->SetValue(i,j,1,left);
					Array->SetValue(i,j,2,1.);       //      left: 1.;down:-1.
				}
				else
				{
					down+=Array->GetValue(i,j,0);
					Array->SetValue(i,j,1,down);
					Array->SetValue(i,j,2,-1.);
				}
			}
		}

// Create mesh
		vtkSmartPointer<vtkCellArray> triangles =  vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkTriangle> triangle =  vtkSmartPointer<vtkTriangle>::New();
		int index_i=OutNum-1;
		int index_j=InnNum-1;
		for(p=0;p<OutNum+InnNum-2;p++)
		{
			if(Array->GetValue(index_i,index_j,2)<0)
			{
				triangle->GetPointIds()->InsertId(0,OutNum+index_j);
				triangle->GetPointIds()->InsertId(1,index_i);
				triangle->GetPointIds()->InsertId(2,index_i-1);			
				index_i-=1;
			}
			else
			{
				triangle->GetPointIds()->InsertId(0,OutNum+index_j);
				triangle->GetPointIds()->InsertId(1,index_i);
				triangle->GetPointIds()->InsertId(2,OutNum+index_j-1);		
				index_j-=1;
			}
			triangles->InsertNextCell(triangle);
		}
		triangle->GetPointIds()->InsertId(0,0);
		triangle->GetPointIds()->InsertId(1,OutNum);
		triangle->GetPointIds()->InsertId(2,OutNum+InnNum-1);	
		triangles->InsertNextCell(triangle);
		triangle->GetPointIds()->InsertId(0,0);
		triangle->GetPointIds()->InsertId(1,OutNum-1);
		triangle->GetPointIds()->InsertId(2,OutNum+InnNum-1);	
		triangles->InsertNextCell(triangle);

		WallPD->SetPoints (EdgePts0);
		WallPD->SetPolys (triangles);

}

//----------------------------------------------------------------------------
void InitialTemplate::CollisionPoints(vtkPolyData* PD0,vtkPolyData* PD1,vtkPoints*CollisionPts)
{
	vtkSmartPointer<vtkMatrix4x4> matrix0= vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> matrix1 = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkSmartPointer<vtkCollisionDetectionFilter> collide = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
	collide->SetInputConnection(0, PD0->GetProducerPort());
	collide->SetMatrix(0, matrix0);
	collide->SetInputConnection(1, PD1->GetProducerPort());
	collide->SetMatrix(1, matrix1);
	collide->SetBoxTolerance(0.0);
	collide->SetCellTolerance(0.0);
	collide->SetNumberOfCellsPerBucket(2);
	collide->SetCollisionModeToAllContacts();
	collide->GenerateScalarsOn();
	Progress->iStage = 100;
	Progress->text="	collide...";
	collide->AddObserver(vtkCommand::ProgressEvent, Progress);

	vtkSmartPointer<vtkCleanPolyData>clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInputConnection(collide->GetContactsOutputPort());
	Progress->text="	clean...";
	clean->AddObserver(vtkCommand::ProgressEvent, Progress);
	clean->Update();

	vtkSmartPointer<vtkStripper>stripper = vtkSmartPointer<vtkStripper>::New();
	stripper->SetInputConnection(clean->GetOutputPort());
	Progress->text="	stripper...";
	stripper->AddObserver(vtkCommand::ProgressEvent, Progress);
	stripper->Update();

	vtkSmartPointer<vtkCellArray>lines = stripper->GetOutput()->GetLines();
	vtkSmartPointer<vtkPoints>allPoints = stripper->GetOutput()->GetPoints();
	vtkIdType numPts, *pts;

	lines->InitTraversal();
	lines->GetNextCell(numPts, pts);
	CollisionPts->Reset();
	// don't close the loop by inserting the last point...
	// vtkSelectPolyData does it for you (and crashes if you do it)
	for (int i =0; i<numPts-1; i++)
	{
		CollisionPts->InsertNextPoint(allPoints->GetPoint(pts[i]));
	}
}

//----------------------------------------------------------------------------
void InitialTemplate::CreateTube(double x0[3],double x1[3], double Radius,vtkPolyData*OutputPD, bool IsCappingOn)
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
	tubeFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	tubeFilter->Update();

	vtkSmartPointer<vtkTriangleFilter> TriangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	TriangleFilter->SetInput(tubeFilter->GetOutput());
	TriangleFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	TriangleFilter->Update();

	vtkSmartPointer<vtkLinearSubdivisionFilter> SubdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	SubdivisionFilter->SetNumberOfSubdivisions(3);
	SubdivisionFilter->SetInput(TriangleFilter->GetOutput());
	SubdivisionFilter->AddObserver(vtkCommand::ProgressEvent, Progress);
	SubdivisionFilter->Update();

	OutputPD->DeepCopy(SubdivisionFilter->GetOutput());
}

//----------------------------------------------------------------------------
void InitialTemplate::Collision(vtkPolyData* PD0,vtkPolyData* PD1,vtkPolyData* OutputPD0,
										 vtkPolyData* OutputPD1, vtkPoints*CollisionPts, 
										 int Mode0, int Mode1, double x0[3], double x1[3])
{
	//Collision:RemoveDeletedCells & GetCollisonPts
	//1) CollisionDetection
	vtkSmartPointer<vtkMatrix4x4> matrix0 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> matrix1 = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkSmartPointer<vtkCollisionDetectionFilter> collide = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
	collide->SetInputConnection(0, PD0->GetProducerPort());
	collide->SetMatrix(0, matrix0);
	collide->SetInputConnection(1, PD1->GetProducerPort());
	collide->SetMatrix(1, matrix1);
	collide->SetBoxTolerance(0.0);
	collide->SetCellTolerance(0.0);
	collide->SetNumberOfCellsPerBucket(2);
	collide->SetCollisionModeToAllContacts();
	collide->GenerateScalarsOn();
	collide->AddObserver(vtkCommand::ProgressEvent, Progress);
	collide->Update();

	//2) GetCollisonPts
	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInputConnection(collide->GetContactsOutputPort());
	clean->AddObserver(vtkCommand::ProgressEvent, Progress);
	clean->Update();

	vtkSmartPointer<vtkPolyData> PolyData = vtkSmartPointer<vtkPolyData>::New();
	PolyData->DeepCopy(clean->GetOutput());

	int num=PolyData->GetNumberOfCells();
	for(int i=0;i<num;i++)
	{
		if(PolyData->GetCellType(i)!=3)
		{
			PolyData->DeleteCell(i);
		}
	}
	PolyData->RemoveDeletedCells();
	int NumofCollisionPts=PolyData->GetPoints()->GetNumberOfPoints();

	CollisionPts->SetNumberOfPoints(NumofCollisionPts);
	vtkIdType LastCellId=-1;
	vtkIdType CurrentptId=0;
	CollisionPts->InsertPoint(0,PolyData->GetPoints()->GetPoint(0));

	vtkSmartPointer<vtkIdList> cellidlist = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList> ptidlist = vtkSmartPointer<vtkIdList>::New();
	for(int i=1;i<NumofCollisionPts;i++)
	{
		PolyData->GetPointCells(CurrentptId,cellidlist);

		if(cellidlist->GetId(0)!=LastCellId)
		{
			LastCellId=cellidlist->GetId(0);
		}
		else
		{
			LastCellId=cellidlist->GetId(1);
		}

		PolyData->GetCellPoints(LastCellId,ptidlist);
		if(ptidlist->GetId(0)!=CurrentptId)
		{
			CurrentptId=ptidlist->GetId(0);
		}
		else
		{
			CurrentptId=ptidlist->GetId(1);
		}
		CollisionPts->InsertPoint(i,PolyData->GetPoint(CurrentptId));
	}

	//3) PD0:RemoveDeletedCells
	OutputPD0->DeepCopy(PD0);
	vtkSmartPointer<vtkIdTypeArray> IdArray0 = vtkSmartPointer<vtkIdTypeArray>::New();
	IdArray0=collide->GetContactCells(0);
	int num0=IdArray0->GetNumberOfTuples();
	for(int i=0;i<num0;i++)
	{
		OutputPD0->DeleteCell(IdArray0->GetValue(i));
	}
	OutputPD0->RemoveDeletedCells();

	vtkSmartPointer<vtkCleanPolyData> clean0 = vtkSmartPointer<vtkCleanPolyData>::New();
	clean0->SetInput(OutputPD0);
	clean0->AddObserver(vtkCommand::ProgressEvent, Progress);
	clean0->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> ConnectivityFilter0 = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	ConnectivityFilter0->SetInput(clean0->GetOutput());

	if(Mode0)
	{
		ConnectivityFilter0->SetExtractionModeToLargestRegion();
	}
	else
	{
		ConnectivityFilter0->SetExtractionModeToClosestPointRegion();
		ConnectivityFilter0->SetClosestPoint(x0);
	}
	ConnectivityFilter0->AddObserver(vtkCommand::ProgressEvent, Progress);
	ConnectivityFilter0->Update();
	OutputPD0->DeepCopy(ConnectivityFilter0->GetOutput());


	//4) PD1:RemoveDeletedCells
	OutputPD1->DeepCopy(PD1);
	vtkSmartPointer<vtkIdTypeArray> IdArray1 = vtkSmartPointer<vtkIdTypeArray>::New();
	IdArray1=collide->GetContactCells(1);
	int num1=IdArray1->GetNumberOfTuples();
	for(int i=0;i<num1;i++)
	{
		OutputPD1->DeleteCell(IdArray1->GetValue(i));
	}
	OutputPD1->RemoveDeletedCells();
	vtkSmartPointer<vtkCleanPolyData> clean1 = vtkSmartPointer<vtkCleanPolyData>::New();
	clean1->SetInput(OutputPD1);
	clean1->AddObserver(vtkCommand::ProgressEvent, Progress);
	clean1->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> ConnectivityFilter1 = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	ConnectivityFilter1->SetInput(clean1->GetOutput());

	if(Mode1)
	{
		ConnectivityFilter1->SetExtractionModeToLargestRegion();
	}
	else
	{
		ConnectivityFilter1->SetExtractionModeToClosestPointRegion();
		ConnectivityFilter1->SetClosestPoint(x1);
	}
	ConnectivityFilter1->AddObserver(vtkCommand::ProgressEvent, Progress);
	ConnectivityFilter1->Update();
	OutputPD1->DeepCopy(ConnectivityFilter1->GetOutput());
}

//----------------------------------------------------------------------------
int InitialTemplate::Punching(vtkPolyData*outSurf_Input, vtkPolyData*innSurf_Input, vtkPolyData*Mesh_Input, 
										double point[3], double Radius, vtkDataArray *CellNormalArray,
										vtkPolyData*outSurf_Output, vtkPolyData*innSurf_Output, vtkPolyData*Mesh_Output)
{
	//QMessageBox::information(0,"","Punching STARTS!");
//1) Get point normal. Actually it's the normal of the triangle cell which contains the point
vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
		cellLocator->SetDataSet(outSurf_Input);
		cellLocator->BuildLocator();

		double closestPoint[3];				//the coordinates of the closest point will be returned here
		double closestPointDist2;			//the squared distance to the closest point will be returned here
		vtkIdType cellId;					//the cell id of the cell containing the closest point will be returned here
		int subId;							//this is rarely used (in triangle strips only, I believe)

		cellLocator->FindClosestPoint(point, closestPoint, cellId, subId, closestPointDist2);

		double Normal[3];
		CellNormalArray->GetTuple(cellId, Normal);

		//QMessageBox::information(0,"","0");
//2) Test direction of the normal
		vtkSmartPointer<vtkCellLocator> cellLocator1 = vtkSmartPointer<vtkCellLocator>::New();
		cellLocator1->SetDataSet(innSurf_Input);
		cellLocator1->BuildLocator();
		cellLocator1->FindClosestPoint(point, closestPoint, cellId, subId, closestPointDist2);

		//QMessageBox::information(0,"","1");

		double newclosestPointDist2;
		double newpoint[3];
		for(int i=0; i<3; i++)
		{
			newpoint[i] = point[i]+Normal[i];
		}
		//QMessageBox::information(0,"","2");
		cellLocator1->FindClosestPoint(newpoint, closestPoint, cellId, subId, newclosestPointDist2);
		//QMessageBox::information(0,"","3");
		if(newclosestPointDist2<closestPointDist2)
		{
			for(int i=0; i<3; i++)
			{
				Normal[i] = -Normal[i];
			}
		}
		//QMessageBox::information(0,"","4");
		
//3) Create Cylinder
		double xo[3], xi[3];
		for(int i=0; i<3; i++)
		{
			xo[i] = point[i]+thickness*Normal[i];
			xi[i] = point[i]-3*thickness*Normal[i];
			/*double color[3]={0,1,0};
			DrawSinglePoint(point,color);*/
		}
		vtkSmartPointer<vtkPolyData> Cylinder = vtkSmartPointer<vtkPolyData>::New();

		Progress->text="	(2/5)	Tubes creating...";
		CreateTube(xo, xi, Radius, Cylinder, 1);

		//QMessageBox::information(0,"","5");

		//QMessageBox::information(0,"","6");

//3.5)Test if the cylinder intersects the wall of plate
		double ClosestPoint[3]={0.,0.,0.};
		vtkSmartPointer<vtkMatrix4x4>matrix0 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkSmartPointer<vtkMatrix4x4>matrix1 = vtkSmartPointer<vtkMatrix4x4>::New();

		vtkSmartPointer<vtkCollisionDetectionFilter>collide = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
		collide->SetInputConnection(0, Cylinder->GetProducerPort());
		collide->SetMatrix(0, matrix0);
		collide->SetInputConnection(1, m_wall->GetProducerPort());
		collide->SetMatrix(1, matrix1);
		collide->SetBoxTolerance(0.0);
		collide->SetCellTolerance(0.0);
		collide->SetNumberOfCellsPerBucket(2);
		collide->SetCollisionModeToAllContacts();
		collide->GenerateScalarsOn();
		collide->Update();

		vtkSmartPointer<vtkCleanPolyData>clean = vtkSmartPointer<vtkCleanPolyData>::New();
		clean->SetInputConnection(collide->GetContactsOutputPort());
		clean->Update();
		int numCollision=clean->GetOutput()->GetNumberOfCells();

		if(numCollision)
		{
			return 0;   //punching cylinder intersects the wall
		}
		else
		{
			vtkSmartPointer<vtkAppendPolyData >append = 
				vtkSmartPointer<vtkAppendPolyData >::New();
			append->AddInput(Mesh_Input);
			append->Update();

//4) Collision: Cylinder&outSurf_Input: SetExtractionModeToLargestRegion
		vtkSmartPointer<vtkPoints> CollisionPts0 = vtkSmartPointer<vtkPoints>::New();

		vtkSmartPointer<vtkPolyData> NewCylinder = vtkSmartPointer<vtkPolyData>::New();
		Progress->text="	(3/5)	Collision detection...";
		Collision(outSurf_Input, Cylinder,outSurf_Output, NewCylinder, CollisionPts0, 1, 0, ClosestPoint, xi);

//Wall of  NewCylinder
		vtkSmartPointer<vtkPoints> EdgePts00 = vtkSmartPointer<vtkPoints>::New();
		Progress->text="	(4/5)	Extracting edges...";
		GetEdgePoints(NewCylinder,EdgePts00);
		vtkSmartPointer<vtkPolyData> wall00 = vtkSmartPointer<vtkPolyData>::New();
		WallGeneration(EdgePts00,CollisionPts0,wall00);

//Wall of  NewoutSurface
		append->AddInput(innSurf_Input);
		append->AddInput(outSurf_Output);
		append->Update();

		vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
		clean->SetInput(append->GetOutput());
		clean->Update();

		vtkSmartPointer<vtkPoints> EdgePts01 = vtkSmartPointer<vtkPoints>::New();
		Progress->text="	(4/5)	Extracting edges...";
		GetEdgePoints(clean->GetOutput(),EdgePts01);
		vtkSmartPointer<vtkPolyData> wall01 = vtkSmartPointer<vtkPolyData>::New();
		WallGeneration(EdgePts01,CollisionPts0,wall01);

//5) Collision: Cylinder&innSurf_Input: SetExtractionModeToClosestPointRegion(point)
		vtkSmartPointer<vtkPoints> CollisionPts1 = vtkSmartPointer<vtkPoints>::New();
		Cylinder->DeepCopy(NewCylinder);
		Progress->text="	(5/5)	Collision detection...";
		Collision(innSurf_Input, Cylinder,innSurf_Output, NewCylinder, CollisionPts1, 1, 0, ClosestPoint, xo);
//Wall of  NewCylinder
		vtkSmartPointer<vtkPoints> EdgePts10 = vtkSmartPointer<vtkPoints>::New();
		append->AddInput(wall00);
		append->AddInput(wall01);
		append->AddInput(NewCylinder);
		append->Update();

		clean->RemoveAllInputs();
		clean->SetInputConnection(append->GetOutputPort());
		clean->Update();
		GetEdgePoints(clean->GetOutput(),EdgePts10);
		vtkSmartPointer<vtkPolyData> wall10 = vtkSmartPointer<vtkPolyData>::New();
		WallGeneration(EdgePts10,CollisionPts1,wall10);
//Wall of  NewinnSurface
		append->RemoveInput(NewCylinder);
		append->RemoveInput(innSurf_Input);
		append->AddInput(innSurf_Output);
		append->AddInput(Cylinder);
		append->Update();

		clean->RemoveAllInputs();
		clean->SetInputConnection(append->GetOutputPort());

		clean->Update();

		vtkSmartPointer<vtkPoints> EdgePts11 = vtkSmartPointer<vtkPoints>::New();
		GetEdgePoints(clean->GetOutput(),EdgePts11);

		vtkSmartPointer<vtkPolyData> wall11 = vtkSmartPointer<vtkPolyData>::New();
		WallGeneration(EdgePts11,CollisionPts1,wall11);

		append->RemoveInput(Cylinder);
		append->RemoveInput(innSurf_Output);
		append->RemoveInput(outSurf_Output);
		append->AddInput(wall10);
		append->AddInput(wall11);
		append->AddInput(NewCylinder);

		append->Update();

		Mesh_Output->DeepCopy(append->GetOutput());
		return 1;
		}
}

//----------------------------------------------------------------------------
void InitialTemplate::Project(vtkPolyData* inputMesh,vtkPoints* InitialPts,vtkPoints* ResultPts)
{
	//==========================Project the points(InitialPts) onto surface(inputMesh)
	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	cellLocator->SetDataSet(inputMesh);
	cellLocator->BuildLocator();
	Progress->text="	(5/8)	Locating...";
	cellLocator->AddObserver(vtkCommand::ProgressEvent, Progress);

	double closestPoint[3];				//the coordinates of the closest point will be returned here
	double closestPointDist2;			//the squared distance to the closest point will be returned here
	vtkIdType cellId;							//the cell id of the cell containing the closest point will be returned here
	int subId;										//this is rarely used (in triangle strips only, I believe)
	ResultPts->SetNumberOfPoints(InitialPts->GetNumberOfPoints());

	int sum = InitialPts->GetNumberOfPoints();
	Progress->text="	(5/8)	Projecting...";
	for(int i=0;i<sum/*-1*/;i++)
	{
		Progress->m_nProgress = 100*i/sum;
		Progress->Update();
		cellLocator->FindClosestPoint(InitialPts->GetPoint(i), closestPoint, cellId, subId, closestPointDist2);
		ResultPts->InsertPoint(i,closestPoint);
	}

}

void InitialTemplate::DeepCopy(InitialTemplate* data)
{
	m_Output->DeepCopy(data->m_Output);
	m_OutputIni->DeepCopy(data->m_OutputIni);
	m_OutputFin->DeepCopy(data->m_OutputFin);
	m_AxisXMLPoints->DeepCopy(data->m_AxisXMLPoints);
	m_Mesh->DeepCopy(data->m_Mesh);
	m_Loop->DeepCopy(data->m_Loop);
	m_innSurface->DeepCopy(data->m_innSurface);
	m_outSurface->DeepCopy(data->m_outSurface);
	m_wall->DeepCopy(data->m_wall);
	m_innSurf->DeepCopy(data->m_innSurf);
	m_outSurf->DeepCopy(data->m_outSurf);
	m_mesh->DeepCopy(data->m_mesh);
	/*CellNormalArray->DeepCopy(data->CellNormalArray);*/
}

void InitialTemplate::FixPoint(double *fTipPoint)
{
	if(!IsContourClosed)
	{
	ControlPts->InsertNextPoint(fTipPoint);
	AllControlPts->InsertNextPoint(fTipPoint);

	if(!IsFirstPt)
	{
		if(IsFirstUserPt==1)
		{
			ren->RemoveActor(FirstUserPtActor);
			renWin->Render();	
			IsFirstUserPt = 2;
		}

		SaveCurveCommand->sign = true;
		DrawContour(ControlPts, 0);		

		if(Multi_Angle)
		{
			if(ControlPts->GetNumberOfPoints()<=3)
			{
				double x[3];
				double y[3];
				ConnectionControlPts->GetPoint(0,x);
				ConnectionControlPts->GetPoint(1,y);
				ConnectionControlPts->Reset();
				ConnectionControlPts->SetNumberOfPoints(4);
				ConnectionControlPts->InsertPoint(0,x);
				ConnectionControlPts->InsertPoint(1,y);
				SplineFilterPoints->GetPoint(0,x);
				SplineFilterPoints->GetPoint(1,y);

				for(int i=0; i<3; i++)
				{
					y[i]=x[i]-y[i];
				}
				double length=sqrt(pow(y[0],2) + pow(y[1],2)+ pow(y[2],2));
				for(int k=0;k<3;k++)
				{
					y[k]=y[k]/length;
				}
				for(int i=0; i<3; i++)
				{
					y[i]=x[i]+0.5*y[i];
				}
				ConnectionControlPts->InsertPoint(2,y);
				ConnectionControlPts->InsertPoint(3,x);
				DrawConnectionContour(ConnectionControlPts);	
			}			
		}			
	}
	else
	{
		IsFirstPt=false;
		if(Multi_Angle)
		{
			double x[3];
			double y[3];
			ConnectionControlPts->GetPoint(0,x);
			ConnectionControlPts->GetPoint(1,y);
			for(int i=0; i<3; i++)
			{
				y[i]=x[i]-y[i];
			}
			double length=sqrt(pow(y[0],2) + pow(y[1],2)+ pow(y[2],2));
			for(int k=0;k<3;k++)
			{
				y[k]=y[k]/length;
			}
			for(int i=0; i<3; i++)
			{
				y[i]=x[i]+0.5*y[i];
			}
			ConnectionControlPts->InsertPoint(1,y);
			ConnectionControlPts->InsertPoint(2,fTipPoint);
			DrawConnectionContour(ConnectionControlPts);
			SaveCurveCommand->sign = true;
		}
		else
		{
			if(IsFirstUserPt==0)
			{
				//Draw the first point
				IsFirstUserPt = 1;
				vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
				sphere->SetCenter(fTipPoint[0], fTipPoint[1], fTipPoint[2]);
				sphere->SetRadius(tubeRadius);
				sphere->SetPhiResolution(100);
				sphere->SetThetaResolution(12);

				vtkSmartPointer<vtkPolyDataMapper> SphereMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
				SphereMapper->SetInputConnection(sphere->GetOutputPort());
				FirstUserPtActor->GetProperty()->SetColor(0,1,0);
				FirstUserPtActor->SetMapper(SphereMapper);
				ren->AddActor(FirstUserPtActor);
				renWin->Render();
			}		
		}
	}
	}
	else
	{
		QMessageBox::information(0,QObject::tr(""),QObject::tr("The loop has been closed. Please undo the last step if more points need to be added!"));
	}
	//QMessageBox::information(0, "","!");
}

void InitialTemplate::SaveCurrentCurve()
{	
	SaveCurveCommand->sign = false;
	IsFirstPt = true;
	int num;
	double color[3]={123.,104.,238.};
	QContourObject *pConnectionCO;
	QContourObject *pCO;
	if(Multi_Angle)
	{
		pConnectionCO = new QContourObject();
		QContourObjectList->append(pConnectionCO);
		vtkPoints *ConSplFilPts = pConnectionCO->Pts;
		ConSplFilPts->DeepCopy(ConnectionSplineFilterPoints);
		num = ConnectionSplineFilterPoints->GetNumberOfPoints();
		for(int i=1; i<num-1; i++)
		{
			m_AllSubdivisionPts->InsertNextPoint(ConnectionSplineFilterPoints->GetPoint(i));
		}		
		vtkActor *ConActor = pConnectionCO->contourActor;
		vtkPolyDataMapper *ConMapper = pConnectionCO->contourMapper;
		ConMapper->SetInput(ConnectionSubdivisionPtsMapper->GetInput());
		ConActor->GetProperty()->SetColor(color[0],color[1],color[2]);
		ConActor->SetMapper(ConMapper);
		ConnectionSubdivisionPtsMapper->RemoveAllInputs();
		ren->AddActor(ConActor);
	}
	num = SplineFilterPoints->GetNumberOfPoints();	
	
	if(num)
	{
		pCO = new QContourObject();
		vtkPoints *SplFilPts = pCO->Pts;
		SplFilPts->DeepCopy(SplineFilterPoints);
		for(int i=0; i<num; i++)
		{
			m_AllSubdivisionPts->InsertNextPoint(SplineFilterPoints->GetPoint(i));
		}

		vtkActor *ConActor = pCO->contourActor;
		vtkPolyDataMapper *ConMapper = pCO->contourMapper;

		ConMapper->SetInputConnection(SubdivisionPtsMapper->GetInputConnection(0,0));	
		ConActor->GetProperty()->SetColor(color[0],color[1],color[2]);
		ConActor->SetMapper(ConMapper);
		SubdivisionPtsMapper->RemoveAllInputs();
		ren->AddActor(ConActor);
		QContourObjectList->append(pCO);
	}
	//else
	//{
	//	num = ConnectionSplineFilterPoints->GetNumberOfPoints();
	//	m_AllSubdivisionPts->InsertNextPoint(ConnectionSplineFilterPoints->GetPoint(num-1));
	//}
	ConnectionSplineFilterPoints->Reset();
	
	SplineFilterPoints->Reset();	
	num = m_AllSubdivisionPts->GetNumberOfPoints();
	ConnectionControlPts->Reset();
	ConnectionControlPts->SetNumberOfPoints(3);
	ConnectionControlPts->InsertPoint(0,m_AllSubdivisionPts->GetPoint(num-1));
	ConnectionControlPts->InsertPoint(1,m_AllSubdivisionPts->GetPoint(num-2));
	ControlPts->Reset();

	renWin->Render();
	Multi_Angle = true;	
}

void InitialTemplate::CloseContour()
{
	IsContourClosed = true;
	if(!Multi_Angle)
	{
		DrawContour(ControlPts, 1);			
		m_AllSubdivisionPts->DeepCopy(SplineFilterPoints);
		SaveCurveCommand->sign=false;
	}
	else
	{
		if(SaveCurveCommand->sign)
		{
			SaveCurrentCurve();
		}

		int num = m_AllSubdivisionPts->GetNumberOfPoints();
		ConnectionControlPts->Reset();
		ConnectionControlPts->SetNumberOfPoints(4);
		double x[3], y[3], z[3], w[3]; 
		m_AllSubdivisionPts->GetPoint(0,x);
		m_AllSubdivisionPts->GetPoint(1,y);
		m_AllSubdivisionPts->GetPoint(num-1,z);
		m_AllSubdivisionPts->GetPoint(num-2,w);
		ExtensionPt(x, y);
		ExtensionPt(z, w);
		ConnectionControlPts->InsertPoint(0, z);
		ConnectionControlPts->InsertPoint(1,w);
		ConnectionControlPts->InsertPoint(2, y);
		ConnectionControlPts->InsertPoint(3, x);
		DrawConnectionContour(ConnectionControlPts);
		num = ConnectionSplineFilterPoints->GetNumberOfPoints();
		for(int i=1; i<num-1; i++)
		{
			m_AllSubdivisionPts->InsertNextPoint(ConnectionSplineFilterPoints->GetPoint(i));
		}
	}
}

void InitialTemplate::OnUndoPoint() 
{
	int num = AllControlPts->GetNumberOfPoints();
	if(num)
	{
		if(IsContourClosed)
		{
			IsContourClosed = false;
			SaveCurveCommand->sign=true;
			if(Multi_Angle)
			{
				ControlPts->Reset();
				ConnectionSubdivisionPtsMapper->RemoveAllInputs();
				ren->RemoveActor(pConnectionContourObject->contourActor);
				renWin->Render();		
				int num = ConnectionSplineFilterPoints->GetNumberOfPoints();
				int Num = m_AllSubdivisionPts->GetNumberOfPoints();
				vtkSmartPointer<vtkPoints> TemPts = vtkSmartPointer<vtkPoints>::New();	
				TemPts->SetNumberOfPoints(Num-(num-2));
				for(int i = 0; i<Num-(num-2); i++)
				{
					TemPts->InsertPoint(i,m_AllSubdivisionPts->GetPoint(i));
				}
				m_AllSubdivisionPts->Reset();
				m_AllSubdivisionPts->DeepCopy(TemPts);

				IsFirstPt = true;
				SaveCurveCommand->sign = false;
				
				num = m_AllSubdivisionPts->GetNumberOfPoints();
				ConnectionControlPts->Reset();
				ConnectionControlPts->SetNumberOfPoints(3);
				ConnectionControlPts->InsertPoint(0,m_AllSubdivisionPts->GetPoint(num-1));
				ConnectionControlPts->InsertPoint(1,m_AllSubdivisionPts->GetPoint(num-2));	
			}
			else
			{
				DrawContour(ControlPts, 0);
				m_AllSubdivisionPts->DeepCopy(SplineFilterPoints);
			}
			
		}
		else
		{
			if(IsFirstUserPt==1)
			{
				//Remove actor
				ren->RemoveActor(FirstUserPtActor);
				renWin->Render();	
				//Delete point	
				ControlPts->Reset();
				AllControlPts->Reset();
				m_AllSubdivisionPts->Reset();
				SplineFilterPoints->Reset();
				ConnectionControlPts->Reset();
				ConnectionSplineFilterPoints->Reset();
				//Reset signs
				IsFirstPt = true;
				Multi_Angle = false;
				IsFirstUserPt = 0;
				SaveCurveCommand->sign = false;
			}
			else
			{
				vtkSmartPointer<vtkPoints> TemPts = vtkSmartPointer<vtkPoints>::New();	
				TemPts->SetNumberOfPoints(num-1);
				int i;
				for(i=0; i<num-1; i++)
				{
					TemPts->InsertPoint(i, AllControlPts->GetPoint(i));
				}
				double y[3];

				TemPts->GetPoint(i-1,y);

				AllControlPts->Reset();
				AllControlPts->DeepCopy(TemPts);
				
				num = ControlPts->GetNumberOfPoints();
				if(num == 0)
				{
					SaveCurveCommand->sign = false;
					IsFirstPt = true;
					if( !QContourObjectList->isEmpty())
					{
						QContourObject* pContourObject=(QContourObject*) QContourObjectList->last();
						vtkPoints *Points = pContourObject->Pts;
						int Num = Points->GetNumberOfPoints();
						double distance = VTK_DOUBLE_MAX;
						double dist2;
						double x[3];
						int closest = -1;
						
						for(i=0; i<Num ;i++)
						{
							Points->GetPoint(i, x);
							dist2 = vtkMath::Distance2BetweenPoints(x, y);
							  if ( dist2 < distance )
								{
									closest = i;
									distance = dist2;
								}
						}
									
						if(closest<5)
						{		
							int n = m_AllSubdivisionPts->GetNumberOfPoints();
							TemPts->Reset();
							for(i=0; i<n-(Num-1); i++)
							{
								TemPts->InsertNextPoint(m_AllSubdivisionPts->GetPoint(i));
							}
							m_AllSubdivisionPts->Reset();
							m_AllSubdivisionPts->DeepCopy(TemPts);

							ren->RemoveActor(pContourObject->contourActor);
							QContourObjectList->removeLast();
							/*pContourObject->contourActor->Delete();
							pContourObject->contourMapper->Delete();
							pContourObject->Pts->Delete();*/
							delete pContourObject;
							renWin->Render();
						}
						else
						{
							int n = m_AllSubdivisionPts->GetNumberOfPoints();
							TemPts->Reset();
							for(i=0; i<n-(Num-closest); i++)
							{
								TemPts->InsertNextPoint(m_AllSubdivisionPts->GetPoint(i));
							}
							m_AllSubdivisionPts->Reset();
							m_AllSubdivisionPts->DeepCopy(TemPts);
							vtkSmartPointer<vtkPolyData>polyData = vtkSmartPointer<vtkPolyData>::New();
							vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
							lines->InsertNextCell(closest);
							TemPts->Reset();
							for(i=0; i<closest; i++)
							{
								TemPts->InsertNextPoint(Points->GetPoint(i));			
								lines->InsertCellPoint(i);
							}
							Points->Reset();
							Points->DeepCopy(TemPts);

							polyData->SetPoints(TemPts);
							polyData->SetLines(lines);

							vtkSmartPointer<vtkTubeFilter> tubes = vtkSmartPointer<vtkTubeFilter>::New();
							tubes->SetInput(polyData);
							tubes->SetNumberOfSides(8);
							tubes->SetRadius(tubeRadius);
							tubes->CappingOn();
							tubes->Update();

							vtkPolyDataMapper *Mapper = pContourObject->contourMapper;
							Mapper->RemoveAllInputs();
							Mapper->SetInputConnection(tubes->GetOutputPort());
							renWin->Render();

						}
						if(AllControlPts->GetNumberOfPoints()>=2)
						{
							ConnectionControlPts->SetNumberOfPoints(3);
							num = m_AllSubdivisionPts->GetNumberOfPoints();
							ConnectionControlPts->InsertPoint(0,m_AllSubdivisionPts->GetPoint(num-1));
							ConnectionControlPts->InsertPoint(1,m_AllSubdivisionPts->GetPoint(num-2));
						}
						else
						{
							//Only the first user placed point left					
							ConnectionControlPts->Reset();
							IsFirstUserPt = 1;
							SaveCurveCommand->sign = false;
							Multi_Angle = false;
							IsFirstPt = false;
							//Draw the first point
							double *fTipPoint = AllControlPts->GetPoint(0);
							ControlPts->InsertNextPoint(fTipPoint);
							vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
							sphere->SetCenter(fTipPoint[0], fTipPoint[1], fTipPoint[2]);
							sphere->SetRadius(tubeRadius);
							sphere->SetPhiResolution(100);
							sphere->SetThetaResolution(12);

							vtkSmartPointer<vtkPolyDataMapper> SphereMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
							SphereMapper->SetInputConnection(sphere->GetOutputPort());
							FirstUserPtActor->GetProperty()->SetColor(0,1,0);
							FirstUserPtActor->SetMapper(SphereMapper);
							ren->AddActor(FirstUserPtActor);
							renWin->Render();
						}
						
					}
					else
					{
						QMessageBox::information(0,"Warning","Current points can't be deleted!");
					}

				}
				else if(num == 1)
				{
					ControlPts->Reset();
					ConnectionSubdivisionPtsMapper->RemoveAllInputs();
					ren->RemoveActor(pConnectionContourObject->contourActor);
					renWin->Render();		
					/*vtkActor *Actor = pConnectionContourObject->contourActor; 
					Actor->Delete();*/
					IsFirstPt = true;
					SaveCurveCommand->sign = false;
					
					num = m_AllSubdivisionPts->GetNumberOfPoints();
					ConnectionControlPts->Reset();
					ConnectionControlPts->SetNumberOfPoints(3);
					ConnectionControlPts->InsertPoint(0,m_AllSubdivisionPts->GetPoint(num-1));
					ConnectionControlPts->InsertPoint(1,m_AllSubdivisionPts->GetPoint(num-2));		
				}
				else
				{		
					vtkSmartPointer<vtkPoints> TemporaryPts = vtkSmartPointer<vtkPoints>::New();
					for(int i=0; i<num-1; i++)
					{
						TemporaryPts->InsertNextPoint(ControlPts->GetPoint(i));
					}
					ControlPts->Reset();
					ControlPts->DeepCopy(TemporaryPts);
					int n = ControlPts->GetNumberOfPoints();
					if(n>1)
					{
						DrawContour(ControlPts, 0);	
					}
					else
					{
						SubdivisionPtsMapper->RemoveAllInputs();
						ren->RemoveActor(pContourObject->contourActor);
						renWin->Render();	
						/*vtkActor *Actor = pContourObject->contourActor; 
						Actor->Delete();*/
						if(Multi_Angle)
						{
							vtkSmartPointer<vtkPoints> TemporaryPts = vtkSmartPointer<vtkPoints>::New();
							for(int i=0; i<3; i++)
							{
								TemporaryPts->InsertNextPoint(ConnectionControlPts->GetPoint(i));
							}
							ConnectionControlPts->Reset();
							ConnectionControlPts->DeepCopy(TemporaryPts);
							DrawConnectionContour(ConnectionControlPts);
							SplineFilterPoints->Reset();

						}//		if(Multi_Angle) ends	
						else
						{
							//Only the first user placed point left
							ConnectionControlPts->Reset();
							IsFirstUserPt = 1;
							SaveCurveCommand->sign = false;
							Multi_Angle = false;
							IsFirstPt = false;
							//Draw the first point
							double *fTipPoint = AllControlPts->GetPoint(0);
							vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
							sphere->SetCenter(fTipPoint[0], fTipPoint[1], fTipPoint[2]);
							sphere->SetRadius(tubeRadius);
							sphere->SetPhiResolution(100);
							sphere->SetThetaResolution(12);

							vtkSmartPointer<vtkPolyDataMapper> SphereMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
							SphereMapper->SetInputConnection(sphere->GetOutputPort());
							FirstUserPtActor->GetProperty()->SetColor(0,1,0);
							FirstUserPtActor->SetMapper(SphereMapper);
							ren->AddActor(FirstUserPtActor);
							renWin->Render();
						}
					}//else(n==1) ends
				}//else (num>=2) ends
			}
		}
	}
	else
	{
		QMessageBox::information(0,QObject::tr("Warning"),QObject::tr("No points to delete!"));
    }
}

void InitialTemplate::DrawContour(vtkPoints *Pts, int Mode)
{      	    
	SubdivisionPtsMapper->RemoveAllInputs();
	vtkSmartPointer<vtkPoints>points = vtkSmartPointer<vtkPoints>::New();
	if(Mode)
	{
		GenerateContourWidget(Pts,Mode);
		points = m_rep->GetContourRepresentationAsPolyData()->GetPoints();
	}
	else
	{
		points->DeepCopy(Pts);
	}

	int count = points->GetNumberOfPoints();
	if(count>=3)
	{
		ren->RemoveActor(pContourObject->contourActor);
		renWin->Render();	

	}

	vtkSmartPointer<vtkPolyData>polyData = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(count);
	for (int i = 0; i<count; ++i)
	{
		lines->InsertCellPoint(i);
	}
	polyData->SetPoints(points);
	polyData->SetLines(lines);

	vtkSmartPointer<vtkCardinalSpline>spline = vtkSmartPointer<vtkCardinalSpline>::New();
	vtkSmartPointer<vtkSplineFilter>splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInput(polyData);
	splineFilter->SetNumberOfSubdivisions(20*(Pts->GetNumberOfPoints()-1)); 
	splineFilter->SetSpline(spline);
	splineFilter->Update();

	SplineFilterPoints->Reset();
	SplineFilterPoints = splineFilter->GetOutput()->GetPoints();
	count = SplineFilterPoints->GetNumberOfPoints();

	polyData->Reset();
	lines->Reset();
	if(Mode)
	{
		lines->InsertNextCell(count+1);
	}
	else
	{
		lines->InsertNextCell(count);
	}
	double ResultPt[3];
	double WorldPt[3];
	int i=0;
	double color[3] = {0,0,1};

	for(; i<count; i++)
	{
		SplineFilterPoints->GetPoint(i,WorldPt);
		ConvertPt(WorldPt,ResultPt);
		SplineFilterPoints->InsertPoint(i,ResultPt);
		lines->InsertCellPoint(i);			
	}
	if(Mode)
	{
		lines->InsertCellPoint(0);	
	}

	polyData->SetPoints(SplineFilterPoints);
	polyData->SetLines(lines);

	vtkSmartPointer<vtkTubeFilter> tubes = vtkSmartPointer<vtkTubeFilter>::New();
	tubes->SetInput(polyData);
	tubes->SetNumberOfSides(8);
	tubes->SetRadius(tubeRadius);
	tubes->CappingOn();

	pContourObject->contourMapper = SubdivisionPtsMapper;
	SubdivisionPtsMapper->SetInputConnection(tubes->GetOutputPort());
	pContourObject->contourActor->GetProperty()->SetColor(color[0],color[1],color[2]);
	pContourObject->contourActor->SetMapper(SubdivisionPtsMapper);
	ren->AddActor(pContourObject->contourActor);
	renWin->Render();
}

void InitialTemplate::DrawConnectionContour(vtkPoints *Pts)
{
	vtkSmartPointer<vtkPoints>points = vtkSmartPointer<vtkPoints>::New();
	points->DeepCopy(Pts);

	int count = points->GetNumberOfPoints();
	if(count == 4)
	{
		ren->RemoveActor(pConnectionContourObject->contourActor);
		renWin->Render();		
	}

	vtkSmartPointer<vtkPolyData>polyData = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(count);
	for (int i = 0; i<count; ++i)
	{
		lines->InsertCellPoint(i);
	}
	polyData->SetPoints(points);
	polyData->SetLines(lines);

	vtkSmartPointer<vtkCardinalSpline>spline = vtkSmartPointer<vtkCardinalSpline>::New();
	vtkSmartPointer<vtkSplineFilter>splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInput(polyData);
	splineFilter->SetNumberOfSubdivisions(20*(Pts->GetNumberOfPoints()-1)); 
	splineFilter->SetSpline(spline);
	splineFilter->Update();

	ConnectionSplineFilterPoints->Reset();
	ConnectionSplineFilterPoints = splineFilter->GetOutput()->GetPoints();
	count = ConnectionSplineFilterPoints->GetNumberOfPoints();

	polyData->Reset();
	lines->Reset();
	lines->InsertNextCell(count);
	double ResultPt[3];
	double WorldPt[3];
	int i=0;
	for(; i<count; i++)
	{
		ConnectionSplineFilterPoints->GetPoint(i,WorldPt);
		ConvertPt(WorldPt,ResultPt);
		ConnectionSplineFilterPoints->InsertPoint(i,ResultPt);
		lines->InsertCellPoint(i);
	}
	polyData->SetPoints(ConnectionSplineFilterPoints);
	polyData->SetLines(lines);

	vtkSmartPointer<vtkTubeFilter> tubes = vtkSmartPointer<vtkTubeFilter>::New();
	tubes->SetInput(polyData);
	tubes->SetNumberOfSides(8);
	tubes->SetRadius(tubeRadius);
	tubes->CappingOn();

	double color[3] = {0,0,1};

	pConnectionContourObject->contourActor = vtkActor::New();
	pConnectionContourObject->contourMapper = ConnectionSubdivisionPtsMapper;
	ConnectionSubdivisionPtsMapper->SetInputConnection(tubes->GetOutputPort());
	pConnectionContourObject->contourActor->GetProperty()->SetColor(color[0],color[1],color[2]);
	pConnectionContourObject->contourActor->SetMapper(ConnectionSubdivisionPtsMapper);
	ren->AddActor(pConnectionContourObject->contourActor);
	renWin->Render();
}

void InitialTemplate::GenerateContourWidget(vtkPoints*points, int Mode)
{
	//Mode = 0: Open;        Mode = 1:Close
	vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

	int count=points->GetNumberOfPoints();

	if(Mode)
	{
		lines->InsertNextCell(count+1);
		for(int i=0; i<count; i++)
		{
			lines->InsertCellPoint(i);
		}
		lines->InsertCellPoint(0);
	}
	else
	{
		lines->InsertNextCell(count);
		for(int i=0; i<count; i++)
		{
			lines->InsertCellPoint(i);
		}
	}

	pd->SetPoints(points);
	pd->SetLines(lines);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> Style
		= vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor 
		= vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);
	interactor->SetInteractorStyle(Style);

	vtkSmartPointer<vtkContourWidget> contourWidget 
		= vtkSmartPointer<vtkContourWidget>::New();
	contourWidget=vtkContourWidget::New();
	contourWidget->SetInteractor(interactor);

	m_rep = vtkOrientedGlyphContourRepresentation::SafeDownCast
		(contourWidget->GetRepresentation());

	contourWidget->EnabledOn();
	contourWidget->Initialize(pd);

	renWin->SetInteractor(iren);
	renWin->Render();
}

void InitialTemplate::ConvertPt(double *worldPoint,double *result)
{
	vtkSmartPointer<vtkRenderer> renPtr = vtkSmartPointer<vtkRenderer>::New();
	renPtr = ren;
	renPtr->SetWorldPoint(worldPoint[0], worldPoint[1], worldPoint[2],1);
	renPtr->WorldToDisplay();

	double screenPoint[3];
	renPtr->GetDisplayPoint(screenPoint);
	picker->Pick(screenPoint,renPtr);

	double *tmp;
	tmp = picker->GetPickPosition();
	result[0] = tmp[0];
	result[1] = tmp[1];
	result[2] = tmp[2];
}

void InitialTemplate::ExtensionPt(double x[3], double y[3])
{
	for(int i=0; i<3; i++)
	{
		y[i]=x[i]-y[i];
	}
	double length=sqrt(pow(y[0],2) + pow(y[1],2)+ pow(y[2],2));
	for(int k=0;k<3;k++)
	{
		y[k]=y[k]/length;
	}
	for(int i=0; i<3; i++)
	{
		y[i]=x[i]+0.5*y[i];
	}
}

void InitialTemplate::ClearContours()
{
	int count = this->QContourObjectList->count();
	for (int i = 0; i < count; ++i)
	{
		ren->RemoveActor(QContourObjectList->at(i)->contourActor);
	}
}

void InitialTemplate::UndoPunching()
{
	m_outSurf->DeepCopy(m_outSurface);
	m_innSurf->DeepCopy(m_innSurface);
	m_mesh->DeepCopy(m_wall);
}