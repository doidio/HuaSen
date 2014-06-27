#ifndef QTSIGNAL_H
#define QTSIGNAL_H

#include <QObject>

class QtSignal : public QObject
{
	Q_OBJECT

public:
	QtSignal();
	~QtSignal();

	void emitMeaWidgetMove();
	void emitConnect();

signals:
	void MeaWidgetMove();
	void Connect();

private:
	
};

#endif // QTSIGNAL_H
