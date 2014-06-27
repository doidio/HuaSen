#include "QtSignal.h"

QtSignal::QtSignal()
	: QObject()
{

}

QtSignal::~QtSignal()
{

}

void QtSignal::emitMeaWidgetMove()
{
	emit MeaWidgetMove();
}

void QtSignal::emitConnect()
{
	emit Connect();
}