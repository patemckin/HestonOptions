#include "app.h"
#include "file_worker.h"

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
	filePath = QFileDialog::getOpenFileName(0, "Âûבונטעו פאיכ", "", "*.csv");

	FileWorker * fw = new FileWorker();
	if (fw->loadFile(filePath))
	{
		ui.filePathLine->insert(filePath);
	}
}

void App::on_getParametersButton_clicked()
{

}

void App::on_getCostButton_clicked()
{

}
