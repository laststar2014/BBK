#ifndef _SCROLLIMAGEWIDGET_H_
#define _SCROLLIMAGEWIDGET_H_

#include <QtCore/QString>
#include <QtGui/QScrollArea>

class ShowImage;

class ScrollImageWidget :
    public QScrollArea
{
    Q_OBJECT

public:

    ScrollImageWidget( QWidget *parent = 0 );
    ~ScrollImageWidget();
    float getZoomFactor() const {return m_zoomFactor; }

    void setImage(const QImage &);
    QPoint getScrollValue();
    QPoint getScrollMaximum();
    QColor getColorAtPosition(const QPoint & );


public slots:

    void setZoomFactor( float );
    void setScrollValue( const QPoint &);
    void setScrollMaximum(const QPoint&);
    void setHorizontalScrollValue(int);
    void setVerticalScrollValue(int);
    void fitToWindow();
	void drawPosition(const QPoint &);
	void insertPosition(const QPoint & pos,int flag);
	void insertShowPosition(const QPoint & pos,int flag);
signals:

    void somethingChanged();
    void mousePosition(const QPoint&);
	void countClicked(const QPoint&,int );
    void scrollPosition(const QPoint&);
    void horizontalScroll(int);
    void verticalScroll(int);

    void currentZoomFactor(float);

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent( QWheelEvent *event );
	void paintEvent(QPaintEvent *);
private:

    bool m_moving;
    QPoint lastPoint;
	QPoint pressPoint;
    QSize m_pmSize;
    QPoint m_point;
    float m_zoomFactor;
public:
	ShowImage *m_pw;
	
};

#endif // _SCROLLIMAGEWIDGET_H_
