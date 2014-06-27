#ifndef QCUSTOMVTKWIDGET_H
#define QCUSTOMVTKWIDGET_H

#include <QtGui>
#include "qvtkwidget.h"
//#include <QMetaObject.h>


class QCustomVTKWidget :
	public QVTKWidget
{
	Q_OBJECT

public:
	QCustomVTKWidget(QWidget *);
public:
	~QCustomVTKWidget(void);

	bool disFlag;
	bool angFlag;
	int dpTimes;

	int value() const { return m_value; }
	virtual void keyPressEvent(QKeyEvent* event)
	{
		QVTKWidget::keyPressEvent(event);
	}

protected:
	void wheelEvent(QWheelEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void resizeEvent(QResizeEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

signals:
	void wheelScrolled();
	void ChgSlice();
	void customResize();
	void fixPoint();
	void EndSelect();
	void DeleteSelect();

	void shift();

	void disMeasure();
	void angMeasure();
	void updateMeasureValue();

	void selctPts();

public:
     int m_value;
	 //int m_nFrameHeight,m_nFrameWidth;


};

#endif 