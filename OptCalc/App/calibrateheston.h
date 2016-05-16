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
		p = new QProgressDialog("Calculating parameters...", "", 0, 100, (QWidget*)nullptr);
		QProgressBar * bar = new QProgressBar();
		bar->setTextVisible(true);


		p->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		p->setModal(true);
		//p->setWindowModality(Qt::WindowModal);
	    p->setCancelButton(0);
		p->setValue(0);
		p->setMinimumDuration(0);
		p->setAutoReset(true);
		p->show();
	}
	virtual ~MyClass() {
		delete p;
	};
	virtual void step();
private:
	QProgressDialog *p;
};