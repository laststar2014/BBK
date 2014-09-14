#include <QtGui/QApplication>
#include <QTextCodec>
#include <QtGui/QPlastiqueStyle>
#include "widget.h"
#include "camera.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	// װ�ؿ��ܵ�ͼƬ���
	app.addLibraryPath( QApplication::applicationDirPath() + "./plugins");

	// modifying base look
	QApplication::setStyle(new QPlastiqueStyle);


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
  

	
	Widget w;
	w.move(0,0);
	w.show();
    return app.exec();
}
