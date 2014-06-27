#ifndef QLINEACTOROBJECT_H
#define QLINEACTOROBJECT_H

#include <vtkActor.h>

#include <vtkDataSetMapper.h>
#include <vtkFollower.h>
#include <vtkActor.h>

#include <QApplication>

class QLineActorObject :public QObject
{
	Q_OBJECT

public:
	vtkDataSetMapper* LinePolyLineMapper;
	vtkActor* LinePolyLineActor;
	vtkFollower *labelActor;
	
public:
	QLineActorObject(){}
	~QLineActorObject(){}
};

#endif