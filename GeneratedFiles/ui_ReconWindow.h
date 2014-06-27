/********************************************************************************
** Form generated from reading UI file 'ReconWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECONWINDOW_H
#define UI_RECONWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ctkCollapsibleButton.h"
#include "qcustomvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_ReconWindow
{
public:
    QAction *FILE_OPEN;
    QAction *CONNECT;
    QAction *RECONSTRU3D;
    QAction *EXPORT_STL;
    QAction *TO_MAINWINDOW;
    QAction *CUT;
    QAction *VIEW_MAXIMIZE;
    QAction *SHOW_WIDGET;
    QAction *STL_OPEN;
    QAction *VIEW_MODE;
    QAction *DISTANCE_MEASURE;
    QAction *ANGLE_MEASURE;
    QAction *DELETE_GRAY;
    QAction *SELECT_PTS;
    QAction *TEM_GENERATE;
    QAction *IMPORT_AXIS;
    QAction *PUNCH;
    QAction *INFORMATION;
    QAction *CLOSE_CONTOURS;
    QAction *DELETE_PTS;
    QAction *UNDO_PUNCHING;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    ctkCollapsibleButton *settingCButton;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_panel;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QSlider *verticalSlider_threshold_max;
    QSpinBox *spinBox_threshold_max;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSlider *verticalSlider_width;
    QSpinBox *spinBox_width;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSlider *verticalSlider_threshold_min;
    QSpinBox *spinBox_threshold_min;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSlider *verticalSlider_center;
    QSpinBox *spinBox_center;
    QGridLayout *gridLayout_2;
    QCustomVTKWidget *AxialView;
    QScrollBar *AxialViewScrollBar;
    QCustomVTKWidget *ReconstruView;
    QCustomVTKWidget *SagittalView;
    QScrollBar *SagittalViewScrollBar;
    QCustomVTKWidget *CoronalView;
    QScrollBar *CoronalViewScrollBar;
    ctkCollapsibleButton *modelCButton;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_panel_3;
    QGridLayout *gridLayout_6;
    QTableWidget *modelTable;
    QPushButton *delModelButton;
    ctkCollapsibleButton *measureCButton;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_panel_2;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *measureTable;
    QPushButton *delMeasureButton;
    QProgressBar *progressBar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *ReconWindow)
    {
        if (ReconWindow->objectName().isEmpty())
            ReconWindow->setObjectName(QString::fromUtf8("ReconWindow"));
        ReconWindow->resize(1278, 738);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ReconWindow->sizePolicy().hasHeightForWidth());
        ReconWindow->setSizePolicy(sizePolicy);
        ReconWindow->setAutoFillBackground(false);
        FILE_OPEN = new QAction(ReconWindow);
        FILE_OPEN->setObjectName(QString::fromUtf8("FILE_OPEN"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/FILE_OPEN.png"), QSize(), QIcon::Normal, QIcon::Off);
        FILE_OPEN->setIcon(icon);
        CONNECT = new QAction(ReconWindow);
        CONNECT->setObjectName(QString::fromUtf8("CONNECT"));
        CONNECT->setCheckable(true);
        CONNECT->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/CONNECT.png"), QSize(), QIcon::Normal, QIcon::Off);
        CONNECT->setIcon(icon1);
        RECONSTRU3D = new QAction(ReconWindow);
        RECONSTRU3D->setObjectName(QString::fromUtf8("RECONSTRU3D"));
        RECONSTRU3D->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/RECONSTRU3D.png"), QSize(), QIcon::Normal, QIcon::Off);
        RECONSTRU3D->setIcon(icon2);
        EXPORT_STL = new QAction(ReconWindow);
        EXPORT_STL->setObjectName(QString::fromUtf8("EXPORT_STL"));
        EXPORT_STL->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/EXPORT_STL.png"), QSize(), QIcon::Normal, QIcon::Off);
        EXPORT_STL->setIcon(icon3);
        TO_MAINWINDOW = new QAction(ReconWindow);
        TO_MAINWINDOW->setObjectName(QString::fromUtf8("TO_MAINWINDOW"));
        TO_MAINWINDOW->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/TO_MAINWINDOW.png"), QSize(), QIcon::Normal, QIcon::Off);
        TO_MAINWINDOW->setIcon(icon4);
        CUT = new QAction(ReconWindow);
        CUT->setObjectName(QString::fromUtf8("CUT"));
        CUT->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/CROSS_SEC_SLICE.png"), QSize(), QIcon::Normal, QIcon::Off);
        CUT->setIcon(icon5);
        VIEW_MAXIMIZE = new QAction(ReconWindow);
        VIEW_MAXIMIZE->setObjectName(QString::fromUtf8("VIEW_MAXIMIZE"));
        VIEW_MAXIMIZE->setCheckable(true);
        VIEW_MAXIMIZE->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/VIEW_MAXIMIZE.png"), QSize(), QIcon::Normal, QIcon::Off);
        VIEW_MAXIMIZE->setIcon(icon6);
        SHOW_WIDGET = new QAction(ReconWindow);
        SHOW_WIDGET->setObjectName(QString::fromUtf8("SHOW_WIDGET"));
        SHOW_WIDGET->setCheckable(true);
        SHOW_WIDGET->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/SHOW_PLANE_WIDGET.png"), QSize(), QIcon::Normal, QIcon::Off);
        SHOW_WIDGET->setIcon(icon7);
        STL_OPEN = new QAction(ReconWindow);
        STL_OPEN->setObjectName(QString::fromUtf8("STL_OPEN"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/RECONSTRU.png"), QSize(), QIcon::Normal, QIcon::Off);
        STL_OPEN->setIcon(icon8);
        VIEW_MODE = new QAction(ReconWindow);
        VIEW_MODE->setObjectName(QString::fromUtf8("VIEW_MODE"));
        VIEW_MODE->setCheckable(true);
        VIEW_MODE->setChecked(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/GET_CAMERA_PARA.png"), QSize(), QIcon::Normal, QIcon::Off);
        VIEW_MODE->setIcon(icon9);
        DISTANCE_MEASURE = new QAction(ReconWindow);
        DISTANCE_MEASURE->setObjectName(QString::fromUtf8("DISTANCE_MEASURE"));
        DISTANCE_MEASURE->setCheckable(true);
        DISTANCE_MEASURE->setEnabled(false);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/POLYLINE.png"), QSize(), QIcon::Selected, QIcon::On);
        DISTANCE_MEASURE->setIcon(icon10);
        ANGLE_MEASURE = new QAction(ReconWindow);
        ANGLE_MEASURE->setObjectName(QString::fromUtf8("ANGLE_MEASURE"));
        ANGLE_MEASURE->setCheckable(true);
        ANGLE_MEASURE->setEnabled(false);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/ANGLE_POINT.png"), QSize(), QIcon::Normal, QIcon::Off);
        ANGLE_MEASURE->setIcon(icon11);
        DELETE_GRAY = new QAction(ReconWindow);
        DELETE_GRAY->setObjectName(QString::fromUtf8("DELETE_GRAY"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/DELETE_ACTOR.png"), QSize(), QIcon::Normal, QIcon::Off);
        DELETE_GRAY->setIcon(icon12);
        SELECT_PTS = new QAction(ReconWindow);
        SELECT_PTS->setObjectName(QString::fromUtf8("SELECT_PTS"));
        SELECT_PTS->setCheckable(true);
        SELECT_PTS->setEnabled(false);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/DRAW_NERVE.png"), QSize(), QIcon::Normal, QIcon::Off);
        SELECT_PTS->setIcon(icon13);
        TEM_GENERATE = new QAction(ReconWindow);
        TEM_GENERATE->setObjectName(QString::fromUtf8("TEM_GENERATE"));
        TEM_GENERATE->setEnabled(false);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/VOLUME_RENDERING.png"), QSize(), QIcon::Normal, QIcon::Off);
        TEM_GENERATE->setIcon(icon14);
        IMPORT_AXIS = new QAction(ReconWindow);
        IMPORT_AXIS->setObjectName(QString::fromUtf8("IMPORT_AXIS"));
        IMPORT_AXIS->setCheckable(true);
        IMPORT_AXIS->setEnabled(false);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/SHOW_WIDGET.png"), QSize(), QIcon::Normal, QIcon::Off);
        IMPORT_AXIS->setIcon(icon15);
        PUNCH = new QAction(ReconWindow);
        PUNCH->setObjectName(QString::fromUtf8("PUNCH"));
        PUNCH->setEnabled(false);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/images/DESIGN_IMPLANT.png"), QSize(), QIcon::Normal, QIcon::Off);
        PUNCH->setIcon(icon16);
        INFORMATION = new QAction(ReconWindow);
        INFORMATION->setObjectName(QString::fromUtf8("INFORMATION"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/images/HELP.png"), QSize(), QIcon::Normal, QIcon::Off);
        INFORMATION->setIcon(icon17);
        CLOSE_CONTOURS = new QAction(ReconWindow);
        CLOSE_CONTOURS->setObjectName(QString::fromUtf8("CLOSE_CONTOURS"));
        CLOSE_CONTOURS->setEnabled(false);
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/images/NERVE_RESLICE.png"), QSize(), QIcon::Normal, QIcon::Off);
        CLOSE_CONTOURS->setIcon(icon18);
        DELETE_PTS = new QAction(ReconWindow);
        DELETE_PTS->setObjectName(QString::fromUtf8("DELETE_PTS"));
        DELETE_PTS->setEnabled(false);
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/images/DELETE_NERVE.png"), QSize(), QIcon::Normal, QIcon::Off);
        DELETE_PTS->setIcon(icon19);
        UNDO_PUNCHING = new QAction(ReconWindow);
        UNDO_PUNCHING->setObjectName(QString::fromUtf8("UNDO_PUNCHING"));
        UNDO_PUNCHING->setEnabled(false);
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/images/DELETE_PLANT.png"), QSize(), QIcon::Normal, QIcon::Off);
        UNDO_PUNCHING->setIcon(icon20);
        centralwidget = new QWidget(ReconWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setContentsMargins(1, 1, 1, 1);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        settingCButton = new ctkCollapsibleButton(centralwidget);
        settingCButton->setObjectName(QString::fromUtf8("settingCButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(settingCButton->sizePolicy().hasHeightForWidth());
        settingCButton->setSizePolicy(sizePolicy1);
        settingCButton->setMinimumSize(QSize(200, 0));
        settingCButton->setMaximumSize(QSize(200, 16777215));
        settingCButton->setChecked(true);
        settingCButton->setCollapsed(false);
        settingCButton->setCollapsedHeight(0);
        settingCButton->setContentsFrameShape(QFrame::StyledPanel);
        verticalLayout_4 = new QVBoxLayout(settingCButton);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widget_panel = new QWidget(settingCButton);
        widget_panel->setObjectName(QString::fromUtf8("widget_panel"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_panel->sizePolicy().hasHeightForWidth());
        widget_panel->setSizePolicy(sizePolicy2);
        gridLayout = new QGridLayout(widget_panel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(widget_panel);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy3);
        label_4->setMinimumSize(QSize(50, 20));
        label_4->setMaximumSize(QSize(36, 20));
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_4);

        verticalSlider_threshold_max = new QSlider(widget_panel);
        verticalSlider_threshold_max->setObjectName(QString::fromUtf8("verticalSlider_threshold_max"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(1);
        sizePolicy4.setHeightForWidth(verticalSlider_threshold_max->sizePolicy().hasHeightForWidth());
        verticalSlider_threshold_max->setSizePolicy(sizePolicy4);
        verticalSlider_threshold_max->setMinimumSize(QSize(0, 20));
        verticalSlider_threshold_max->setMaximumSize(QSize(16777215, 20));
        verticalSlider_threshold_max->setAutoFillBackground(false);
        verticalSlider_threshold_max->setMinimum(-1024);
        verticalSlider_threshold_max->setMaximum(3071);
        verticalSlider_threshold_max->setValue(3000);
        verticalSlider_threshold_max->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(verticalSlider_threshold_max);

        spinBox_threshold_max = new QSpinBox(widget_panel);
        spinBox_threshold_max->setObjectName(QString::fromUtf8("spinBox_threshold_max"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(spinBox_threshold_max->sizePolicy().hasHeightForWidth());
        spinBox_threshold_max->setSizePolicy(sizePolicy5);
        spinBox_threshold_max->setMinimumSize(QSize(50, 20));
        spinBox_threshold_max->setMaximumSize(QSize(50, 20));
        spinBox_threshold_max->setMinimum(-1024);
        spinBox_threshold_max->setMaximum(3071);
        spinBox_threshold_max->setValue(3000);

        horizontalLayout_5->addWidget(spinBox_threshold_max);


        gridLayout_3->addLayout(horizontalLayout_5, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(widget_panel);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(36, 20));
        label_2->setMaximumSize(QSize(36, 20));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_2);

        verticalSlider_width = new QSlider(widget_panel);
        verticalSlider_width->setObjectName(QString::fromUtf8("verticalSlider_width"));
        sizePolicy4.setHeightForWidth(verticalSlider_width->sizePolicy().hasHeightForWidth());
        verticalSlider_width->setSizePolicy(sizePolicy4);
        verticalSlider_width->setMinimumSize(QSize(0, 20));
        verticalSlider_width->setMaximumSize(QSize(16777215, 20));
        verticalSlider_width->setMinimum(10);
        verticalSlider_width->setMaximum(4095);
        verticalSlider_width->setValue(400);
        verticalSlider_width->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(verticalSlider_width);

        spinBox_width = new QSpinBox(widget_panel);
        spinBox_width->setObjectName(QString::fromUtf8("spinBox_width"));
        sizePolicy5.setHeightForWidth(spinBox_width->sizePolicy().hasHeightForWidth());
        spinBox_width->setSizePolicy(sizePolicy5);
        spinBox_width->setMinimumSize(QSize(50, 20));
        spinBox_width->setMaximumSize(QSize(50, 20));
        spinBox_width->setMinimum(10);
        spinBox_width->setMaximum(4095);
        spinBox_width->setValue(400);

        horizontalLayout_3->addWidget(spinBox_width);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(widget_panel);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);
        label_3->setMinimumSize(QSize(50, 20));
        label_3->setMaximumSize(QSize(36, 20));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_3);

        verticalSlider_threshold_min = new QSlider(widget_panel);
        verticalSlider_threshold_min->setObjectName(QString::fromUtf8("verticalSlider_threshold_min"));
        sizePolicy4.setHeightForWidth(verticalSlider_threshold_min->sizePolicy().hasHeightForWidth());
        verticalSlider_threshold_min->setSizePolicy(sizePolicy4);
        verticalSlider_threshold_min->setMinimumSize(QSize(0, 20));
        verticalSlider_threshold_min->setMaximumSize(QSize(16777215, 20));
        verticalSlider_threshold_min->setMinimum(-1024);
        verticalSlider_threshold_min->setMaximum(3071);
        verticalSlider_threshold_min->setValue(115);
        verticalSlider_threshold_min->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(verticalSlider_threshold_min);

        spinBox_threshold_min = new QSpinBox(widget_panel);
        spinBox_threshold_min->setObjectName(QString::fromUtf8("spinBox_threshold_min"));
        sizePolicy5.setHeightForWidth(spinBox_threshold_min->sizePolicy().hasHeightForWidth());
        spinBox_threshold_min->setSizePolicy(sizePolicy5);
        spinBox_threshold_min->setMinimumSize(QSize(50, 20));
        spinBox_threshold_min->setMaximumSize(QSize(50, 20));
        spinBox_threshold_min->setMinimum(-1024);
        spinBox_threshold_min->setMaximum(3071);
        spinBox_threshold_min->setValue(115);

        horizontalLayout_4->addWidget(spinBox_threshold_min);


        gridLayout_3->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(widget_panel);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(36, 20));
        label->setMaximumSize(QSize(36, 20));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        verticalSlider_center = new QSlider(widget_panel);
        verticalSlider_center->setObjectName(QString::fromUtf8("verticalSlider_center"));
        sizePolicy4.setHeightForWidth(verticalSlider_center->sizePolicy().hasHeightForWidth());
        verticalSlider_center->setSizePolicy(sizePolicy4);
        verticalSlider_center->setMinimumSize(QSize(0, 20));
        verticalSlider_center->setMaximumSize(QSize(16777215, 20));
        verticalSlider_center->setMaximum(3096);
        verticalSlider_center->setValue(40);
        verticalSlider_center->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(verticalSlider_center);

        spinBox_center = new QSpinBox(widget_panel);
        spinBox_center->setObjectName(QString::fromUtf8("spinBox_center"));
        sizePolicy5.setHeightForWidth(spinBox_center->sizePolicy().hasHeightForWidth());
        spinBox_center->setSizePolicy(sizePolicy5);
        spinBox_center->setMinimumSize(QSize(50, 20));
        spinBox_center->setMaximumSize(QSize(50, 20));
        spinBox_center->setMaximum(3096);
        spinBox_center->setValue(40);

        horizontalLayout_2->addWidget(spinBox_center);


        gridLayout_3->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout_3, 0, 0, 1, 1);


        verticalLayout_4->addWidget(widget_panel);


        gridLayout_4->addWidget(settingCButton, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(1);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        AxialView = new QCustomVTKWidget(centralwidget);
        AxialView->setObjectName(QString::fromUtf8("AxialView"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(3);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(AxialView->sizePolicy().hasHeightForWidth());
        AxialView->setSizePolicy(sizePolicy6);
        AxialView->setMinimumSize(QSize(500, 320));
        AxialView->setCursor(QCursor(Qt::CrossCursor));

        gridLayout_2->addWidget(AxialView, 0, 0, 1, 1);

        AxialViewScrollBar = new QScrollBar(centralwidget);
        AxialViewScrollBar->setObjectName(QString::fromUtf8("AxialViewScrollBar"));
        AxialViewScrollBar->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(AxialViewScrollBar, 0, 1, 1, 1);

        ReconstruView = new QCustomVTKWidget(centralwidget);
        ReconstruView->setObjectName(QString::fromUtf8("ReconstruView"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(1);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(ReconstruView->sizePolicy().hasHeightForWidth());
        ReconstruView->setSizePolicy(sizePolicy7);
        ReconstruView->setMinimumSize(QSize(500, 320));

        gridLayout_2->addWidget(ReconstruView, 0, 2, 1, 1);

        SagittalView = new QCustomVTKWidget(centralwidget);
        SagittalView->setObjectName(QString::fromUtf8("SagittalView"));
        sizePolicy7.setHeightForWidth(SagittalView->sizePolicy().hasHeightForWidth());
        SagittalView->setSizePolicy(sizePolicy7);
        SagittalView->setMinimumSize(QSize(500, 320));
        SagittalView->setCursor(QCursor(Qt::CrossCursor));

        gridLayout_2->addWidget(SagittalView, 1, 0, 1, 1);

        SagittalViewScrollBar = new QScrollBar(centralwidget);
        SagittalViewScrollBar->setObjectName(QString::fromUtf8("SagittalViewScrollBar"));
        SagittalViewScrollBar->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(SagittalViewScrollBar, 1, 1, 1, 1);

        CoronalView = new QCustomVTKWidget(centralwidget);
        CoronalView->setObjectName(QString::fromUtf8("CoronalView"));
        sizePolicy6.setHeightForWidth(CoronalView->sizePolicy().hasHeightForWidth());
        CoronalView->setSizePolicy(sizePolicy6);
        CoronalView->setMinimumSize(QSize(500, 320));
        CoronalView->setCursor(QCursor(Qt::CrossCursor));

        gridLayout_2->addWidget(CoronalView, 1, 2, 1, 1);

        CoronalViewScrollBar = new QScrollBar(centralwidget);
        CoronalViewScrollBar->setObjectName(QString::fromUtf8("CoronalViewScrollBar"));
        CoronalViewScrollBar->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(CoronalViewScrollBar, 1, 3, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 1, 4, 1);

        modelCButton = new ctkCollapsibleButton(centralwidget);
        modelCButton->setObjectName(QString::fromUtf8("modelCButton"));
        QSizePolicy sizePolicy8(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(2);
        sizePolicy8.setHeightForWidth(modelCButton->sizePolicy().hasHeightForWidth());
        modelCButton->setSizePolicy(sizePolicy8);
        modelCButton->setMinimumSize(QSize(200, 0));
        modelCButton->setMaximumSize(QSize(200, 16777215));
        modelCButton->setCollapsedHeight(0);
        modelCButton->setContentsFrameShape(QFrame::StyledPanel);
        verticalLayout_3 = new QVBoxLayout(modelCButton);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_panel_3 = new QWidget(modelCButton);
        widget_panel_3->setObjectName(QString::fromUtf8("widget_panel_3"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(2);
        sizePolicy9.setHeightForWidth(widget_panel_3->sizePolicy().hasHeightForWidth());
        widget_panel_3->setSizePolicy(sizePolicy9);
        widget_panel_3->setMinimumSize(QSize(0, 0));
        gridLayout_6 = new QGridLayout(widget_panel_3);
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setVerticalSpacing(0);
        modelTable = new QTableWidget(widget_panel_3);
        if (modelTable->columnCount() < 3)
            modelTable->setColumnCount(3);
        QFont font;
        font.setKerning(false);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qtablewidgetitem->setFont(font);
        modelTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        modelTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        modelTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        modelTable->setObjectName(QString::fromUtf8("modelTable"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(2);
        sizePolicy10.setHeightForWidth(modelTable->sizePolicy().hasHeightForWidth());
        modelTable->setSizePolicy(sizePolicy10);
        modelTable->setFrameShape(QFrame::StyledPanel);
        modelTable->setFrameShadow(QFrame::Sunken);
        modelTable->setLineWidth(1);
        modelTable->setAutoScrollMargin(10);
        modelTable->setSelectionMode(QAbstractItemView::SingleSelection);
        modelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        modelTable->horizontalHeader()->setDefaultSectionSize(60);
        modelTable->horizontalHeader()->setHighlightSections(false);
        modelTable->horizontalHeader()->setMinimumSectionSize(60);
        modelTable->horizontalHeader()->setStretchLastSection(true);
        modelTable->verticalHeader()->setVisible(true);
        modelTable->verticalHeader()->setDefaultSectionSize(20);
        modelTable->verticalHeader()->setMinimumSectionSize(20);
        modelTable->verticalHeader()->setStretchLastSection(false);

        gridLayout_6->addWidget(modelTable, 0, 0, 1, 1);

        delModelButton = new QPushButton(widget_panel_3);
        delModelButton->setObjectName(QString::fromUtf8("delModelButton"));

        gridLayout_6->addWidget(delModelButton, 1, 0, 1, 1);


        verticalLayout_3->addWidget(widget_panel_3);


        gridLayout_4->addWidget(modelCButton, 1, 0, 1, 1);

        measureCButton = new ctkCollapsibleButton(centralwidget);
        measureCButton->setObjectName(QString::fromUtf8("measureCButton"));
        QSizePolicy sizePolicy11(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(1);
        sizePolicy11.setHeightForWidth(measureCButton->sizePolicy().hasHeightForWidth());
        measureCButton->setSizePolicy(sizePolicy11);
        measureCButton->setMinimumSize(QSize(200, 0));
        measureCButton->setMaximumSize(QSize(200, 16777215));
        measureCButton->setCollapsed(false);
        measureCButton->setCollapsedHeight(0);
        measureCButton->setContentsFrameShape(QFrame::StyledPanel);
        verticalLayout_2 = new QVBoxLayout(measureCButton);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget_panel_2 = new QWidget(measureCButton);
        widget_panel_2->setObjectName(QString::fromUtf8("widget_panel_2"));
        QSizePolicy sizePolicy12(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy12.setHorizontalStretch(0);
        sizePolicy12.setVerticalStretch(1);
        sizePolicy12.setHeightForWidth(widget_panel_2->sizePolicy().hasHeightForWidth());
        widget_panel_2->setSizePolicy(sizePolicy12);
        verticalLayout_5 = new QVBoxLayout(widget_panel_2);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        measureTable = new QTableWidget(widget_panel_2);
        if (measureTable->columnCount() < 2)
            measureTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qtablewidgetitem3->setFont(font);
        measureTable->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        measureTable->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        measureTable->setObjectName(QString::fromUtf8("measureTable"));
        sizePolicy10.setHeightForWidth(measureTable->sizePolicy().hasHeightForWidth());
        measureTable->setSizePolicy(sizePolicy10);
        measureTable->setFrameShape(QFrame::StyledPanel);
        measureTable->setFrameShadow(QFrame::Sunken);
        measureTable->setLineWidth(1);
        measureTable->setAutoScrollMargin(10);
        measureTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        measureTable->setSelectionMode(QAbstractItemView::SingleSelection);
        measureTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        measureTable->horizontalHeader()->setDefaultSectionSize(60);
        measureTable->horizontalHeader()->setHighlightSections(false);
        measureTable->horizontalHeader()->setMinimumSectionSize(60);
        measureTable->horizontalHeader()->setStretchLastSection(true);
        measureTable->verticalHeader()->setVisible(true);
        measureTable->verticalHeader()->setDefaultSectionSize(20);
        measureTable->verticalHeader()->setMinimumSectionSize(20);
        measureTable->verticalHeader()->setStretchLastSection(false);

        verticalLayout_5->addWidget(measureTable);

        delMeasureButton = new QPushButton(widget_panel_2);
        delMeasureButton->setObjectName(QString::fromUtf8("delMeasureButton"));

        verticalLayout_5->addWidget(delMeasureButton);


        verticalLayout_2->addWidget(widget_panel_2);


        gridLayout_4->addWidget(measureCButton, 2, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        QSizePolicy sizePolicy13(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(0);
        sizePolicy13.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy13);
        progressBar->setMinimumSize(QSize(200, 0));
        progressBar->setMaximumSize(QSize(16777215, 20));
        progressBar->setValue(20);

        gridLayout_4->addWidget(progressBar, 3, 0, 1, 1);

        ReconWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ReconWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ReconWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(ReconWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setEnabled(true);
        ReconWindow->addToolBar(Qt::TopToolBarArea, toolBar);
#ifndef QT_NO_SHORTCUT
        label_4->setBuddy(spinBox_threshold_max);
        label_2->setBuddy(spinBox_width);
        label_3->setBuddy(spinBox_threshold_min);
        label->setBuddy(spinBox_center);
#endif // QT_NO_SHORTCUT

        toolBar->addAction(FILE_OPEN);
        toolBar->addAction(STL_OPEN);
        toolBar->addAction(INFORMATION);
        toolBar->addSeparator();
        toolBar->addAction(CONNECT);
        toolBar->addAction(RECONSTRU3D);
        toolBar->addSeparator();
        toolBar->addAction(SHOW_WIDGET);
        toolBar->addAction(CUT);
        toolBar->addSeparator();
        toolBar->addAction(DISTANCE_MEASURE);
        toolBar->addAction(ANGLE_MEASURE);
        toolBar->addSeparator();
        toolBar->addAction(SELECT_PTS);
        toolBar->addAction(DELETE_PTS);
        toolBar->addAction(CLOSE_CONTOURS);
        toolBar->addAction(TEM_GENERATE);
        toolBar->addAction(IMPORT_AXIS);
        toolBar->addAction(PUNCH);
        toolBar->addAction(UNDO_PUNCHING);
        toolBar->addSeparator();
        toolBar->addAction(EXPORT_STL);
        toolBar->addAction(TO_MAINWINDOW);
        toolBar->addSeparator();
        toolBar->addAction(VIEW_MAXIMIZE);
        toolBar->addAction(VIEW_MODE);

        retranslateUi(ReconWindow);
        QObject::connect(verticalSlider_center, SIGNAL(valueChanged(int)), spinBox_center, SLOT(setValue(int)));
        QObject::connect(spinBox_center, SIGNAL(valueChanged(int)), verticalSlider_center, SLOT(setValue(int)));
        QObject::connect(verticalSlider_threshold_min, SIGNAL(valueChanged(int)), spinBox_threshold_min, SLOT(setValue(int)));
        QObject::connect(spinBox_threshold_min, SIGNAL(valueChanged(int)), verticalSlider_threshold_min, SLOT(setValue(int)));
        QObject::connect(spinBox_width, SIGNAL(valueChanged(int)), verticalSlider_width, SLOT(setValue(int)));
        QObject::connect(spinBox_threshold_max, SIGNAL(valueChanged(int)), verticalSlider_threshold_max, SLOT(setValue(int)));
        QObject::connect(verticalSlider_width, SIGNAL(valueChanged(int)), spinBox_width, SLOT(setValue(int)));
        QObject::connect(verticalSlider_threshold_max, SIGNAL(valueChanged(int)), spinBox_threshold_max, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(ReconWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ReconWindow)
    {
        ReconWindow->setWindowTitle(QApplication::translate("ReconWindow", "Huasen 3D-ORTHO Surgical Planing System", 0, QApplication::UnicodeUTF8));
        FILE_OPEN->setText(QApplication::translate("ReconWindow", "Select a Directory", 0, QApplication::UnicodeUTF8));
        FILE_OPEN->setIconText(QApplication::translate("ReconWindow", "Select a DICOM Directory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        FILE_OPEN->setToolTip(QApplication::translate("ReconWindow", "Select a DICOM Directory", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        FILE_OPEN->setStatusTip(QApplication::translate("ReconWindow", "Select a DICOM Directory", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        CONNECT->setText(QApplication::translate("ReconWindow", "Region Growing Segmentation", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        CONNECT->setToolTip(QApplication::translate("ReconWindow", "Region Growing Segmentation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        CONNECT->setStatusTip(QApplication::translate("ReconWindow", "Region Growing Segmentation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        RECONSTRU3D->setText(QApplication::translate("ReconWindow", "3D Reconstruction", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        RECONSTRU3D->setToolTip(QApplication::translate("ReconWindow", "3D Reconstruction", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        RECONSTRU3D->setStatusTip(QApplication::translate("ReconWindow", "3D Reconstruction", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        EXPORT_STL->setText(QApplication::translate("ReconWindow", "Export STL", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        EXPORT_STL->setToolTip(QApplication::translate("ReconWindow", "Export STL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        EXPORT_STL->setStatusTip(QApplication::translate("ReconWindow", "Export STL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        TO_MAINWINDOW->setText(QApplication::translate("ReconWindow", "To Cura", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TO_MAINWINDOW->setToolTip(QApplication::translate("ReconWindow", "To Cura", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TO_MAINWINDOW->setStatusTip(QApplication::translate("ReconWindow", "To Cura", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        CUT->setText(QApplication::translate("ReconWindow", "CUT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        CUT->setStatusTip(QApplication::translate("ReconWindow", "CUT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        VIEW_MAXIMIZE->setText(QApplication::translate("ReconWindow", "Zoom In/Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        VIEW_MAXIMIZE->setStatusTip(QApplication::translate("ReconWindow", "Zoom In/Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        SHOW_WIDGET->setText(QApplication::translate("ReconWindow", "Show Plane Widget", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        SHOW_WIDGET->setStatusTip(QApplication::translate("ReconWindow", "Show Plane Widget", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        STL_OPEN->setText(QApplication::translate("ReconWindow", "Select a STL File", 0, QApplication::UnicodeUTF8));
        STL_OPEN->setIconText(QApplication::translate("ReconWindow", "Open a STL File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        STL_OPEN->setToolTip(QApplication::translate("ReconWindow", "Open a STL File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        STL_OPEN->setStatusTip(QApplication::translate("ReconWindow", "Open a STL File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        VIEW_MODE->setText(QApplication::translate("ReconWindow", "Select View Mode", 0, QApplication::UnicodeUTF8));
        VIEW_MODE->setIconText(QApplication::translate("ReconWindow", "Switch View Mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        VIEW_MODE->setToolTip(QApplication::translate("ReconWindow", "Switch View Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        VIEW_MODE->setStatusTip(QApplication::translate("ReconWindow", "Switch View Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        DISTANCE_MEASURE->setText(QApplication::translate("ReconWindow", "Distance Measure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DISTANCE_MEASURE->setToolTip(QApplication::translate("ReconWindow", "Distance Measure", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        DISTANCE_MEASURE->setStatusTip(QApplication::translate("ReconWindow", "Distance Measure", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        ANGLE_MEASURE->setText(QApplication::translate("ReconWindow", "Angle Measure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ANGLE_MEASURE->setToolTip(QApplication::translate("ReconWindow", "Angle Measure", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        ANGLE_MEASURE->setStatusTip(QApplication::translate("ReconWindow", "Angle Measure", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        DELETE_GRAY->setText(QApplication::translate("ReconWindow", "delete", 0, QApplication::UnicodeUTF8));
        SELECT_PTS->setText(QApplication::translate("ReconWindow", "select points", 0, QApplication::UnicodeUTF8));
        SELECT_PTS->setIconText(QApplication::translate("ReconWindow", "Draw Template On Selected Model", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SELECT_PTS->setToolTip(QApplication::translate("ReconWindow", "Draw Template On Selected Model", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        SELECT_PTS->setStatusTip(QApplication::translate("ReconWindow", "Draw Template On Selected Model", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        TEM_GENERATE->setText(QApplication::translate("ReconWindow", "generate template", 0, QApplication::UnicodeUTF8));
        TEM_GENERATE->setIconText(QApplication::translate("ReconWindow", "Generate Template", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TEM_GENERATE->setToolTip(QApplication::translate("ReconWindow", "Generate Template", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TEM_GENERATE->setStatusTip(QApplication::translate("ReconWindow", "Generate Template", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        IMPORT_AXIS->setText(QApplication::translate("ReconWindow", "Import Axis", 0, QApplication::UnicodeUTF8));
        IMPORT_AXIS->setIconText(QApplication::translate("ReconWindow", "Draw Punching Position", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        IMPORT_AXIS->setToolTip(QApplication::translate("ReconWindow", "Draw Punching Position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        IMPORT_AXIS->setStatusTip(QApplication::translate("ReconWindow", "Draw Punching Position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        PUNCH->setText(QApplication::translate("ReconWindow", "punching", 0, QApplication::UnicodeUTF8));
        PUNCH->setIconText(QApplication::translate("ReconWindow", "Punching", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        PUNCH->setToolTip(QApplication::translate("ReconWindow", "Punching", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        PUNCH->setStatusTip(QApplication::translate("ReconWindow", "Punching", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        INFORMATION->setText(QApplication::translate("ReconWindow", "Information", 0, QApplication::UnicodeUTF8));
        INFORMATION->setIconText(QApplication::translate("ReconWindow", "Project Information", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        INFORMATION->setToolTip(QApplication::translate("ReconWindow", "Project Information", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        INFORMATION->setStatusTip(QApplication::translate("ReconWindow", "Project Information", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        CLOSE_CONTOURS->setText(QApplication::translate("ReconWindow", "close contours", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        CLOSE_CONTOURS->setToolTip(QApplication::translate("ReconWindow", "close contours", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        DELETE_PTS->setText(QApplication::translate("ReconWindow", "delete selected points", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DELETE_PTS->setToolTip(QApplication::translate("ReconWindow", "delete selected points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UNDO_PUNCHING->setText(QApplication::translate("ReconWindow", "undo punching", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        UNDO_PUNCHING->setToolTip(QApplication::translate("ReconWindow", "undo punching", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        settingCButton->setText(QApplication::translate("ReconWindow", "Setting", 0, QApplication::UnicodeUTF8));
        widget_panel->setStyleSheet(QApplication::translate("ReconWindow", "background-color: rgb(191,219,255);", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ReconWindow", "Max TH", 0, QApplication::UnicodeUTF8));
        spinBox_threshold_max->setSuffix(QString());
        label_2->setText(QApplication::translate("ReconWindow", "Level", 0, QApplication::UnicodeUTF8));
        spinBox_width->setSuffix(QString());
        label_3->setText(QApplication::translate("ReconWindow", "Min TH", 0, QApplication::UnicodeUTF8));
        spinBox_threshold_min->setSuffix(QString());
        label->setText(QApplication::translate("ReconWindow", "Window", 0, QApplication::UnicodeUTF8));
        spinBox_center->setSuffix(QString());
        modelCButton->setText(QApplication::translate("ReconWindow", "Model", 0, QApplication::UnicodeUTF8));
        widget_panel_3->setStyleSheet(QApplication::translate("ReconWindow", "background-color: rgb(191,219,255);", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = modelTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ReconWindow", "Visib.", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = modelTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ReconWindow", "Color", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = modelTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ReconWindow", "Opacity", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        delModelButton->setToolTip(QApplication::translate("ReconWindow", "Delete A Selected Model", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        delModelButton->setStatusTip(QApplication::translate("ReconWindow", "Delete A Selected Model", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        delModelButton->setText(QApplication::translate("ReconWindow", "Delete", 0, QApplication::UnicodeUTF8));
        measureCButton->setText(QApplication::translate("ReconWindow", "Measurement", 0, QApplication::UnicodeUTF8));
        widget_panel_2->setStyleSheet(QApplication::translate("ReconWindow", "background-color: rgb(191,219,255);", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = measureTable->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("ReconWindow", "Visib.", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = measureTable->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("ReconWindow", "Value", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        delMeasureButton->setToolTip(QApplication::translate("ReconWindow", "Delete A Selected Measurement", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        delMeasureButton->setStatusTip(QApplication::translate("ReconWindow", "Delete A Selected Measurement", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        delMeasureButton->setText(QApplication::translate("ReconWindow", "Delete", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("ReconWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar->setStyleSheet(QApplication::translate("ReconWindow", "background-color: rgb(191,219,255);", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReconWindow: public Ui_ReconWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECONWINDOW_H
