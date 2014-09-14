#include <QtGui>
#include <QMouseEvent>
#include "showimage.h"

ShowImage::ShowImage(QWidget *parent)
    :QWidget(parent),wscale(1.0),hscale(1.0),m_zoomFactor(1.0f),m_tile( QPixmap(16, 16) ),img_width(0),img_height(0),m_color(QColor(255,255,255)),m_IsAsistance(0)
{
	m_tile.fill(Qt::white);
	QPainter pt(&m_tile);
	QColor color(202, 202, 202);
	//QColor color(0, 0, 0);
	pt.fillRect(0, 0, 8, 8, color);
	pt.fillRect(8, 8, 8, 8, color);

	pt.end();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    init_path(QDir("C:/"),0);

    setBackgroundRole(QPalette::Background);//系统背景

    setAutoFillBackground(true);//自动填充背景
    FirstTimeShowFlag = true;

	setMouseTracking(false);
	//先注册自己的MsgHandler  
	//qInstallMsgHandler(customMessageHandler);  

	// 初始化m_point
	m_point.setX(-1);
	m_point.setY(-1);
	
	gArray.clear();
	gShowArray.clear();
	gBoudary.clear();
}
void ShowImage::ColorTurn(QColor &color,QColor &dst_color)
{
	if(color.red() == 255)
	{
		dst_color.setRed(0);
	}
	else
	{
		dst_color.setRed(255);
	}
	if (color.blue() == 255)
	{
		dst_color.setBlue(0);
	}
	else
	{
		dst_color.setBlue(255);
	}
	if (color.green() == 255)
	{
		dst_color.setGreen(0);
	}
	else
	{
		dst_color.setGreen(255);
	}
}
void ShowImage::drawFlag(const QPoint & pos,float zoomFactor)
{
	m_point.setX(pos.x());
	m_point.setY(pos.y());
	m_zoomFactor = zoomFactor;
	update();
}
// 0 increase
// 1 degrese
void ShowImage::insertArray(const QPoint & pos,int flag)
{
	QPoint pt;
	pt.setX(pos.x());
	pt.setY(pos.y());
	
	if(m_IsAsistance)
	{
		if (flag == 0)
		{
			gBoudary.push_back(pt);
		}
	}
	else
	{
		
		if (flag == 1)
		{
			// -
			gArray.pop_back();
		}
		else if(flag == 0)
		{
			// +
			gArray.push_back(pt);
		}
	}
	update();
}
void ShowImage::insertShowArray(const QPoint & pos,int flag)
{
	QPoint pt;
	pt.setX(pos.x());
	pt.setY(pos.y());
	if(m_IsAsistance)
	{
		if (flag == 0)
		{
			gBoudary.push_back(pt);
		}
	}
	else
	{
		if (flag == 1)
		{
			// -
			//gShowArray.pop_back();
		}
		else if(flag == 0)
		{
			// +
			//gShowArray.push_back(pt);
		}
	}
	update();
}
QSize ShowImage::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize ShowImage::sizeHint() const
{
    return QSize(640, 480);
}



void ShowImage::init_path(QDir newdir, QString file)
{
    filepath = QDir(newdir);
    QStringList filters;
    filters<<"*.png"<<"*.bmp"<<"*.jpg"<<"*.gif";
    filepath.setNameFilters(filters);//文件类型过滤

    //设置过滤器
    filepath.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    filepath.setSorting(QDir::Size | QDir::Reversed);//设置排序顺序:按大小，反向排序
    list = filepath.entryList();//返回一个列表目录中的所有文件和目录的名称

    if (!list.empty())
    {
        emit get2beg(false);
        emit get2end(false);
    }
    else//列表不为空，开始显示
    {
        emit get2beg(true);
        emit get2end(true);
    }
    //pixmap = QPixmap(file);
	pixmap = QImage(file);
    QStringList::const_iterator itmp = list.begin();
    for (; itmp != list.end(); ++itmp)
        if (filepath.absoluteFilePath((*itmp)) == file)
        {//absoluteFilePath函数,返回目录中的文件的绝对路径名
            iterator = itmp;
            break;
        }
    FirstTimeShowFlag = true;
    update();//重绘图形
	
}

void ShowImage::imread(QString file)
{
	pixmap = QImage(file);
	update();
}

void ShowImage::setImage(const QString &image)
{
	if ( !image.isEmpty() )
	{
		pixmap = QImage( image );

		setMinimumSize( (int)(pixmap.width() * m_zoomFactor),(int) (pixmap.height() * m_zoomFactor) );
		update();

	}
}


void ShowImage::setImage(const QImage &image)
{
	if ( !image.isNull() )
	{
		pixmap = QImage( image );
		int w = pixmap.width() * m_zoomFactor;
		int h = pixmap.height() * m_zoomFactor;

		setMinimumSize( w, h );
		update();
	}
}

void ShowImage::setZoomFactor( float f )
{
	int w, h;

	if( f == m_zoomFactor || pixmap.isNull() )
		return;

	m_zoomFactor = f;

	w = (int)( pixmap.width() * m_zoomFactor);
	h = (int)(pixmap.height() * m_zoomFactor);

	// set the size of the widget
	setMinimumSize( w, h );
	update();
}

int ShowImage::getImageWidth()
{
	if (pixmap.width()>=0 && pixmap.height()>=0)
	{
		return pixmap.width();
	}
	return 0;
}

int ShowImage::getImageHeight()
{
	if (pixmap.width()>=0 && pixmap.height()>=0)
	{
		return pixmap.height();
	}
	return 0;
}

int ShowImage::getSum()
{
	return gArray.size();
}

//利用QPainter绘图
void ShowImage::paintEvent(QPaintEvent * event)
{
	int xoffset = 0, yoffset = 0;
    QPainter painter;

	painter.begin(this);
	painter.setClipRect( event->rect() );

	QPainterPath clipPath;

	QRect r = rect();
	qreal left = r.x() + 1;
	qreal top = r.y() + 1;
	qreal right = r.right();
	qreal bottom = r.bottom();
	qreal radius2 = 8 * 2;

	clipPath.moveTo(right - radius2, top);
	clipPath.arcTo(right - radius2, top, radius2, radius2, 90, -90);
	clipPath.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
	clipPath.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
	clipPath.arcTo(left, top, radius2, radius2, 180, -90);
	clipPath.closeSubpath();

	painter.setClipPath(clipPath, Qt::IntersectClip);

	painter.drawTiledPixmap(rect(), m_tile);

	if ( pixmap.isNull() )
	{
		painter.end();
		return;
	}
	// client painting
    int pw = pixmap.width();
    int ph = pixmap.height();
    int ww = width();
    int wh = height();

    xoffset = (int) (ww - pw * m_zoomFactor) / 2;

    yoffset = (int)(wh - ph * m_zoomFactor) / 2;

    painter.translate( xoffset, yoffset );
    painter.scale(m_zoomFactor, m_zoomFactor);

    painter.drawImage(QPointF(0.0f, 0.0f), pixmap);

    // trace contour rouge
    painter.setPen( QPen(m_color/*QColor(255, 0, 0, 255)*/, 1.0 / m_zoomFactor, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin) );
    painter.setBrush(Qt::NoBrush);
    painter.drawRect( QRect( 0.0f, 0.0f, pixmap.width(), pixmap.height() ).adjusted(-2, -2, 2, 2) );
	QColor showColor;
	ColorTurn(m_color,showColor);
	////// add by last_star
	painter.setRenderHint(QPainter::Antialiasing,true);
	// 绘制点击坐标队列
	for (int i = 0; i < gArray.size(); i ++)
	{
		painter.drawPoint(gArray[i]);
		painter.drawEllipse(gArray[i],(int)(4),(int)(8));
		painter.drawEllipse(gArray[i],(int)(5),(int)(8));
	}
	painter.setPen( QPen(showColor/*QColor(255, 0, 0, 255)*/, 1.0 / m_zoomFactor, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin) );
	for (int i = 0; i < gShowArray.size();i++)
	{
		painter.drawPoint(gShowArray[i]);
		painter.drawEllipse(gShowArray[i],(int)(4),(int)(8));
		painter.drawEllipse(gShowArray[i],(int)(5),(int)(8));
	}
	painter.setPen( QPen(QColor(0, 0, 0, 255), 1.0 / m_zoomFactor, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin) );
	painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
	for (int i = 0; i < gBoudary.size();i++)
	{
		
		painter.drawPoint(gBoudary[i]);
		//painter.drawEllipse(gBoudary[i],5,5);
		painter.drawEllipse(gBoudary[i],4,4);
		//painter.drawEllipse(gBoudary[i],3,3);
		//painter.drawEllipse(gBoudary[i],2,2);
		if (i>0)
		{
			painter.drawLine(gBoudary[i-1],gBoudary[i]);
		}
	}
	

	painter.setPen(QPen(Qt::black,3,Qt::DashDotDotLine,Qt::RoundCap));
	painter.setBrush(Qt::NoBrush);

	painter.drawLine(QPoint(m_point.x()-10,m_point.y()),QPoint(m_point.x()-1,m_point.y()));
	painter.drawLine(QPoint(m_point.x()+10,m_point.y()),QPoint(m_point.x()+1,m_point.y()));
	painter.drawLine(QPoint(m_point.x(),m_point.y()-10),QPoint(m_point.x(),m_point.y()-1));
	painter.drawLine(QPoint(m_point.x(),m_point.y()+10),QPoint(m_point.x(),m_point.y()+1));
    painter.end();
}

void ShowImage::mouseDoubleClickEvent(QMouseEvent *)
{
	//mainStaticMatch();
}

