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
    QPushButton *btn_Catch1;
	QPushButton *btn_Catch2;
    QPushButton *btn_Continue;
    QRadioButton *rbn_In;
    QRadioButton *rbn_Out;

    void setupUi(QWidget *Camera)
    {
        if (Camera->objectName().isEmpty())
            Camera->setObjectName(QString::fromUtf8("Camera"));
        Camera->resize(600, 480);
        lbl_Image = new QLabel(Camera);
        lbl_Image->setObjectName(QString::fromUtf8("lbl_Image"));
        lbl_Image->setGeometry(QRect(30, 20, 570, 390));
        btn_Start = new QPushButton(Camera);
        btn_Start->setObjectName(QString::fromUtf8("Start"));
        btn_Start->setGeometry(QRect(40, 420, 70, 41));
        btn_Catch1 = new QPushButton(Camera);
        btn_Catch1->setObjectName(QString::fromUtf8("btn_Catch1"));
        btn_Catch1->setGeometry(QRect(150+40, 420, 70, 41));
        btn_Catch2 = new QPushButton(Camera);
        btn_Catch2->setObjectName(QString::fromUtf8("btn_Catch2"));
        btn_Catch2->setGeometry(QRect(300+40, 420, 70, 41));
		btn_Continue = new QPushButton(Camera);
		btn_Continue->setObjectName(QString::fromUtf8("btn_Continue"));
		btn_Continue->setGeometry(QRect(450+40, 420, 70, 41));

        rbn_In = new QRadioButton(Camera);
        rbn_In->setObjectName(QString::fromUtf8("rbn_In"));
        rbn_In->setGeometry(QRect(150, 405, 100, 15));
        rbn_Out = new QRadioButton(Camera);
        rbn_Out->setObjectName(QString::fromUtf8("rbn_Out"));
        rbn_Out->setGeometry(QRect(350, 405, 100, 15));

        retranslateUi(Camera);

        QMetaObject::connectSlotsByName(Camera);
    } // setupUi

    void retranslateUi(QWidget *Camera)
    {
        Camera->setWindowTitle(QApplication::translate("Camera", "Camera", 0, QApplication::UnicodeUTF8));
        lbl_Image->setText(QString());
        btn_Start->setText(QApplication::translate("Camera", "Start", 0, QApplication::UnicodeUTF8));
        btn_Catch1->setText(QApplication::translate("Camera", "first", 0, QApplication::UnicodeUTF8));
        btn_Continue->setText(QApplication::translate("Camera", "Run", 0, QApplication::UnicodeUTF8));
        btn_Catch2->setText(QApplication::translate("Camera", "second", 0, QApplication::UnicodeUTF8));
        rbn_In->setText(QApplication::translate("Camera", "In", 0, QApplication::UnicodeUTF8));
        rbn_Out->setText(QApplication::translate("Camera", "Out", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Camera: public Ui_Camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
