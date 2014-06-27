#ifndef QSPHEREACTOROBJECT_H
#define QSPHEREACTOROBJECT_H

#include <QApplication>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>

class QSphereActorObject :
	public QObject
{
public:
	vtkActor *sphereActor;
	vtkSphereSource *sphere;
	vtkPolyDataMapper *sphereMapper;
	double m_fPosition[3];

public:
	QSphereActorObject(){}
public:
	~QSphereActorObject(){}
};

#endif