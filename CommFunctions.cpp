
#include <vtkMath.h>
#include <vtkWorldPointPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkPolyLine.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVectorText.h>
#include "QLineActorObject.h"
#include "QSphereActorObject.h"

//extern bool m_bMandiblePlan;

typedef struct Position3dStruct
{
    double   x;
    double   y;
    double   z;
} Position3d;

const double pi=3.14159265358979f;

void GetParallelPoint( double a1,double b1,double c1,
									double a2,double b2,double c2,
									double a3,double b3,double c3,
									double fLength,
									Position3d* fPoint)
{
	double m1,m2,m3,k;
	m1=a2-a1;
	m2=b2-b1;
	m3=c2-c1;
	k=fLength/sqrt((pow(m1,2)+pow(m2,2)+pow(m3,2)));
	fPoint->x=m1*k+a3;
	fPoint->y=m2*k+b3;
	fPoint->z=m3*k+c3;
}

double GetIncludedAngle(Position3d* fStartPoint1,Position3d* fEndPoint1,
Position3d* fStartPoint2,Position3d* fEndPoint2)
{
    Position3d Vp,Vq;
    double angle,LofVp,LofVq;//依次为偏离角度，Vp长,Vq长

    Vp.x=(fEndPoint1->x-fStartPoint1->x);
    Vp.y=(fEndPoint1->y-fStartPoint1->y);
    Vp.z=(fEndPoint1->z-fStartPoint1->z);

    Vq.x=fEndPoint2->x-fStartPoint2->x;
    Vq.y=fEndPoint2->y-fStartPoint2->y;
    Vq.z=fEndPoint2->z-fStartPoint2->z;

    LofVp=sqrt(Vp.x*Vp.x+Vp.y*Vp.y+Vp.z*Vp.z);
    LofVq=sqrt(Vq.x*Vq.x+Vq.y*Vq.y+Vq.z*Vq.z);
    Vp.x/=LofVp;
    Vp.y/=LofVp;
    Vp.z/=LofVp;

    Vq.x/=LofVq;
    Vq.y/=LofVq;
    Vq.z/=LofVq;

    angle=acos(sqrt((Vp.x*Vq.x+Vp.y*Vq.y+Vp.z*Vq.z)*(Vp.x*Vq.x+Vp.y*Vq.y+Vp.z*Vq.z)
		  /(Vp.x*Vp.x+Vp.y*Vp.y+Vp.z*Vp.z)
		   *(Vq.x*Vq.x+Vq.y*Vq.y+Vq.z*Vq.z)));
    angle*=180/pi;
	return angle;
}

bool JudgeDirection(double *P1,double *P2,double *Pn,double *Pn1)
{
	bool bClockWise=false;
	double fVector[3] ; 
	double fVector1[3] ;
	double fVector2[3] ;
	fVector[0]=P2[0]-P1[0];
	fVector[1]=P2[1]-P1[1];
	fVector[2]=0;

	fVector1[0]=Pn1[0]-Pn[0];
	fVector1[1]=Pn1[1]-Pn[1];
	fVector1[2]=0;

	vtkMath *math=vtkMath::New();
	math->Cross(fVector,fVector1,fVector2); 
	if(fVector2[2]<0)
		bClockWise=true;
	else
		bClockWise=false;

	math->Delete();
	return bClockWise;
}

void DrawMeasureLine(bool b3DView,vtkWorldPointPicker *wpicker,QList<QSphereActorObject *> *pSphereActorLinePointList,
					 QList<QLineActorObject *> *pActorLineList,vtkRenderer *ren,vtkRenderWindow *renWin) 
{
	double *fTipPoint=wpicker->GetPickPosition();
	if(fTipPoint!=NULL)
	{
		QSphereActorObject* pActorObject=new QSphereActorObject();

		pActorObject->sphereActor=vtkActor::New();
		pActorObject->sphere=vtkSphereSource::New();
		pActorObject->sphereMapper=vtkPolyDataMapper::New();

		vtkActor *sphereActor=pActorObject->sphereActor;
		vtkSphereSource *sphere=pActorObject->sphere;
		vtkPolyDataMapper *sphereMapper=pActorObject->sphereMapper;

		pActorObject->m_fPosition[0]=fTipPoint[0];
		pActorObject->m_fPosition[1]=fTipPoint[1];
		pActorObject->m_fPosition[2]=fTipPoint[2];

		sphere->SetRadius(0.5);
		sphere->SetThetaResolution(12);
		sphere->SetPhiResolution(100);
		sphereMapper->SetInput(sphere->GetOutput());
		
		sphereActor->SetMapper(sphereMapper);
		sphereActor->SetPosition(fTipPoint);
		sphereActor->GetProperty()->SetColor(255,0,255);//hot_pink
		sphereActor->GetProperty()->SetSpecularColor( 1 ,1 ,1);
		sphereActor->GetProperty()->SetSpecular( 0.3);
		sphereActor->GetProperty()->SetSpecularPower( 20);
		sphereActor->GetProperty()->SetAmbient( 0.2);
		sphereActor->GetProperty()->SetDiffuse( 0.8);

		pSphereActorLinePointList->append(pActorObject);
		ren->AddActor(sphereActor);

		int nRemainder=pSphereActorLinePointList->count()%2;
		if(nRemainder==0)
		{
			QLineActorObject* pLineActorObject=new QLineActorObject();

			vtkPoints *LinepolyLinePoints=vtkPoints::New();
			vtkPolyLine *LinePolyLine=vtkPolyLine::New();
			vtkUnstructuredGrid *LinePolyLineGrid=vtkUnstructuredGrid::New();
			pLineActorObject->LinePolyLineMapper=vtkDataSetMapper::New() ;
			pLineActorObject->LinePolyLineActor=vtkActor::New() ;

			LinepolyLinePoints->SetNumberOfPoints(2);
			QSphereActorObject *pActorObject,*pActorObject1;
			pActorObject= pSphereActorLinePointList->at(pSphereActorLinePointList->count()-2) ;
			LinepolyLinePoints->InsertPoint(0, pActorObject->m_fPosition[0],pActorObject->m_fPosition[1],pActorObject->m_fPosition[2]);
			pActorObject1=pSphereActorLinePointList->last() ;
			LinepolyLinePoints->InsertPoint(1, pActorObject1->m_fPosition[0],pActorObject1->m_fPosition[1],pActorObject1->m_fPosition[2]);

			LinePolyLine->GetPointIds()->SetNumberOfIds(2);
				
			for ( int i = 0; i < 2; i++ ) 
			{
				LinePolyLine->GetPointIds()->SetId(i, i);
			}

			LinePolyLineGrid->Allocate(1, 1);
			LinePolyLineGrid->InsertNextCell(LinePolyLine->GetCellType(), LinePolyLine->GetPointIds());
			LinePolyLineGrid->SetPoints(LinepolyLinePoints);

			pLineActorObject->LinePolyLineMapper->SetInput(LinePolyLineGrid);		
			pLineActorObject->LinePolyLineActor->SetMapper(pLineActorObject->LinePolyLineMapper);
			pLineActorObject->LinePolyLineActor->GetProperty()->SetDiffuseColor(180.0/255.0,1.0f,1.0f);
			if(!b3DView) pLineActorObject->LinePolyLineActor->AddPosition(0,0,0.1f);
			ren->AddActor(pLineActorObject->LinePolyLineActor);

			LinepolyLinePoints->Delete();
			LinePolyLine->Delete();
			LinePolyLineGrid->Delete();

			pActorLineList->append(pLineActorObject);

			double X12,Y12,Z12,fLine;

			X12=pActorObject1->m_fPosition[0]-pActorObject->m_fPosition[0];
			Y12=pActorObject1->m_fPosition[1]-pActorObject->m_fPosition[1];
			Z12=pActorObject1->m_fPosition[2]-pActorObject->m_fPosition[2];	

			fLine=sqrt(pow(X12,2)+pow(Y12,2)+pow(Z12,2));

			QString sLine;
			sLine.sprintf("%.2f mm", fLine);

			vtkVectorText *Label=vtkVectorText::New();
			Label->SetText(sLine.toAscii () );

			vtkPolyDataMapper *labelMapper=vtkPolyDataMapper::New();
			labelMapper->SetInput(Label->GetOutput());

			pLineActorObject->labelActor=vtkFollower::New();
			pLineActorObject->labelActor->SetMapper(labelMapper);
			pLineActorObject->labelActor->SetPosition(pActorObject1->m_fPosition[0],pActorObject1->m_fPosition[1],pActorObject1->m_fPosition[2]);

			pLineActorObject->labelActor->SetScale(3.0,3.0,3.0);
			pLineActorObject->labelActor->GetProperty()->SetColor(180.0/255.0,1.0f,1.0f);

			if(b3DView) 
			{
				/*if(m_bMandiblePlan)
					pLineActorObject->labelActor->RotateX(90);
				else*/
					pLineActorObject->labelActor->RotateX(-90);
			}
			else
				pLineActorObject->labelActor->AddPosition(0,0,0.1f);

			ren->AddActor(pLineActorObject->labelActor);

			labelMapper->Delete();
			Label->Delete();			
		}
		renWin->Render();
	}
}


void DrawMeasureAngle(bool b3DView,vtkWorldPointPicker *wpicker,QList<QSphereActorObject *> *pSphereActorLinePointList,
					 QList<QLineActorObject *> *pActorLineList,vtkRenderer *ren,vtkRenderWindow *renWin) 
{
	double *fTipPoint=wpicker->GetPickPosition();
	if(fTipPoint!=NULL)
	{
		QSphereActorObject* pActorObject=new QSphereActorObject();

		pActorObject->sphereActor=vtkActor::New();
		pActorObject->sphere=vtkSphereSource::New();
		pActorObject->sphereMapper=vtkPolyDataMapper::New();

		vtkActor *sphereActor=pActorObject->sphereActor;
		vtkSphereSource *sphere=pActorObject->sphere;
		vtkPolyDataMapper *sphereMapper=pActorObject->sphereMapper;

		pActorObject->m_fPosition[0]=fTipPoint[0];
		pActorObject->m_fPosition[1]=fTipPoint[1];
		pActorObject->m_fPosition[2]=fTipPoint[2];

		sphere->SetRadius(0.5);
		sphere->SetThetaResolution(12);
		sphere->SetPhiResolution(100);
		sphereMapper->SetInput(sphere->GetOutput());
		
		sphereActor->SetMapper(sphereMapper);
		sphereActor->SetPosition(fTipPoint);
		sphereActor->GetProperty()->SetColor(255,0,255);//hot_pink
		sphereActor->GetProperty()->SetSpecularColor( 1 ,1 ,1);
		sphereActor->GetProperty()->SetSpecular( 0.3);
		sphereActor->GetProperty()->SetSpecularPower( 20);
		sphereActor->GetProperty()->SetAmbient( 0.2);
		sphereActor->GetProperty()->SetDiffuse( 0.8);

		pSphereActorLinePointList->append(pActorObject);
		ren->AddActor(sphereActor);

		int nRemainder=pSphereActorLinePointList->count()%3;
		if(nRemainder==2)
		{
			QLineActorObject* pLineActorObject=new QLineActorObject();

			vtkPoints *LinepolyLinePoints=vtkPoints::New();
			vtkPolyLine *LinePolyLine=vtkPolyLine::New();
			vtkUnstructuredGrid *LinePolyLineGrid=vtkUnstructuredGrid::New();
			pLineActorObject->LinePolyLineMapper=vtkDataSetMapper::New() ;
			pLineActorObject->LinePolyLineActor=vtkActor::New() ;

			LinepolyLinePoints->SetNumberOfPoints(2);
			QSphereActorObject *pActorObject,*pActorObject1;
			pActorObject= pSphereActorLinePointList->at(pSphereActorLinePointList->count()-2) ;
			LinepolyLinePoints->InsertPoint(0, pActorObject->m_fPosition[0],pActorObject->m_fPosition[1],pActorObject->m_fPosition[2]);
			pActorObject1=pSphereActorLinePointList->last() ;
			LinepolyLinePoints->InsertPoint(1, pActorObject1->m_fPosition[0],pActorObject1->m_fPosition[1],pActorObject1->m_fPosition[2]);

			LinePolyLine->GetPointIds()->SetNumberOfIds(2);
				
			for ( int i = 0; i < 2; i++ ) 
			{
				LinePolyLine->GetPointIds()->SetId(i, i);
			}

			LinePolyLineGrid->Allocate(1, 1);
			LinePolyLineGrid->InsertNextCell(LinePolyLine->GetCellType(), LinePolyLine->GetPointIds());
			LinePolyLineGrid->SetPoints(LinepolyLinePoints);

			pLineActorObject->LinePolyLineMapper->SetInput(LinePolyLineGrid);		
			pLineActorObject->LinePolyLineActor->SetMapper(pLineActorObject->LinePolyLineMapper);
			pLineActorObject->LinePolyLineActor->GetProperty()->SetDiffuseColor(180.0/255.0,1.0f,1.0f);
			if(!b3DView) pLineActorObject->LinePolyLineActor->AddPosition(0,0,0.1f);
			ren->AddActor(pLineActorObject->LinePolyLineActor);

			LinepolyLinePoints->Delete();
			LinePolyLine->Delete();
			LinePolyLineGrid->Delete();

			pActorLineList->append(pLineActorObject);
		}
		else if(nRemainder==0)
		{
			QLineActorObject* pLineActorObject=pActorLineList->last();

			vtkPoints *LinepolyLinePoints=vtkPoints::New();
			vtkPolyLine *LinePolyLine=vtkPolyLine::New();
			vtkUnstructuredGrid *LinePolyLineGrid=vtkUnstructuredGrid::New();				

			LinepolyLinePoints->SetNumberOfPoints(3);
			QSphereActorObject *pActorObject1,*pActorObject2,*pActorObject3;
			pActorObject3= pSphereActorLinePointList->at(pSphereActorLinePointList->count()-3) ;
			LinepolyLinePoints->InsertPoint(0, pActorObject3->m_fPosition[0],pActorObject3->m_fPosition[1],pActorObject3->m_fPosition[2]);
			pActorObject2= pSphereActorLinePointList->at(pSphereActorLinePointList->count()-2) ;
			LinepolyLinePoints->InsertPoint(1, pActorObject2->m_fPosition[0],pActorObject2->m_fPosition[1],pActorObject2->m_fPosition[2]);
			pActorObject1=pSphereActorLinePointList->last() ;
			LinepolyLinePoints->InsertPoint(2, pActorObject1->m_fPosition[0],pActorObject1->m_fPosition[1],pActorObject1->m_fPosition[2]);

			LinePolyLine->GetPointIds()->SetNumberOfIds(3);
				
			for ( int i = 0; i < 3; i++ ) 
			{
				LinePolyLine->GetPointIds()->SetId(i, i);
			}

			LinePolyLineGrid->Allocate(1, 1);
			LinePolyLineGrid->InsertNextCell(LinePolyLine->GetCellType(), LinePolyLine->GetPointIds());
			LinePolyLineGrid->SetPoints(LinepolyLinePoints);

			pLineActorObject->LinePolyLineMapper->SetInput(LinePolyLineGrid);		
			pLineActorObject->LinePolyLineActor->SetMapper(pLineActorObject->LinePolyLineMapper);
			pLineActorObject->LinePolyLineActor->GetProperty()->SetDiffuseColor(180.0/255.0,1.0f,1.0f);
			if(!b3DView) pLineActorObject->LinePolyLineActor->AddPosition(0,0,0.1f);
			//ren->AddActor(pLineActorObject->LinePolyLineActor);

			LinepolyLinePoints->Delete();
			LinePolyLine->Delete();
			LinePolyLineGrid->Delete();				

			double a,b,c,X12,Y12,Z12,X23,Y23,Z23,X31,Y31,Z31,fAngle;

			X12=pActorObject2->m_fPosition[0]-pActorObject1->m_fPosition[0];
			Y12=pActorObject2->m_fPosition[1]-pActorObject1->m_fPosition[1];
			Z12=pActorObject2->m_fPosition[2]-pActorObject1->m_fPosition[2];

			X23=pActorObject3->m_fPosition[0]-pActorObject2->m_fPosition[0];
			Y23=pActorObject3->m_fPosition[1]-pActorObject2->m_fPosition[1];
			Z23=pActorObject3->m_fPosition[2]-pActorObject2->m_fPosition[2];

			X31=pActorObject1->m_fPosition[0]-pActorObject3->m_fPosition[0];
			Y31=pActorObject1->m_fPosition[1]-pActorObject3->m_fPosition[1];
			Z31=pActorObject1->m_fPosition[2]-pActorObject3->m_fPosition[2];

			a=sqrt(pow(X12,2)+pow(Y12,2)+pow(Z12,2));
			b=sqrt(pow(X23,2)+pow(Y23,2)+pow(Z23,2));
			c=sqrt(pow(X31,2)+pow(Y31,2)+pow(Z31,2));

			fAngle=acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b))*180.0/3.14159265358979f;

			QString sAngle;
			sAngle.sprintf("%.2f D", fAngle);

			vtkVectorText *Label=vtkVectorText::New();
			Label->SetText(sAngle.toAscii () );

			vtkPolyDataMapper *labelMapper=vtkPolyDataMapper::New();
			labelMapper->SetInput(Label->GetOutput());

			pLineActorObject->labelActor=vtkFollower::New();
			pLineActorObject->labelActor->SetMapper(labelMapper);
			pLineActorObject->labelActor->SetPosition(pActorObject1->m_fPosition[0],pActorObject1->m_fPosition[1],pActorObject1->m_fPosition[2]);

			pLineActorObject->labelActor->SetScale(3.0,3.0,3.0);
			pLineActorObject->labelActor->GetProperty()->SetColor(180.0/255.0,1.0f,1.0f);

			if(b3DView)
			{
				/*if(m_bMandiblePlan)
					pLineActorObject->labelActor->RotateX(90);
				else*/
					pLineActorObject->labelActor->RotateX(-90);
			}
			else
				pLineActorObject->labelActor->AddPosition(0,0,0.1f);

			ren->AddActor(pLineActorObject->labelActor);

			labelMapper->Delete();
			Label->Delete();			
		}
		renWin->Render();
	}
}

bool IsChineseIncluded(QString str)
{
	bool BoolValue=false;

	//char strRead[1000];
	QByteArray strRead=str.toUtf8();

	for (int i =0 ;i<str.length(); i++)
	{	
		//QByteArray  iInt16 =(str.mid(i,1)).toUtf8().toHex();
		//if ((iInt16 >= "0x4E00") && (iInt16 <= "0x9FA5")) 
		if(((unsigned)strRead[i])>0x80)
		{
		  BoolValue = true;
		  break;
		}
		else
		{
		  BoolValue = false;
		}
	}
	//QByteArray  sb=str.toUtf8();
	//if(sb.at(0)<"0")
	//{
	//	BoolValue = true;
	//}
	//else
	//{
	//	BoolValue = false;
	//}

	return BoolValue;
}
