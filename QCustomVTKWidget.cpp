#include "QCustomVTKWidget.h"

QCustomVTKWidget::QCustomVTKWidget(QWidget *)
{
	m_value=0;

	disFlag = false;
	angFlag = false;
	dpTimes = 0;
}

QCustomVTKWidget::~QCustomVTKWidget(void)
{
}

void QCustomVTKWidget::wheelEvent(QWheelEvent* event)
{
	if(event->delta()>0)
		m_value=1;
	else
		m_value=-1;

	emit wheelScrolled();
}

void QCustomVTKWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QVTKWidget::mouseReleaseEvent(event);

	if(event->button()==Qt::LeftButton)
	{
		emit ChgSlice();
	}


}


void QCustomVTKWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if(event->button()==Qt::LeftButton)
	{
		QKeyEvent *eKeyEvent=new QKeyEvent(QEvent::KeyPress,Qt::Key_P,Qt::NoModifier,QString(QChar(Qt::Key_P)));		
		QVTKWidget::keyPressEvent(eKeyEvent);
		delete eKeyEvent;

		if(disFlag)
		{
			++dpTimes;
			emit disMeasure();
		}
		if(angFlag)
		{
			++dpTimes;
			emit angMeasure();
		}
		emit fixPoint();
	}
}

void QCustomVTKWidget::resizeEvent(QResizeEvent *event)
{
	QVTKWidget::resizeEvent(event);
	emit customResize();
}
void QCustomVTKWidget::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key()==Qt::Key_Shift)
	{
		emit shift();
	}

	if(event->key()==Qt::Key_Space)
	{
		emit EndSelect();
	}

	if(event->key()==Qt::Key_Delete)
	{
		emit DeleteSelect();
	}
}