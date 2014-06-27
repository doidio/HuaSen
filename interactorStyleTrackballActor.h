#ifndef INTERACTORSTYLETRACKBALLACTOR_H
#define INTERACTORSTYLETRACKBALLACTOR_H

#include "vtkInteractorStyleTrackballActor.h"

class interactorStyleTrackballActor : public vtkInteractorStyleTrackballActor
{

public:
	static interactorStyleTrackballActor *New()
	{return new interactorStyleTrackballActor;}

	interactorStyleTrackballActor();
	~interactorStyleTrackballActor();

	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();

private:
	
};

#endif // INTERACTORSTYLETRACKBALLACTOR_H
