#ifndef DLGPASSWORD_H
#define DLGPASSWORD_H

#include <QtGui>
#include <ui_Password.h>

class CDlgPassword:
	public QDialog
{
	Q_OBJECT
public:
	CDlgPassword();
	
public:
	Ui::DialogPassword ui;

	
	~CDlgPassword();


};

#endif