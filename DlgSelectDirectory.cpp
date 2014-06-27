#include "DlgSelectDirectory.h"

DlgSelectDirectory::DlgSelectDirectory()
{
	ui.setupUi(this);

	//ui.SelectDirectoryTreeView->setRootIsDecorated(false);
    //ui.AllPointTreeView->setAlternatingRowColors(true);
	//ui.SelectDirectoryTreeView->setSortingEnabled(true);
	ui.fileListTreeView->setRootIsDecorated(false);
	ui.fileListTreeView->setSortingEnabled(true);

	model = new QDirModel();
	model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
	setSourceModel(model);	

	pStandardItemModel = new QStandardItemModel(0, 1);

	connect(ui.SelectDirectoryTreeView, SIGNAL(clicked (QModelIndex)), this, SLOT(OnSetCurrentDirFiles()));

	//ui.AllPointTreeView->setEditFocus  (false);
	//ui.SelectDirectoryTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void DlgSelectDirectory::setSourceModel(QAbstractItemModel *model)
{
    //SourceModel->setSourceModel(model);
    ui.SelectDirectoryTreeView->setModel(model);

	//ui.SelectDirectoryTreeView->setColumnWidth(0,20);
	ui.SelectDirectoryTreeView->setColumnHidden(1,true);
	ui.SelectDirectoryTreeView->setColumnHidden(2,true);
	ui.SelectDirectoryTreeView->setColumnHidden(3,true);
}

void DlgSelectDirectory::OnSetCurrentDirFiles()
{
	QItemSelectionModel *selectionModel = ui.SelectDirectoryTreeView->selectionModel();	
	QModelIndex index=selectionModel->currentIndex();
	m_sDirectory=model->filePath(index);
	//QString directory="c:/";

	//QStandardItemModel *pStandardItemModel = new QStandardItemModel(0, 1);
	delete pStandardItemModel;
	pStandardItemModel = new QStandardItemModel(0, 1);

    pStandardItemModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));

	QDir *dir=new QDir(m_sDirectory);
    QFileInfoList entries = dir->entryInfoList(QDir::Files);
    for (int i = 0; i < entries.size(); ++i)
	{
		pStandardItemModel->insertRow(0);
		pStandardItemModel->setData(pStandardItemModel->index(0, 0), entries.at(i).fileName());
        //ui.fileListTreeView->addItem(entries.at(i).fileName());
	}

	ui.fileListTreeView->setModel(pStandardItemModel);

	delete dir;
	//delete model;
}

DlgSelectDirectory::~DlgSelectDirectory()
{
	delete model;
	delete pStandardItemModel;
}
