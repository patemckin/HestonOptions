#pragma once
#include <QObject>
#include <QProgressDialog>
#include <QProgressBar>
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

private:
	AlgoParams alparam;
	static unsigned int N;
	static optionParams * data;
	static size_t size;
	void *ptr;

	static double currentPrice(GAGenome& g, optionParams params);
	static float objective(GAGenome& g);
	GABoolean done();
	static GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga);

	static double marketSpread;
};

class MyClass : public QObject, public  GASteadyStateGA
{
	Q_OBJECT

public:
	MyClass(GARealGenome &g, void *_ptr, QObject * parent = 0) : GASteadyStateGA(g), QObject(parent)
	{
		//connect(this, SIGNAL(sig(double)), (QObject*)_ptr, SLOT(_ptr->ui.progressBar->setValue(int)));
		p = new QProgressDialog();
		QProgressBar * bar = new QProgressBar();
		p->setBar(bar);
		p->setMaximum(100);
		p->setMinimum(0);
		p->setWindowModality(Qt::WindowModal);
		p->setValue(0);
		p->show();
	}
	virtual ~MyClass() {
		delete p;
	};
	virtual void step();
private:
	QProgressDialog *p;

signals:
	void sig(int val);
};