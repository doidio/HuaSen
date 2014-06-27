#ifndef QSAVECURVEOBJECT_H
#define QSAVECURVEOBJECT_H

//#include <QtGui>
//#include <qobject.h>
#include <QApplication>


class QSaveCurveObject : public QObject
{
	Q_OBJECT

public:
	QSaveCurveObject();
	~QSaveCurveObject();
	void SaveCurve();


signals:
	void saveCurrentCurve();

};

#endif 

