#include "MeasureObject.h"

MeasureObject::MeasureObject(void)
{
	p1[0]=0;	p1[1]=0;	p1[2]=0;
	p2[0]=0;	p2[1]=0;	p2[2]=0;
	p3[0]=0;	p3[1]=0;	p3[2]=0;
	is2D = false;
	isAngle = false;
	isAxi = false;
	isCor = false;
	isSag = false;
	nSlice = 0;
	disValue = 0;
	angValue = 0;
	//3D distance measure
	disHandleRep3D = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	disRep3D = vtkSmartPointer<vtkDistanceRepresentation3D>::New();
	disWidget3D = vtkSmartPointer<vtkDistanceWidget>::New();
	//3D angle measure
	angHandleRep3D = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	angRep3D = vtkSmartPointer<vtkAngleRepresentation3D>::New();
	angWidget3D = vtkSmartPointer<vtkAngleWidget>::New();

	checkBox = new QCheckBox();

	disRep3D->SetHandleRepresentation(disHandleRep3D);
	disWidget3D->SetRepresentation(disRep3D);
	disRep3D->InstantiateHandleRepresentation();

	angRep3D->SetHandleRepresentation(angHandleRep3D);
	angWidget3D->SetRepresentation(angRep3D);
	angRep3D->InstantiateHandleRepresentation();
}

MeasureObject::~MeasureObject(void)
{
}
