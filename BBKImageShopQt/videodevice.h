#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <QObject>
#include<highgui.h>
#include<cv.h>
#include<QImage>
#include<malloc.h>
class VideoDevice : public QObject
{
    Q_OBJECT
private:
    CvCapture *cp;
    IplImage *frame;
    int dev;
public:
    explicit VideoDevice(QObject *parent = 0,int dev=0);
    bool OpenDev();
    bool CloseDev();
    QImage GetFrame();
    
signals:
    
public slots:
    
};

#endif // VIDEODEVICE_H
