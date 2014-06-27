/********************************************************************************
** Form generated from reading UI file 'ProjectInfo.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTINFO_H
#define UI_PROJECTINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "ctkDoubleSlider.h"
#include "ctkDoubleSpinBox.h"
#include "ctkPushButton.h"

QT_BEGIN_NAMESPACE

class Ui_ProjectInfo
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_PatientName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_Width;
    QLabel *label_3;
    QLineEdit *lineEdit_Height;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *lineEdit_PixelSpacing;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *lineEdit_SlicesNum;
    QLabel *label_7;
    QLineEdit *lineEdit_SliceInc;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    ctkDoubleSlider *DoubleSlider;
    ctkDoubleSpinBox *DoubleSpinBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_9;
    ctkDoubleSlider *DoubleSlider_2;
    ctkDoubleSpinBox *DoubleSpinBox_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_10;
    ctkDoubleSlider *DoubleSlider_3;
    ctkDoubleSpinBox *DoubleSpinBox_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    ctkPushButton *PushButton;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *ProjectInfo)
    {
        if (ProjectInfo->objectName().isEmpty())
            ProjectInfo->setObjectName(QString::fromUtf8("ProjectInfo"));
        ProjectInfo->resize(490, 276);
        verticalLayout = new QVBoxLayout(ProjectInfo);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ProjectInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        lineEdit_PatientName = new QLineEdit(ProjectInfo);
        lineEdit_PatientName->setObjectName(QString::fromUtf8("lineEdit_PatientName"));
        lineEdit_PatientName->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_PatientName->sizePolicy().hasHeightForWidth());
        lineEdit_PatientName->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(lineEdit_PatientName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(ProjectInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        lineEdit_Width = new QLineEdit(ProjectInfo);
        lineEdit_Width->setObjectName(QString::fromUtf8("lineEdit_Width"));
        lineEdit_Width->setEnabled(false);

        horizontalLayout_2->addWidget(lineEdit_Width);

        label_3 = new QLabel(ProjectInfo);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_3);

        lineEdit_Height = new QLineEdit(ProjectInfo);
        lineEdit_Height->setObjectName(QString::fromUtf8("lineEdit_Height"));
        lineEdit_Height->setEnabled(false);

        horizontalLayout_2->addWidget(lineEdit_Height);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(ProjectInfo);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_4);

        lineEdit_PixelSpacing = new QLineEdit(ProjectInfo);
        lineEdit_PixelSpacing->setObjectName(QString::fromUtf8("lineEdit_PixelSpacing"));
        lineEdit_PixelSpacing->setEnabled(false);

        horizontalLayout_3->addWidget(lineEdit_PixelSpacing);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(ProjectInfo);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_5);

        lineEdit_SlicesNum = new QLineEdit(ProjectInfo);
        lineEdit_SlicesNum->setObjectName(QString::fromUtf8("lineEdit_SlicesNum"));
        lineEdit_SlicesNum->setEnabled(false);

        horizontalLayout_4->addWidget(lineEdit_SlicesNum);

        label_7 = new QLabel(ProjectInfo);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_7);

        lineEdit_SliceInc = new QLineEdit(ProjectInfo);
        lineEdit_SliceInc->setObjectName(QString::fromUtf8("lineEdit_SliceInc"));
        lineEdit_SliceInc->setEnabled(false);

        horizontalLayout_4->addWidget(lineEdit_SliceInc);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(ProjectInfo);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(150, 0));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_7->addWidget(label_8);

        DoubleSlider = new ctkDoubleSlider(ProjectInfo);
        DoubleSlider->setObjectName(QString::fromUtf8("DoubleSlider"));
        DoubleSlider->setValue(2);
        DoubleSlider->setSingleStep(0.001);
        DoubleSlider->setPageStep(0.1);
        DoubleSlider->setMaximum(10);
        DoubleSlider->setTickInterval(0.5);
        DoubleSlider->setTickPosition(QSlider::TicksBothSides);
        DoubleSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(DoubleSlider);

        DoubleSpinBox = new ctkDoubleSpinBox(ProjectInfo);
        DoubleSpinBox->setObjectName(QString::fromUtf8("DoubleSpinBox"));
        DoubleSpinBox->setMinimumSize(QSize(80, 0));
        DoubleSpinBox->setMaximumSize(QSize(80, 16777215));
        DoubleSpinBox->setDecimals(3);
        DoubleSpinBox->setMaximum(10);
        DoubleSpinBox->setSingleStep(0.1);
        DoubleSpinBox->setValue(2);

        horizontalLayout_7->addWidget(DoubleSpinBox);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_9 = new QLabel(ProjectInfo);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(150, 0));
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_9);

        DoubleSlider_2 = new ctkDoubleSlider(ProjectInfo);
        DoubleSlider_2->setObjectName(QString::fromUtf8("DoubleSlider_2"));
        DoubleSlider_2->setValue(2);
        DoubleSlider_2->setSingleStep(0.001);
        DoubleSlider_2->setPageStep(1);
        DoubleSlider_2->setMaximum(10);
        DoubleSlider_2->setTickInterval(0.5);
        DoubleSlider_2->setTickPosition(QSlider::TicksBothSides);
        DoubleSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(DoubleSlider_2);

        DoubleSpinBox_2 = new ctkDoubleSpinBox(ProjectInfo);
        DoubleSpinBox_2->setObjectName(QString::fromUtf8("DoubleSpinBox_2"));
        DoubleSpinBox_2->setMinimumSize(QSize(80, 0));
        DoubleSpinBox_2->setMaximumSize(QSize(80, 16777215));
        DoubleSpinBox_2->setDecimals(3);
        DoubleSpinBox_2->setMaximum(10);
        DoubleSpinBox_2->setSingleStep(0.1);
        DoubleSpinBox_2->setValue(2);

        horizontalLayout_5->addWidget(DoubleSpinBox_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_10 = new QLabel(ProjectInfo);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(150, 0));
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_10);

        DoubleSlider_3 = new ctkDoubleSlider(ProjectInfo);
        DoubleSlider_3->setObjectName(QString::fromUtf8("DoubleSlider_3"));
        DoubleSlider_3->setValue(0.15);
        DoubleSlider_3->setSingleStep(0.01);
        DoubleSlider_3->setPageStep(0.1);
        DoubleSlider_3->setMaximum(2);
        DoubleSlider_3->setTickInterval(0.1);
        DoubleSlider_3->setTickPosition(QSlider::TicksBothSides);
        DoubleSlider_3->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(DoubleSlider_3);

        DoubleSpinBox_3 = new ctkDoubleSpinBox(ProjectInfo);
        DoubleSpinBox_3->setObjectName(QString::fromUtf8("DoubleSpinBox_3"));
        DoubleSpinBox_3->setMinimumSize(QSize(80, 0));
        DoubleSpinBox_3->setMaximumSize(QSize(80, 16777215));
        DoubleSpinBox_3->setDecimals(3);
        DoubleSpinBox_3->setMaximum(2);
        DoubleSpinBox_3->setSingleStep(0.1);
        DoubleSpinBox_3->setValue(0.15);

        horizontalLayout_8->addWidget(DoubleSpinBox_3);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        PushButton = new ctkPushButton(ProjectInfo);
        PushButton->setObjectName(QString::fromUtf8("PushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(PushButton->sizePolicy().hasHeightForWidth());
        PushButton->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(PushButton);

        pushButton = new QPushButton(ProjectInfo);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy3);

        horizontalLayout_6->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(ProjectInfo);
        QObject::connect(PushButton, SIGNAL(clicked()), ProjectInfo, SLOT(accept()));
        QObject::connect(pushButton, SIGNAL(clicked()), ProjectInfo, SLOT(reject()));
        QObject::connect(DoubleSlider, SIGNAL(valueChanged(double)), DoubleSpinBox, SLOT(setValueIfDifferent(double)));
        QObject::connect(DoubleSpinBox, SIGNAL(valueChanged(double)), DoubleSlider, SLOT(setValue(double)));
        QObject::connect(DoubleSpinBox_2, SIGNAL(valueChanged(double)), DoubleSlider_2, SLOT(setValue(double)));
        QObject::connect(DoubleSlider_2, SIGNAL(valueChanged(double)), DoubleSpinBox_2, SLOT(setValueIfDifferent(double)));
        QObject::connect(DoubleSlider_3, SIGNAL(valueChanged(double)), DoubleSpinBox_3, SLOT(setValue(double)));
        QObject::connect(DoubleSpinBox_3, SIGNAL(valueChanged(double)), DoubleSlider_3, SLOT(setValue(double)));

        QMetaObject::connectSlotsByName(ProjectInfo);
    } // setupUi

    void retranslateUi(QDialog *ProjectInfo)
    {
        ProjectInfo->setWindowTitle(QApplication::translate("ProjectInfo", "Project Infomation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProjectInfo", "Patient Name", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProjectInfo", "Width", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ProjectInfo", "Height", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ProjectInfo", "Pixel Spacing", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ProjectInfo", "Number of Slices", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ProjectInfo", "Slice Increment", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ProjectInfo", "Diameter of Holes", 0, QApplication::UnicodeUTF8));
        DoubleSpinBox->setSuffix(QApplication::translate("ProjectInfo", " mm", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ProjectInfo", "Thickness of Template", 0, QApplication::UnicodeUTF8));
        DoubleSpinBox_2->setSuffix(QApplication::translate("ProjectInfo", " mm", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("ProjectInfo", "Radius of Lines", 0, QApplication::UnicodeUTF8));
        DoubleSpinBox_3->setSuffix(QApplication::translate("ProjectInfo", " mm", 0, QApplication::UnicodeUTF8));
        PushButton->setText(QApplication::translate("ProjectInfo", "OK", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ProjectInfo", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProjectInfo: public Ui_ProjectInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTINFO_H
