#include "app.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{	
	QApplication a(argc, argv);
	App w;
	w.show();
	qDebug() << "Privet";
	return a.exec();
}
