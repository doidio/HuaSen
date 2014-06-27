#pragma once

#include <vtkHandleRepresentation.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceRepresentation3D.h>

#include <vtkAnglewidget.h>
#include <vtkAngleRepresentation.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAngleRepresentation3D.h>

#include <vtkSmartPointer.h>
#include <QCheckBox>
#include <vtkProperty.h>
#include <vtkActor.h>

class MeasureObject
{
public:
	MeasureObject(void);
	virtual ~MeasureObject(void);

	int num;
	bool is2D, isAngle, isAxi, isCor, isSag;	
	double nSlice;	double disValue; double angValue;
	//end points
	double p1[3];	double p2[3];	double p3[3];

	//3D distance measure
	vtkSmartPointer<vtkPointHandleRepresentation3D>		disHandleRep3D;
	vtkSmartPointer<vtkDistanceRepresentation3D>		disRep3D;
	vtkSmartPointer<vtkDistanceWidget>					disWidget3D;
	
	//3D angle measure
	vtkSmartPointer<vtkPointHandleRepresentation3D>		angHandleRep3D;
	vtkSmartPointer<vtkAngleRepresentation3D>			angRep3D;
	vtkSmartPointer<vtkAngleWidget>						angWidget3D;

	QCheckBox *checkBox;
};
