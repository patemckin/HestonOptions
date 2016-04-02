#ifndef APP_H
#define APP_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include "ui_app.h"

class App : public QMainWindow
{
	Q_OBJECT

public:
	App(QWidget *parent = 0);
	~App();

private:
	Ui::AppClass ui;
	QString filePath;

private slots:
	void on_getFileButton_clicked();
	void on_getParametersButton_clicked();
	void on_getCostButton_clicked();
};

#endif // APP_H
