#include "videodevice.h"
#include<qDebug>
VideoDevice::VideoDevice(QObject *parent,int dev) :
    QObject(parent)
{
    cp=NULL;
    this->dev=dev;
}
bool VideoDevice::OpenDev()
{
    cp=cvCreateCameraCapture(dev);
	if (cp == NULL)
	{
		return false;
	}
	cvSetCaptureProperty(cp,CV_CAP_PROP_FRAME_WIDTH , 1620);
	cvSetCaptureProperty(cp,CV_CAP_PROP_FRAME_HEIGHT, 1240);
    return true;
}
bool VideoDevice::CloseDev()
{
    cvReleaseCapture(&cp);
    return true;
}
QImage VideoDevice::GetFrame()
{
    frame=cvQueryFrame(cp);
    cvCvtColor(frame,frame,CV_BGR2RGB);
    QImage image((const uchar *)frame->imageData,frame->width,frame->height,QImage::Format_RGB888);
    return image;
}
