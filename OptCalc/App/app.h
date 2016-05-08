#ifndef APP_H
#define APP_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include "ui_app.h"
#include "file_worker.h"
#include "PriceCalculation.h"
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
	FileWorker * fw;
	Table dataForParams;
	PriceFunParams data;

private slots:
	void on_getFileButton_clicked();
	void on_getParametersButton_clicked();
	void on_getCostButton_clicked();
	void parAutoWrite_checked();
	void parHandWrite_checked();
};

#endif // APP_H
