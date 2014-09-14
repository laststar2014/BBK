#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QWidget>
#include <QString>
#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QMouseEvent>
#include <QScrollArea>
#include "camera.h"
QT_BEGIN_NAMESPACE
class QPushButton;
class QCheckBox;
class QFileDialog;
class QSlider;
class QLabel;
class QStatusBar;
class QGridLayout;
QT_END_NAMESPACE
class ShowImage;
class ScrollImageWidget;
class Camera;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget();
    ~Widget();
public:
	QPushButton *bigButton;
	QPushButton *smallButton;
	ScrollImageWidget *scrollAreaImage1;
	ScrollImageWidget *scrollAreaImage2;
	ScrollImageWidget *scrollAreaImage3;
private:
    
    double scaleFactor;

    QPushButton *indexButton;//图片路径选择按钮
	QPushButton *indexButton2;
	QPushButton *statusButton;

    QCheckBox *checkBox;
    QFileDialog *fileDialog;
	QFileDialog *fileDialog2;
	QFileDialog *fileDialog3;
    //QSlider *slider;
    QLabel *label;
	QPushButton * cameraButton;
	QGridLayout * windowLayout;
    bool AutoPlayFlag;

    //QLabel *label1;
	QPoint m_center;
	Camera camera;
	int m_isAsistance;

private slots:
    void dirChanged(QString);
	void dirChanged2(QString);
    //void changelabel(int);
	void onRunClicked();
	void onCameraClicked();
	void localPosition(const QPoint &);
	void saveClicked1(const QPoint &,int);
	void saveClicked2(const QPoint &,int);
	void saveClicked3(const QPoint &,int);
	void saveShowClicked1(const QPoint & pos,int flag);
	void saveShowClicked2(const QPoint & pos,int flag);
	void showResult();
	void getPicture1(int);
	void getPicture2(int);
	void slotRun();
protected:
	void mousePressEvent(QMouseEvent *event); // 鼠标点击回调函数
	void keyPressEvent(QKeyEvent *event);
	
};

#endif // WIDGET_H
