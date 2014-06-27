#include "QSaveCurveObject.h"


QSaveCurveObject::QSaveCurveObject(){}
QSaveCurveObject::~QSaveCurveObject(){}
void QSaveCurveObject::SaveCurve()
{
	emit saveCurrentCurve();
}
