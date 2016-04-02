#include "hestoncalc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HestonCalc w;
	w.show();
	return a.exec();
}
