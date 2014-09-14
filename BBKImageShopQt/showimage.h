#ifndef SHOWIMAGE_H
#define SHOWIMAGE_H

#include <QPixmap>
#include <QWidget>
#include <QStringList>
#include <QDir>
#include <vector>
class QLabel;
class ShowImage : public QWidget
{
    Q_OBJECT

public:
    ShowImage(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
	void setImage(const QImage &image);
	void setImage(const QString &);

	const QImage & getImage() const {return pixmap; }
	int getImageWidth();
	int getImageHeight();
	int getSum();
	void setZoomFactor( float f );
	void drawFlag(const QPoint &,float zoomFactor);
	void insertArray(const QPoint & pos,int flag);
	void insertShowArray(const QPoint & pos,int flag);
private:
	void ColorTurn(QColor &color,QColor &dst_color);
signals:
    void get2end(bool);
    void get2beg(bool);
    void turn2default(int);//缩放比例
	void centerChange(int,int);

public slots:
    void init_path(QDir,QString);
	void imread(QString file);
protected:
    void paintEvent(QPaintEvent *event);//利用QPainter绘图
	void mouseDoubleClickEvent(QMouseEvent *);

private:
    QStringList::const_iterator iterator;//常量迭代器
    QStringList list;
    QDir filepath;
	float m_zoomFactor;

	QPixmap m_tile;
    //QPixmap pixmap;//“绘图设备”
	
	
    double wscale;
    double hscale;

	QPoint m_point;
    bool FirstTimeShowFlag;
	

public:
	volatile int x;
	volatile int y;
	QImage pixmap;//“绘图设备”

	int		 img_width;
	int      img_height;
	QVector< QPoint>  gArray;
	QVector< QPoint>  gShowArray;
	QVector< QPoint>  gBoudary;
	QColor  m_color;
	int      m_IsAsistance;
};

#endif // SHOWIMAGE_H
