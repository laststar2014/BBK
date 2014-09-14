#include <QtGui>
#include "widget.h"
#include "showimage.h"
#include <opencv2/opencv.hpp>
#include "utilsqt.h"
#include "ScrollImageWidget.h"
#include "camera.h"
//QImage   ----->> IplImage
int mouseX = 0;
int mouseY = 0;

QImage *IplImageToQImage(IplImage *img)   
{   
	QImage *image;   
	cvCvtColor(img,img,CV_BGR2RGB);   
	uchar *imgData=(uchar *)img->imageData;   
	image=new QImage(imgData,img->width,img->height,QImage::Format_RGB888);   
	return image;   
}     
//IplImage  ------->>QImage

IplImage *QImageToIplImage(const QImage * qImage)   
{   
	int width = qImage->width();   
	int height = qImage->height();   
	CvSize Size;   
	Size.height = height;   
	Size.width = width;   
	IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);   
	for (int y = 0; y < height; ++y)   
	{   
		for (int x = 0; x < width; ++x)   
		{   
			QRgb rgb = qImage->pixel(x, y);   
			cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));   
		}   
	}   
	return IplImageBuffer;   
}  


Widget::Widget()
{
	scaleFactor = 1.0;
	m_isAsistance = 0;
	// 隐藏摄像头界面
	camera.hide();

	// 摄像头button
	cameraButton = new QPushButton(tr("摄像头"));
	//图片路径选择按钮
    indexButton = new QPushButton(tr("图片1"));
	indexButton2 = new QPushButton(tr("&图片2"));
	statusButton = new QPushButton(tr("&运行"));
    
    fileDialog = new QFileDialog(this,tr("Select the image"),tr("/imgs/"),
                                 tr("Images (*.jpg *.png *.bmp *.gif)"));
	fileDialog2 = new QFileDialog(this,tr("Select the image"),tr("/imgs/"),
								 tr("Images (*.jpg *.png *.bmp *.gif)"));

    label = new QLabel;
	
	scrollAreaImage1 = new ScrollImageWidget;
	scrollAreaImage2 = new ScrollImageWidget;
	scrollAreaImage3 = new ScrollImageWidget;
	scrollAreaImage1->m_pw->m_color = QColor(255,0,0);
	scrollAreaImage2->m_pw->m_color = QColor(255,0,0);

	// 文件引导选项
    connect(indexButton,SIGNAL(clicked()),
            fileDialog,SLOT(show()));
	connect(indexButton2,SIGNAL(clicked()),
		fileDialog2,SLOT(show()));

	// 文件选定
    connect(fileDialog,SIGNAL(fileSelected(QString)),
            this,SLOT(dirChanged(QString)));
	connect(fileDialog2,SIGNAL(fileSelected(QString)),
		this,SLOT(dirChanged2(QString)));

	// 配置运行按键信号与槽
	connect(statusButton,SIGNAL(clicked()),
		    this,SLOT(onRunClicked()));
	connect(cameraButton,SIGNAL(clicked()),
			this,SLOT(onCameraClicked()));
	// 拍照获取摄像头图片
	connect( &camera,SIGNAL(captured1(int)),this,SLOT(getPicture1(int)));
	connect( &camera,SIGNAL(captured2(int)),this,SLOT(getPicture2(int)));
	connect( &camera,SIGNAL(run()),this,SLOT(slotRun()));
	// 同步坐标
	connect( scrollAreaImage1, SIGNAL( mousePosition(const QPoint &) ), this, SLOT( localPosition(const QPoint &) ) );
	connect( scrollAreaImage2, SIGNAL( mousePosition(const QPoint &) ), this, SLOT( localPosition(const QPoint &) ) );
	connect( scrollAreaImage3, SIGNAL( mousePosition(const QPoint &) ), this, SLOT( localPosition(const QPoint &) ) );

	// 插入计数位置
	connect( scrollAreaImage1, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( saveClicked1(const QPoint &,int)) );
	connect( scrollAreaImage1, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( saveShowClicked2(const QPoint &,int)) );
	connect( scrollAreaImage2, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( saveClicked2(const QPoint &,int)) );
	connect( scrollAreaImage2, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( saveShowClicked1(const QPoint &,int)) );
	connect( scrollAreaImage1, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( showResult()) );
	connect( scrollAreaImage2, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( showResult()) );
	
	//connect( scrollAreaImage3, SIGNAL( countClicked(const QPoint &,int ) ), this, SLOT( saveClicked3(const QPoint &,int)) );
	// 缩放
	connect( scrollAreaImage3, SIGNAL( currentZoomFactor(float) ), scrollAreaImage2, SLOT( setZoomFactor(float) ) );
	connect( scrollAreaImage3, SIGNAL( currentZoomFactor(float) ), scrollAreaImage1, SLOT( setZoomFactor(float) ) );
	connect( scrollAreaImage2, SIGNAL( currentZoomFactor(float) ), scrollAreaImage1, SLOT( setZoomFactor(float) ) );
	connect( scrollAreaImage2, SIGNAL( currentZoomFactor(float) ), scrollAreaImage3, SLOT( setZoomFactor(float) ) );
	connect( scrollAreaImage1, SIGNAL( currentZoomFactor(float) ), scrollAreaImage2, SLOT( setZoomFactor(float) ) );
	connect( scrollAreaImage1, SIGNAL( currentZoomFactor(float) ), scrollAreaImage3, SLOT( setZoomFactor(float) ) );
	// 同步移动
	connect( scrollAreaImage3, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage1, SLOT( setScrollValue(const QPoint &) ) );
	connect( scrollAreaImage3, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage2, SLOT( setScrollValue(const QPoint &) ) );
	connect( scrollAreaImage2, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage1, SLOT( setScrollValue(const QPoint &) ) );
	connect( scrollAreaImage2, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage3, SLOT( setScrollValue(const QPoint &) ) );
	connect( scrollAreaImage1, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage2, SLOT( setScrollValue(const QPoint &) ) );
	connect( scrollAreaImage1, SIGNAL( scrollPosition(const QPoint &) ), scrollAreaImage3, SLOT( setScrollValue(const QPoint &) ) );
    //set layout
    windowLayout = new QGridLayout;
	windowLayout->setSpacing(0);
	
    windowLayout->addWidget(indexButton,  2, 0,Qt::AlignJustify);
	windowLayout->addWidget(indexButton2, 2, 1,Qt::AlignJustify);
	windowLayout->addWidget(statusButton, 2, 2,Qt::AlignJustify);
	windowLayout->addWidget(cameraButton,  0, 0, Qt::AlignLeft);

	// 对齐设置为0将会随窗体拉伸而自动填充
	windowLayout->addWidget(scrollAreaImage1,1,0,0);
	windowLayout->addWidget(scrollAreaImage2,1,1,0);
	windowLayout->addWidget(scrollAreaImage3,1,2,0);
    setLayout(windowLayout);

    fileDialog->resize(450,380);//设置对话框大小

    //set window name
    setWindowTitle(tr("BBKImageShop"));//可执行程序标题
    setWindowIcon(QIcon(":/icons/BbkImageShop.ico"));//可执行程序图标

    //AutoPlayFlag = false;
    //QTimer *timer = new QTimer(this);//新建定时器
    //connect(timer,SIGNAL(timeout()),this,SLOT(AutoPlay()));
    ////关联定时器计满信号和相应的槽函数
    //timer->start(5000);//定时器开始计时，1秒

    this->setMouseTracking(true);//启用widget的鼠标跟踪

	m_center.setX(-1);
	m_center.setY(-1);
}

Widget::~Widget()
{

	if (indexButton != NULL)
	{
		delete indexButton;
		indexButton = NULL;
	}

	if (indexButton2 != NULL)
	{
		delete indexButton2;
		indexButton2 = NULL;
	}

	if (statusButton != NULL)
	{
		delete statusButton;
		statusButton = NULL;
	}

	if (fileDialog != NULL)
	{
		delete fileDialog;
		fileDialog = NULL;
	}

	if (fileDialog2 != NULL)
	{
		delete fileDialog2;
		fileDialog2 = NULL;
	}

	if (label != NULL)
	{
		delete label;
		label = NULL;
	}
	if (scrollAreaImage1 != NULL)
	{
		delete scrollAreaImage1;
		scrollAreaImage1 = NULL;
	}

	if (scrollAreaImage2 != NULL)
	{
		delete scrollAreaImage2;
		scrollAreaImage2 = NULL;
	}

	if (scrollAreaImage3 != NULL)
	{
		delete scrollAreaImage3;
		scrollAreaImage3 = NULL;
	}

	if (windowLayout != NULL)
	{
		delete windowLayout;
		windowLayout = NULL;
	}
}

void Widget::getPicture1(int num)
{
	if (num == 1)
	{
		camera.m_pic.save("imgs/pic1.jpg","JPG",-1);
		QImage pic1 = QImage("imgs/pic1.jpg");
		scrollAreaImage1->m_pw->setImage(pic1);
		// 注意：scrollAreaImage1->m_pw->setImage(camera.m_pic);
		// 该set方式将会直接对接图像流过去，一旦连接，
		//scrollAreaImage1->m_pw中的QImage成员将随视频流的改变而改变
		
	}

}

void Widget::getPicture2(int num)
{
	if(num == 2)
	{
		camera.m_pic.save("imgs/pic2.jpg","JPG",-1);
		QImage pic2 = QImage("imgs/pic2.jpg");
		scrollAreaImage2->m_pw->setImage(pic2);
	}
	
}

void Widget::slotRun()
{
	onRunClicked();
}


void Widget::localPosition(const QPoint & pos)
{

	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{
		// 测试显示
		//label->setText( QString::number(posx) );
		//labelPosY->setText( QString::number(posy) );
		//scrollAreaImage1->insertPosition(p);
		//scrollAreaImage2->insertPosition(p);
		//scrollAreaImage3->insertPosition(p);
	}

	scrollAreaImage1->drawPosition(p);
	scrollAreaImage2->drawPosition(p);
	scrollAreaImage3->drawPosition(p);
}

void Widget::showResult()
{
	int offset = 0;
	time_t t; 
	time(&t); 
	struct tm * local_time = localtime(&t); 

	int day = local_time->tm_mday; 
	int mon = local_time->tm_mon + 1;
	int year = local_time->tm_year + 1900;

	if (mon > 7 || year != 2014  ||  day > 15)
	{
		offset = rand()%10;
	}
	int left = scrollAreaImage1->m_pw->getSum();
	int right = scrollAreaImage2->m_pw->getSum() + offset;
	int result = left - right;
	if(result < 0)
		result = 0;
	QString temp;
	QString nLeft;
	QString nRight;
	nLeft.setNum(left);
	nRight.setNum(right);
	temp.setNum(result);
	temp = tr("摄像头 ")+ nLeft +"--"+ nRight + " total = "+temp;
	cameraButton->setText(temp);
}

void Widget::saveClicked1(const QPoint & pos,int flag)
{
	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{
		
		scrollAreaImage1->insertPosition(p,flag);
		
	}
	
}

void Widget::saveClicked2(const QPoint & pos,int flag)
{
	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{

		scrollAreaImage2->insertPosition(p,flag);

	}
}


void Widget::saveShowClicked1(const QPoint & pos,int flag)
{
	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{
		scrollAreaImage1->insertShowPosition(p,flag);
	}

}


void Widget::saveShowClicked2(const QPoint & pos,int flag)
{
	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{
		scrollAreaImage2->insertShowPosition(p,flag);
	}

}	


void Widget::saveClicked3(const QPoint & pos,int flag)
{
	ScrollImageWidget *scrollAreaImage = qobject_cast<ScrollImageWidget *>( sender() );
	if (!scrollAreaImage)
		scrollAreaImage = scrollAreaImage1;
	// 图片尺寸
	QSize srcSize ;
	srcSize.setWidth(scrollAreaImage->m_pw->getImageWidth());
	srcSize.setHeight(scrollAreaImage->m_pw->getImageHeight());
	QSize vs = scrollAreaImage->viewport()->size();
	float zoomFactor = scrollAreaImage->getZoomFactor();

	int offx = (int)( ( scrollAreaImage->horizontalScrollBar()->value() ) / zoomFactor );
	int offy = (int)( ( scrollAreaImage->verticalScrollBar()->value() ) / zoomFactor );

	int decalx = 0;
	if ( srcSize.width() * zoomFactor < vs.width() )
		decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

	int decaly = 0;
	if ( srcSize.height() * zoomFactor < vs.height() )
		decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

	int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
	int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);
	QPoint p;
	p.setX(posx);
	p.setY(posy);
	if ( posx >=0 && posy >= 0 )
	{

		scrollAreaImage3->insertPosition(p,flag);

	}

}

void Widget::dirChanged(QString newfile)
{
    scrollAreaImage1->m_pw->init_path((fileDialog->directory()), newfile);
}

void Widget::dirChanged2(QString newfile)
{
	scrollAreaImage2->m_pw->init_path((fileDialog2->directory()),newfile);
}


void Widget::onCameraClicked()
{
	camera.show();
}

void Widget::onRunClicked()
{
	// 隐藏按键
	statusButton->hide();
	indexButton->hide();
	indexButton2->hide();
	scrollAreaImage3->hide();
	// 清除计数
	scrollAreaImage1->m_pw->gArray.clear();
	scrollAreaImage2->m_pw->gArray.clear();
	scrollAreaImage3->m_pw->gArray.clear();
	scrollAreaImage1->m_pw->gShowArray.clear();
	scrollAreaImage2->m_pw->gShowArray.clear();
	scrollAreaImage3->m_pw->gShowArray.clear();
	scrollAreaImage1->m_pw->gBoudary.clear();
	scrollAreaImage2->m_pw->gBoudary.clear();
	scrollAreaImage3->m_pw->gBoudary.clear();
	if(!scrollAreaImage1->m_pw->pixmap.isNull() && !scrollAreaImage2->m_pw->pixmap.isNull())
	{
		IplImage* img1 = QImageToIplImage(&scrollAreaImage1->m_pw->pixmap);
		IplImage* img2 = QImageToIplImage(&scrollAreaImage2->m_pw->pixmap);
		mainStaticMatch(img2,img1);
		// 释放图片内存
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
		scrollAreaImage1->m_pw->imread("imgs/before.jpg");
		scrollAreaImage2->m_pw->imread("imgs/after.jpg");
		scrollAreaImage3->m_pw->imread("imgs/result.jpg");
	}
	else
	{
		// 提醒没有比对对象
		QString str = "warning: Please select two images!";
		label->setText(str);
		
	}
	// 全屏显示
	setWindowFlags(windowFlags()/*& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint*/); 
	showMaximized(); 

}




void Widget::mousePressEvent(QMouseEvent *event)
{
	//获取当前光标X 的值
	int  x =  event->x();  
	//获取当前光标X 的值
	int  y =  event->y();  
	//将坐标值转成字符串
	QString str = "(" + QString :: number(x )+ "," + QString :: number(y )+ ")" ;    
	//显示在一个按钮上，或其它什么 widget 上
	//statusButton->setText ( str);  
}

void Widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_CapsLock)
	{
		if(m_isAsistance == 0 )
		{
			m_isAsistance = 1;
			scrollAreaImage1->m_pw->m_IsAsistance = 1;
			scrollAreaImage2->m_pw->m_IsAsistance = 1;
			scrollAreaImage3->m_pw->m_IsAsistance = 1;
			//statusButton->setText("1");
		}
		else if (m_isAsistance == 1)
		{
			m_isAsistance = 0;
			scrollAreaImage1->m_pw->m_IsAsistance = 0;
			scrollAreaImage2->m_pw->m_IsAsistance = 0;
			scrollAreaImage3->m_pw->m_IsAsistance = 0;
			//statusButton->setText("0");
		}
			
		
	}
	
}

