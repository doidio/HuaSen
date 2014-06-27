// ActorObject.h: interface for the CActorObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ACTOROBJECT_H
#define ACTOROBJECT_H

#include "InitialTemplate.h"

#include <QApplication>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
//#include <vtkLODActor.h>
//#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <ctkColorPickerButton.h>
#include <QSpinBox>
#include <ctkPopupWidget.h>
#include <QSlider>
#include <ctkPopupWidget.h>
#include <ctkBasePopupWidget.h>

class QActorObject : public QObject  
{
	
public:
	//vtkSmartPointer<vtkActor> segmentOverlayActor;
	//vtkSmartPointer<vtkImageDilateErode3D> DilateErode3D;
	vtkSmartPointer<vtkPolyDataMapper> isoMapper;
	vtkSmartPointer<vtkActor> isoActor;

	QCheckBox *checkBox;
	ctkColorPickerButton *colorPickerButton;
	QSpinBox *opacSpin;
	ctkPopupWidget *opacPopup;
	QSlider* popupSlider;

	InitialTemplate* templateData;

	bool isTem;

public:
	QActorObject()
	{
		isTem = false;
	}
	virtual ~QActorObject() 
	{
		if (templateData)
		{
			delete templateData;
		}
	}

};

#endif 
