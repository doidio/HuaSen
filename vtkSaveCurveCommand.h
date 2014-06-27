#ifndef VTKSAVECURVECOMMAND_H
#define VTKSAVECURVECOMMAND_H

#include <vtkCommand.h>
#include <QApplication>

#include "QSaveCurveObject.h"


class vtkSaveCurveCommand : public vtkCommand
{
public :
	QSaveCurveObject *qSaveCurve;
	bool sign;

public:
	vtkSaveCurveCommand()
	{
		qSaveCurve = new QSaveCurveObject;
		sign = false;
	}
	~vtkSaveCurveCommand(){}

	static vtkSaveCurveCommand *New() 
	{ 
		return new vtkSaveCurveCommand; 
	}

	virtual void Execute(vtkObject *, unsigned long, void*)
	{
		//QMessageBox::information(0, "","!!!!!!!!!!!!!");
		if(sign)
		{
			qSaveCurve->SaveCurve();
		}		
	}

};

#endif