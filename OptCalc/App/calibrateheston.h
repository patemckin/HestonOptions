#pragma once
#include <QObject>
#include <QProgressDialog>
#include <QDebug>
#include <iostream>
#include <math.h>
#include <ga/ga.h>
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include <ga/GASStateGA.h>
#include "PriceCalculation.h"
#include <vector>
#include "structs.h"

class GASolver: GAGenome 
{
public:	
	GASolver(AlgoParams p, unsigned int _N, optionParams * _data, int _size, void *_ptr);
	~GASolver();
	marketParams getMarketParams();
	static bool stop;

private:
	AlgoParams alparam;
	static unsigned int N;
	static optionParams * data;
	static size_t size;
	void *ptr;

	static GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga);
	static double currentPrice(GAGenome& g, optionParams params);
	static float objective(GAGenome& g);

	static double marketSpread;
};

class PBClass: public QObject, public  GASteadyStateGA
{
	Q_OBJECT

public:
	PBClass(GARealGenome &g, void *_ptr, QObject * parent = 0): GASteadyStateGA(g), QObject(parent)
	{
		p = new QProgressDialog(QString::fromLocal8Bit("Вычисление параметров..."), QString::fromLocal8Bit("Отмена"), 0, 100, (QWidget*)nullptr);
		p->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		p->setModal(true);
		p->setMinimumDuration(0);
		p->setAutoReset(true);
		p->setValue(0);
		p->show();
	}

	virtual ~PBClass()
	{
		if (!GASolver::stop)
		{
			delete p;
		}	
	}

	virtual void step();

	void setProgressValue(int value)
	{
		p->setValue(value);
	}

private:
	QProgressDialog *p;
};