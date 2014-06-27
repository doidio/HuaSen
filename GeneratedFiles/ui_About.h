/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QLabel *label;
    QLabel *label_Version;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(422, 368);
        horizontalLayoutWidget = new QWidget(AboutDialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(5, 5, 406, 226));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        toolButton = new QToolButton(horizontalLayoutWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
        toolButton->setSizePolicy(sizePolicy);
        toolButton->setStyleSheet(QString::fromUtf8("QToolButton \n"
"{ \n"
"  border-radius: 0px; \n"
"  image: url(:/images/UEG_emblem.png);\n"
"}"));

        horizontalLayout->addWidget(toolButton);

        toolButton_2 = new QToolButton(horizontalLayoutWidget);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        sizePolicy.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy);
        toolButton_2->setStyleSheet(QString::fromUtf8("QToolButton \n"
"{ \n"
"  border-radius: 0px; \n"
"  image: url(:/images/SJTU_emblem.png);\n"
"}"));

        horizontalLayout->addWidget(toolButton_2);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 2);
        label = new QLabel(AboutDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 260, 276, 111));
        label_Version = new QLabel(AboutDialog);
        label_Version->setObjectName(QString::fromUtf8("label_Version"));
        label_Version->setGeometry(QRect(140, 235, 151, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_Version->setFont(font);

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About AccuImplant", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QString());
        toolButton_2->setText(QString());
        label->setText(QApplication::translate("AboutDialog", "(C) 2009 Shanghai UEG Software Co.Ltd, China\n"
"Shanghai Jiao Tong University, China\n"
"All rights reserved.\n"
"\n"
"Main contributor:\n"
"Dr.Xiaojun Chen (xiaojunchen@sjtu.edu.cn)", 0, QApplication::UnicodeUTF8));
        label_Version->setText(QApplication::translate("AboutDialog", "AccuImplant 1.0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
