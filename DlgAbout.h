#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QtGui>
#include <ui_About.h>

class CDlgAbout :
	public QDialog
{
	Q_OBJECT

public:
	CDlgAbout();
	~CDlgAbout();

	Ui::AboutDialog ui;
};

#endif