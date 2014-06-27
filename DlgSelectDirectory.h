#ifndef DLGSELECTDIRECTORY_H
#define DLGSELECTDIRECTORY_H

#include <QtGui>
#include <ui_SelectDirectory.h>

class DlgSelectDirectory:
	public QDialog
{
	Q_OBJECT
public:
	DlgSelectDirectory();
	void setSourceModel(QAbstractItemModel *model);
public:
	Ui::SelectDirectoryDialog ui;

	QDirModel *model;
	QStandardItemModel *pStandardItemModel;
	QString m_sDirectory;

	~DlgSelectDirectory();

private slots:
	void OnSetCurrentDirFiles();
};

#endif