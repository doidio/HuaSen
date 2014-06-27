// vtkAnnotatePick.h: interface for the vtkAnnotatePick class.
//
//////////////////////////////////////////////////////////////////////
#ifndef VTKANNOTATEPICK_H
#define VTKANNOTATEPICK_H

#include "QtSignal.h"

#include <vtkCellPicker.h>
#include <vtkWorldPointPicker.h>
#include <vtkPointPicker.h>
#include <vtkCommand.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>

#include <QApplication>


class vtkAnnotatePick : public vtkCommand 
{
public:
	vtkAnnotatePick(){
		signal = new QtSignal;
	}
	virtual ~vtkAnnotatePick(){}

	vtkTextMapper *textMapper;
	vtkActor2D *textActor; 
	vtkImageData *ImageData;
	static vtkAnnotatePick *New() 
	{ 
		return new vtkAnnotatePick; 
	}

	QtSignal *signal;

	virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
		signal->emitConnect();
		//vtkPointPicker *picker = reinterpret_cast<vtkPointPicker*>(caller);
		vtkRenderWindowInteractor *iren=reinterpret_cast<vtkRenderWindowInteractor*>(caller);
		vtkWorldPointPicker *picker = (vtkWorldPointPicker*) iren->GetPicker();

		textMapper->GetTextProperty()->SetFontFamilyToArial();
		textMapper->GetTextProperty()->SetFontSize(10);
		textMapper->GetTextProperty()->BoldOn();
		textMapper->GetTextProperty()->ShadowOff();
		textMapper->GetTextProperty()->ItalicOff();
		textMapper->GetTextProperty()->SetVerticalJustificationToBottom();
		textMapper->GetTextProperty()->SetColor(1,0,0);

		textActor->VisibilityOff();
		textActor->SetMapper(textMapper);

		double selectionPt[3];
		selectionPt[0] = iren->GetEventPosition()[0];
		selectionPt[1] = iren->GetEventPosition()[1];
		selectionPt[2] = 0.0;

//     vtkWorldPointPicker* picker = vtkWorldPointPicker::New();
//     picker->Pick(X, Y, 0.0, renderer);
		
		picker->Pick(selectionPt, iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

		double *pPickPoint=picker->GetPickPosition();

		QString str;
		str.sprintf("%.1f ,%.1f, %.1f",  pPickPoint[0],pPickPoint[1],pPickPoint[2]);

		textMapper->SetInput(str.toAscii ());
	
		//double *pSelectionPoint=picker->GetSelectionPoint();

		//textActor->SetPosition(picker->GetSelectionPoint()[0],picker->GetSelectionPoint()[1]);
		textActor->SetPosition(selectionPt[0],selectionPt[1]);
		iren->GetRenderWindow()->Render ();

		signal->emitConnect();
	}
};

#endif