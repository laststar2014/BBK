/********************************************************************************
** Form generated from reading UI file 'camera.ui'
**
** Created: Sun Jun 8 12:52:08 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERA_H
#define UI_CAMERA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Camera
{
public:
    QLabel *lbl_Image;
    QPushButton *btn_Start;
    QPushButton *btn_Catch;
    QPushButton *btn_Continue;
    QPushButton *btn_Save;
    QRadioButton *rbn_In;
    QRadioButton *rbn_Out;

    void setupUi(QWidget *Camera)
    {
        if (Camera->objectName().isEmpty())
            Camera->setObjectName(QString::fromUtf8("Camera"));
        Camera->resize(701, 594);
        lbl_Image = new QLabel(Camera);
        lbl_Image->setObjectName(QString::fromUtf8("lbl_Image"));
        lbl_Image->setGeometry(QRect(30, 20, 621, 431));
        btn_Start = new QPushButton(Camera);
        btn_Start->setObjectName(QString::fromUtf8("btn_Start"));
        btn_Start->setGeometry(QRect(60, 522, 81, 41));
        btn_Catch = new QPushButton(Camera);
        btn_Catch->setObjectName(QString::fromUtf8("btn_Catch"));
        btn_Catch->setGeometry(QRect(220, 520, 81, 41));
        btn_Continue = new QPushButton(Camera);
        btn_Continue->setObjectName(QString::fromUtf8("btn_Continue"));
        btn_Continue->setGeometry(QRect(530, 520, 81, 41));
        btn_Save = new QPushButton(Camera);
        btn_Save->setObjectName(QString::fromUtf8("btn_Save"));
        btn_Save->setGeometry(QRect(380, 520, 81, 41));
        rbn_In = new QRadioButton(Camera);
        rbn_In->setObjectName(QString::fromUtf8("rbn_In"));
        rbn_In->setGeometry(QRect(170, 480, 81, 31));
        rbn_Out = new QRadioButton(Camera);
        rbn_Out->setObjectName(QString::fromUtf8("rbn_Out"));
        rbn_Out->setGeometry(QRect(420, 470, 61, 41));

        retranslateUi(Camera);

        QMetaObject::connectSlotsByName(Camera);
    } // setupUi

    void retranslateUi(QWidget *Camera)
    {
        Camera->setWindowTitle(QApplication::translate("Camera", "Camera", 0, QApplication::UnicodeUTF8));
        lbl_Image->setText(QString());
        btn_Start->setText(QApplication::translate("Camera", "Start", 0, QApplication::UnicodeUTF8));
        btn_Catch->setText(QApplication::translate("Camera", "Catch", 0, QApplication::UnicodeUTF8));
        btn_Continue->setText(QApplication::translate("Camera", "Continue", 0, QApplication::UnicodeUTF8));
        btn_Save->setText(QApplication::translate("Camera", "Save", 0, QApplication::UnicodeUTF8));
        rbn_In->setText(QApplication::translate("Camera", "In", 0, QApplication::UnicodeUTF8));
        rbn_Out->setText(QApplication::translate("Camera", "Out", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Camera: public Ui_Camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
