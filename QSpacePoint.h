#ifndef QSPACEPOINT_H
#define QSPACEPOINT_H

#include <QApplication>

class QSpacePoint :
	public QObject
{
public:
	float m_fX,m_fY,m_fZ;
public:
	QSpacePoint(){}
public:
	~QSpacePoint(){}
};

#endif