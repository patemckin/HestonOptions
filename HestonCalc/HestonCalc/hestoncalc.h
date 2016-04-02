#ifndef HESTONCALC_H
#define HESTONCALC_H

#include <QtWidgets/QMainWindow>
#include "ui_hestoncalc.h"

class HestonCalc : public QMainWindow
{
	Q_OBJECT

public:
	HestonCalc(QWidget *parent = 0);
	~HestonCalc();

private:
	Ui::HestonCalcClass ui;
};

#endif // HESTONCALC_H
