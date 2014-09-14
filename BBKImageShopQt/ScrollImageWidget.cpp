#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QWheelEvent>
#include <QtGui/QScrollBar>

#include <math.h>

#include "ScrollImageWidget.h"
#include "showimage.h"

const float DeltaZoom = 1.2f;
int gCounting = 0;
ScrollImageWidget::ScrollImageWidget( QWidget *parent ) :
    QScrollArea( parent ),
    m_moving(true),
    m_pw(NULL),
    m_zoomFactor(0.5f)
{
    setWidgetResizable( true );

    m_pw = new ShowImage( this );
    setWidget( m_pw );
    setMouseTracking(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    widget ()->setMouseTracking(true);

    connect( horizontalScrollBar(), SIGNAL( valueChanged(int) ), this, SIGNAL( horizontalScroll(int) ) );
    connect( verticalScrollBar(), SIGNAL( valueChanged(int) ), this, SIGNAL( verticalScroll(int) ) );
}

//-------------------------------------------------------------------------

ScrollImageWidget::~ScrollImageWidget()
{
    //delete pw_;
	if (m_pw != NULL)
	{
		delete m_pw;
	}
}


//-------------------------------------------------------------------------

void ScrollImageWidget::drawPosition(const QPoint & pos)
{
	// 向下传递坐标值，这里要考虑坐标转换问题
	m_pw->drawFlag(pos,m_zoomFactor);
}

void ScrollImageWidget::insertPosition(const QPoint & pos,int flag)
{
	m_pw->insertArray(pos,flag);
}
void ScrollImageWidget::insertShowPosition(const QPoint & pos,int flag)
{
	m_pw->insertShowArray(pos,flag);
}
//-------------------------------------------------------------------------
// 绘制
void ScrollImageWidget::paintEvent(QPaintEvent * e)
{
	//// add by last_star
	QPainter painter(this);

	QPixmap sp = QPixmap(50,50);  
	sp.fill(QColor(255,255,255));
	painter.drawPixmap(20, 50, sp);
}

void ScrollImageWidget::setScrollValue(const QPoint & p)
{
    horizontalScrollBar()->setValue( p.x() );
    verticalScrollBar()->setValue( p.y() );
}

//-------------------------------------------------------------------------

void ScrollImageWidget::setHorizontalScrollValue(int val)
{
    horizontalScrollBar()->setValue( val );
}

//-------------------------------------------------------------------------

void ScrollImageWidget::setVerticalScrollValue(int val)
{
    verticalScrollBar()->setValue( val );
}

//-------------------------------------------------------------------------

void ScrollImageWidget::mouseMoveEvent(QMouseEvent *event)
{

    if ( (event->buttons() & Qt::RightButton) && m_moving )
    {
        const int x = horizontalScrollBar()->value() + lastPoint.x() - event->pos().x();

        //horizontalScrollBar()->setValue( x );
        const int y = verticalScrollBar()->value() + lastPoint.y() - event->pos().y();

		int moveDist = abs(lastPoint.x() - x) + abs(lastPoint.y() - y);
		if(moveDist < 15)
			return ;
        //verticalScrollBar()->setValue( y );
        lastPoint = event->pos();
		
        setScrollValue( QPoint(x,y) );
        emit scrollPosition( QPoint(x,y) );
    }
	
    emit mousePosition( event->pos() );
}

//-------------------------------------------------------------------------
void ScrollImageWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		lastPoint = event->pos();
		pressPoint = event->pos();
		m_moving = true;
		//setCursor(Qt::OpenHandCursor);
	}
	if(event->button() == Qt::LeftButton)
	{
		emit countClicked(event->pos(),0);
	}

}
void ScrollImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int difR = abs(pressPoint.x()-event->pos().x())+abs(pressPoint.y()-event->pos().y());
    if (event->button() == Qt::RightButton && m_moving && difR > 20)
    {
        m_moving = false;
        emit somethingChanged();
		
    }

	
}

//-------------------------------------------------------------------------

void ScrollImageWidget::wheelEvent( QWheelEvent *event )
{
	float f;

    //qDebug("a wheel event is received !!! object = %x with event %x",this,event);
    if (event->delta() >= 0)
        f = m_zoomFactor * DeltaZoom;
    else
        f = m_zoomFactor / DeltaZoom;

    setZoomFactor( f );

    emit currentZoomFactor(f);
}

//-------------------------------------------------------------------------

void ScrollImageWidget::setZoomFactor( float f)
{
    //qDebug("setZoomFactor = %f on object = %x ",f,this);

    // controle du zoom max => l'image zoom閑 ne doit pas depsser 5000x5000
    if (m_pmSize.width() * f > 10000.0f)
        f = 10000.0f / m_pmSize.width();
    if (m_pmSize.height() * f > 10000.0f)
        f = 10000.0f / m_pmSize.height();

    float delta_zoom = f / m_zoomFactor;

    // if (f == m_zoomFactor)
    //    return;

    // decalage sans zoom
    float oldOffsetX = horizontalScrollBar()->value() / m_zoomFactor;
    float oldOffsetY = verticalScrollBar()->value() / m_zoomFactor;

    // disable update for flicking
    //setUpdatesEnabled(false);

    m_zoomFactor = f;

    //qDebug("m_zoomFactor = %f",m_zoomFactor);
    m_pw->setZoomFactor(m_zoomFactor);

    // nouveau decalage avec zoom
    float newOffsetX;
    float newOffsetY;

    newOffsetX = (oldOffsetX) * m_zoomFactor + (delta_zoom - 1) * viewport()->width() / 2;
    newOffsetY = (oldOffsetY) * m_zoomFactor + (delta_zoom - 1) * viewport()->height() / 2;

    // recentrage
    horizontalScrollBar()->setValue( (int)(newOffsetX ) );
    verticalScrollBar()->setValue( (int)(newOffsetY) );

    // enable redraw
    //setUpdatesEnabled(true);

    emit somethingChanged();
}

//-------------------------------------------------------------------------

void ScrollImageWidget::setImage(const QImage &image)
{
    m_pmSize = image.size();
    m_pw->setImage(image);
}

//-------------------------------------------------------------------------

QPoint ScrollImageWidget::getScrollValue()
{
    return QPoint( horizontalScrollBar()->value(),verticalScrollBar()->value() );
}

//-------------------------------------------------------------------------

QPoint ScrollImageWidget::getScrollMaximum()
{
    return QPoint( horizontalScrollBar()->maximum(),verticalScrollBar()->maximum() );
}

//-------------------------------------------------------------------------

void ScrollImageWidget::setScrollMaximum(const QPoint &p)
{
    //qDebug("setScrollMaximum = %d/%d on object = %x ",p.x(),p.y(),this);

    horizontalScrollBar()->setMaximum( p.x() );
    verticalScrollBar()->setMaximum( p.y() );
}

//-------------------------------------------------------------------------

void ScrollImageWidget::fitToWindow()
{
    if ( !m_pmSize.isValid() )
        return;

    QSize vs = viewport()->size();

    QSize imgs = m_pmSize;

    float valy = vs.height() / (float)imgs.height();
    float valx = vs.width() / (float)imgs.width();

    if (valx > valy)
    {
        setZoomFactor(valy);
    }
    else
    {
        setZoomFactor(valx);
    }
    emit currentZoomFactor(m_zoomFactor);
}

//-------------------------------------------------------------------------

QColor ScrollImageWidget::getColorAtPosition(const QPoint &pos)
{
    QSize srcSize = m_pmSize;
    QSize vs = viewport()->size();
    QColor color;

    float zoomFactor = m_zoomFactor;

    int offx = (int)( ( horizontalScrollBar()->value() ) / zoomFactor );
    int offy = (int)( ( verticalScrollBar()->value() ) / zoomFactor );

    int decalx = 0;
    int decaly = 0;

    if ( srcSize.width() * zoomFactor < vs.width() )
        decalx = (int)(vs.width() - srcSize.width() * zoomFactor) / 2;

    if ( srcSize.height() * zoomFactor < vs.height() )
        decaly = (int)(vs.height() - srcSize.height() * zoomFactor) / 2;

    int posx = (int)(pos.x() / zoomFactor + offx - decalx / zoomFactor + 0.5);
    int posy = (int)(pos.y() / zoomFactor + offy - decaly / zoomFactor + 0.5);

    if ( posx >=0 && posx < srcSize.width() && posy >= 0 && posy < srcSize.height() )
    {
        return QColor( m_pw->getImage().pixel(posx,posy) );
    }

    return color;
}
