#ifndef APP_H
#define APP_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ui_app.h"
#include "file_worker.h"
#include "PriceCalculation.h"
#include "calibrateheston.h"
#include "structs.h"


class App : public QMainWindow
{
	Q_OBJECT

public:
	App(QWidget *parent = 0);
	~App();

private:
	Ui::AppClass ui;
	QString filePath;
	FileWorker* fw;
	Table* dataForParams = NULL;
	PriceFunParams data;
	marketParams mp;
	AlgoParams ap;
	unsigned int N;
	QString help;

	const int decimals = 7;
	QValidator* vldtrs[9];
	QValidator* vldtrsAlgo[4];
	QStringList *s;
	QStringList *sAlgo;
	QVector<QLineEdit* > *lines;
	QVector<QLineEdit* > *linesAlgo;

	void doSmthWithAlgoParamsBlock(bool b);
	void doSmthWithMarketParamsBlock(bool b);
	void makeDialogWithString(const QString s);

private slots:
	void on_getFileButton_clicked();
	void on_getParametersButton_clicked();
	void on_getCostButton_clicked();
	void parAutoWrite_checked();
	void parHandWrite_checked();
	void menuHelp_clicked();
};

#endif // APP_H
