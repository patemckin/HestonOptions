#include "app.h"

App::App(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.parAutoWrite, SIGNAL(triggered(bool)), this, SLOT(parAutoWrite_checked()));
	connect(ui.parHandWrite, SIGNAL(triggered(bool)), this, SLOT(parHandWrite_checked()));
}

App::~App()
{

}

void App::on_getFileButton_clicked()
{
	filePath = QFileDialog::getOpenFileName(0, "Select file", "*.csv");

	fw = new FileWorker();
	if (fw->loadFile(filePath))
	{
		ui.filePathLine->clear();
		ui.filePathLine->insert(filePath);
		dataForParams = fw->getTable();
		ui.getParametersButton->setEnabled(true);
	}
	fw->~FileWorker();

	ui.getParametersButton->setEnabled(true);
}

void App::on_getParametersButton_clicked()
{
	ui.getCostButton->setEnabled(true);

	ui.v0_Line->setReadOnly(false);
	ui.theta_Line->setReadOnly(false);
	ui.kappa_Line->setReadOnly(false);
	ui.sigma_Line->setReadOnly(false);
	ui.rho_Line->setReadOnly(false);

	// Все данные для расчета находятся в dataForParam (это QVector<optionParams>, typedef лежит в file_worker.h)
}

void App::on_getCostButton_clicked()
{
	/***********************
	double N = 18;
	double S = 95;
	double K = 100;
	double T = 1;
	double r = 0;
	double v0 = 0.04;
	double theta = 0.04;
	double kappa = 0.25;
	double sigma = 0.5;
	double rho = -0.6;
	***********************/

	if (ui.parHandWrite->isChecked())
	{
		bool *b = new bool[10];
		
		//data.N = ui.N_Line->text().toInt(&b[0]);
		data.S = ui.S_Line->text().toDouble(&b[1]);
		data.K = ui.K_Line->text().toDouble(&b[2]);
		data.T = ui.T_Line->text().toDouble(&b[3]);
		data.r = ui.r_Line->text().toDouble(&b[4]);
		data.v0 = ui.v0_Line->text().toDouble(&b[5]);
		data.theta = ui.theta_Line->text().toDouble(&b[6]);
		data.kappa = ui.kappa_Line->text().toDouble(&b[7]);
		data.sigma = ui.sigma_Line->text().toDouble(&b[8]);
		data.rho = ui.rho_Line->text().toDouble(&b[9]);

		if (!(b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6] && b[7] && b[8] && b[9]))
		{
			QMessageBox::StandardButton Load;
			Load = QMessageBox::critical(this, "Error", "The parameters must be a number", QMessageBox::Ok);
		}
		else
		{
			double price = callPriceFFT(18, data.S, data.K, data.T, data.r, data.v0, data.theta, data.kappa, data.sigma, data.rho);
			ui.Opt_Line->setText(QString::number(price));
		}
	}
	
}

void App::parAutoWrite_checked()
{	
	ui.parHandWrite->setChecked(false);
	ui.filePathLine->setEnabled(true);
	ui.getFileButton->setEnabled(true);
	ui.getParametersButton->setEnabled(false);
	
	ui.v0_Line->clear();
	ui.theta_Line->clear();
	ui.kappa_Line->clear();
	ui.sigma_Line->clear();
	ui.rho_Line->clear();
	ui.v0_Line->setReadOnly(true);
	ui.theta_Line->setReadOnly(true);
	ui.kappa_Line->setReadOnly(true);
	ui.sigma_Line->setReadOnly(true);
	ui.rho_Line->setReadOnly(true);

	ui.getCostButton->setEnabled(false);
	ui.Opt_Line->clear();
}

void App::parHandWrite_checked()
{
	ui.parAutoWrite->setChecked(false);

	ui.filePathLine->clear();
	ui.filePathLine->setEnabled(false);
	if (!dataForParams.isEmpty())
	{
		dataForParams.clear();
	}

	ui.getFileButton->setEnabled(false);
	ui.getParametersButton->setEnabled(false);

	ui.v0_Line->clear();
	ui.theta_Line->clear();
	ui.kappa_Line->clear();
	ui.sigma_Line->clear();
	ui.rho_Line->clear();

	ui.Opt_Line->clear();
}