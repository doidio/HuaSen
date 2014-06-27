/********************************************************************************
** Form generated from reading UI file 'SelectDirectory.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTDIRECTORY_H
#define UI_SELECTDIRECTORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_SelectDirectoryDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QTreeView *SelectDirectoryTreeView;
    QTreeView *fileListTreeView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SelectDirectoryDialog)
    {
        if (SelectDirectoryDialog->objectName().isEmpty())
            SelectDirectoryDialog->setObjectName(QString::fromUtf8("SelectDirectoryDialog"));
        SelectDirectoryDialog->resize(737, 628);
        gridLayout = new QGridLayout(SelectDirectoryDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        SelectDirectoryTreeView = new QTreeView(SelectDirectoryDialog);
        SelectDirectoryTreeView->setObjectName(QString::fromUtf8("SelectDirectoryTreeView"));

        horizontalLayout->addWidget(SelectDirectoryTreeView);

        fileListTreeView = new QTreeView(SelectDirectoryDialog);
        fileListTreeView->setObjectName(QString::fromUtf8("fileListTreeView"));

        horizontalLayout->addWidget(fileListTreeView);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(SelectDirectoryDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(SelectDirectoryDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SelectDirectoryDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SelectDirectoryDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SelectDirectoryDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectDirectoryDialog)
    {
        SelectDirectoryDialog->setWindowTitle(QApplication::translate("SelectDirectoryDialog", "Select Directory", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectDirectoryDialog: public Ui_SelectDirectoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDIRECTORY_H
