#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include "videodevice.h"
#include <QTimer>
#include <QImage>
#include <QFileDialog>

namespace Ui {
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = 0);
    ~Camera();
signals:
	void captured1(int);   // едуупе╨е
	void captured2(int);   // едуупе╨е
	void run();
private:
    Ui::Camera *ui;
    VideoDevice *vd;
    QTimer *timer;
    QImage frame;
    int dev;
private slots:
    void slotStart();
    void slotCatch();
    void slotContinue();
    void slotSave();
    void slotUpdata();
	void slotRun();
public:
	QImage m_pic;
};

#endif // CAMERA_H
