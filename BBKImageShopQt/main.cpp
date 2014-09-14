#include <QtGui/QApplication>
#include <QTextCodec>
#include <QtGui/QPlastiqueStyle>
#include "widget.h"
#include "camera.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	// 装载可能的图片插件
	app.addLibraryPath( QApplication::applicationDirPath() + "./plugins");

	// modifying base look
	QApplication::setStyle(new QPlastiqueStyle);


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
  

	
	Widget w;
	w.move(0,0);
	w.show();
    return app.exec();
}
