#ifndef QCONTOUROBJECT_H
#define QCONTOUROBJECT_H

#include <QObject>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

class QContourObject : public QObject
{
	Q_OBJECT

public:

	vtkSmartPointer<vtkActor> contourActor;
	vtkSmartPointer<vtkPolyDataMapper> contourMapper;
	vtkSmartPointer<vtkPoints> Pts;

	QContourObject() 
	{
		contourActor = vtkSmartPointer<vtkActor>::New();
		contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Pts = vtkSmartPointer<vtkPoints>::New();
	}
	virtual ~QContourObject() 
	{
		//contourActor->Delete();
		//contourMapper->Delete();
		//Pts->Delete();
	}

private:
	
};

#endif // QCONTOUROBJECT_H
