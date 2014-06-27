#pragma once

#include <vtkCommand.h>
#include "QtSignal.h"
#include <QMessageBox>

class MoveCommand : public vtkCommand 
{
public:
	MoveCommand(){}
	virtual ~MoveCommand(){}

	QtSignal *qSignal;

	static MoveCommand *New() 
	{ 
		return new MoveCommand; 
	}

	virtual void Execute(vtkObject *, unsigned long, void*)
    {
		qSignal->emitMeaWidgetMove();
	}
};