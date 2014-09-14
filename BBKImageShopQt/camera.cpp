#include "camera.h"
#include "ui_camera.h"
#include <QDebug>
#include <QtGui>

Camera::Camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    vd = NULL;

    connect(ui->btn_Start,SIGNAL(clicked()),this,SLOT(slotStart()));
    connect(ui->btn_Catch1,SIGNAL(clicked()),this,SLOT(slotCatch()));
    connect(ui->btn_Catch2,SIGNAL(clicked()),this,SLOT(slotSave()));
	connect(ui->btn_Continue,SIGNAL(clicked()),this,SLOT(slotRun()));
    connect(timer,SIGNAL(timeout()),this,SLOT(slotUpdata()));

    ui->btn_Catch1->setEnabled(false);
    ui->btn_Catch2->setEnabled(false);
	setWindowIcon(QIcon(":/icons/BbkImageShop.ico"));//可执行程序图标
}

Camera::~Camera()
{
    vd->CloseDev();
    delete ui;
}
void Camera::slotStart()
{
    if (ui->btn_Start->text()=="Start")
    {
		// 内外摄像头选择
        if (ui->rbn_Out->isChecked())
            dev=0;
        else 
			dev=1;
        vd = new VideoDevice(this,dev);
        if(!vd->OpenDev())
		{
			// 打开摄像头失败
			delete vd;
			QMessageBox msgBox;
			msgBox.setText(tr("警告: 没有发现摄像头设备"));
			msgBox.exec();
			return ;
		}
        timer->start(33);
        ui->btn_Start->setText("Stop");
        ui->btn_Catch1->setEnabled(true);
		ui->btn_Catch2->setEnabled(false);
    }
    else {
        ui->btn_Start->setText("Start");
        ui->btn_Catch1->setEnabled(false);
        ui->btn_Catch2->setEnabled(false);
        timer->stop();
        vd->CloseDev();
        ui->lbl_Image->clear();
        }
}
void Camera::slotUpdata()
{
    frame=vd->GetFrame();
	// 缩放到label尺寸
	ui->lbl_Image->setPixmap(QPixmap::fromImage(frame.scaled(/*size()*/ui->lbl_Image->size(), Qt::IgnoreAspectRatio, 
	Qt::SmoothTransformation)));
	
}
void Camera::slotCatch()
{
	timer->stop();

    ui->btn_Catch1->setEnabled(true);
    ui->btn_Catch2->setEnabled(true);
	m_pic = vd->GetFrame();
	timer->start();
	emit captured1(1);
}

void Camera::slotContinue()
{
    timer->start();
}
void Camera::slotSave()
{
	timer->stop();
	//ui->btn_Continue->setEnabled(true);
	ui->btn_Catch1->setEnabled(true);
	m_pic = vd->GetFrame();
	
	timer->start();
	
	emit captured2(2);

	
}

void Camera::slotRun()
{
	this->hide();
	emit run();
}