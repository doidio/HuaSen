/********************************************************************************
** Form generated from reading UI file 'Password.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORD_H
#define UI_PASSWORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogPassword
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLineEdit *lineEdit_ApplyingID;
    QLabel *label_8;
    QLineEdit *lineEdit_Password;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogPassword)
    {
        if (DialogPassword->objectName().isEmpty())
            DialogPassword->setObjectName(QString::fromUtf8("DialogPassword"));
        DialogPassword->resize(353, 111);
        gridLayout_2 = new QGridLayout(DialogPassword);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_6 = new QLabel(DialogPassword);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        lineEdit_ApplyingID = new QLineEdit(DialogPassword);
        lineEdit_ApplyingID->setObjectName(QString::fromUtf8("lineEdit_ApplyingID"));
        lineEdit_ApplyingID->setEnabled(true);
        lineEdit_ApplyingID->setStyleSheet(QString::fromUtf8("background-color: rgb(218, 218, 218);"));
        lineEdit_ApplyingID->setReadOnly(true);

        gridLayout->addWidget(lineEdit_ApplyingID, 0, 1, 1, 2);

        label_8 = new QLabel(DialogPassword);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_8, 1, 0, 1, 1);

        lineEdit_Password = new QLineEdit(DialogPassword);
        lineEdit_Password->setObjectName(QString::fromUtf8("lineEdit_Password"));

        gridLayout->addWidget(lineEdit_Password, 1, 1, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(DialogPassword);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(DialogPassword);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPassword, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPassword, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogPassword);
    } // setupUi

    void retranslateUi(QDialog *DialogPassword)
    {
        DialogPassword->setWindowTitle(QApplication::translate("DialogPassword", "Password", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogPassword", "\347\263\273\345\210\227\345\217\267", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DialogPassword", "\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPassword: public Ui_DialogPassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORD_H
