#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QDialog>
#include "ui_ProjectInfo.h"

class ProjectInfo : public QDialog
{
	Q_OBJECT

public:
	ProjectInfo(QWidget *parent = 0);
	~ProjectInfo();

	Ui::ProjectInfo ui;

private:
	
};

#endif // PROJECTINFO_H
