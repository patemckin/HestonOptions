#include "app.h"

App::App(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

App::~App()
{

}

void App::on_getFileButton_clicked()
{
	filePath = QFileDialog::getOpenFileName(0, QString::fromUtf16((ushort const*)_T("Выберите файл")), QString::fromUtf16((ushort const*)_T("")), QString::fromUtf16((ushort const*)_T("*.csv")));

	fw = new FileWorker();
	if (fw->loadFile(filePath))
	{
		ui.filePathLine->clear();
		ui.filePathLine->insert(filePath);
		table = fw->getTable();
		ui.getParametersButton->setEnabled(true);
	}
	fw->~FileWorker();
}

void App::on_getParametersButton_clicked()
{

}

void App::on_getCostButton_clicked()
{

}
