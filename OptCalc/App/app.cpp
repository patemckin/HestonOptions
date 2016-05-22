#include "app.h"

App::App(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.parAutoWrite, SIGNAL(triggered(bool)), this, SLOT(parAutoWrite_checked()));
	connect(ui.parHandWrite, SIGNAL(triggered(bool)), this, SLOT(parHandWrite_checked()));
	connect(ui.menuHelp, SIGNAL(triggered(bool)), this, SLOT(menuHelp_clicked()));

	QFile file("README.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ошибка открытия файла"), QMessageBox::Ok);
	}
	help = QString::fromLocal8Bit(file.readAll());
	file.close();
}

App::~App()
{

}

void App::on_getFileButton_clicked()
{
	filePath = QFileDialog::getOpenFileName(0, QString::fromLocal8Bit("Выберите файл"), "*.csv");

	fw = new FileWorker();
	if (fw->loadFile(filePath))
	{
		ui.filePathLine->clear();
		ui.filePathLine->insert(filePath);
		dataForParams = fw->getTable();
		ui.getParametersButton->setEnabled(true);
	}
	fw->~FileWorker();
}

void App::on_getParametersButton_clicked()
{
	// Разблокируем кнопку "Расчет цены"
	ui.getCostButton->setEnabled(true);

	// Разрешим вносить изменения в параметры рынка после расчета
	doSmthWithMarketParamsBlock(false);

	// Считываем данные с юай
	N = ui.N_SpinBox->text().toInt();
	bool *b = new bool[4];
	ap.popSize = ui.popSize_Line->text().toDouble(&b[0]);
	ap.genCount = ui.genCount_Line->text().toDouble(&b[1]);
	ap.crosProb = ui.crosProb_Line->text().toDouble(&b[2]);
	ap.mutProb = ui.mutProb_Line->text().toDouble(&b[3]);

	// Производим валидацию и вычисляем параметры с занесением их в Lines
	if (!(b[0] && b[1] && b[2] && b[3]))
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Параметры должны быть числами"), QMessageBox::Ok);
	}
	else
	{
		QString *s = new QString[4];
		s[0] = QString::fromLocal8Bit("Размер популяции");
		s[1] = QString::fromLocal8Bit("Количество поколений");
		s[2] = QString::fromLocal8Bit("Вероятность мутации");
		s[3] = QString::fromLocal8Bit("Вероятность кроссовера");
		QString msg = "";
		
		b[0] = ap.popSize >= 3 & ap.popSize <= 250 ;
		b[1] = ap.genCount >= 1 & ap.genCount <= 1000;
		b[2] = ap.mutProb >= 0 & ap.mutProb <= 1;
		b[3] = ap.crosProb >= 0 & ap.crosProb <= 1;

		if (!(b[0] && b[1] && b[2] && b[3]))
		{
			QString temp = "";
			for (int j = 0; j < 4; j++)
			{
				if (!b[j])
				{
					temp += s[j] + ", ";
				}
			}
			temp.remove(temp.length() - 2, 2);
			msg += temp;
			msg += QString::fromLocal8Bit(" вне границ\n");

			QMessageBox::StandardButton Load;
			Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), msg, QMessageBox::Ok);
		}
		else
		{
			GASolver solver(ap, N, dataForParams.toStdVector().data(), dataForParams.toStdVector().size(), this);
			marketParams mp = solver.getMarketParams();

			ui.kappa_Line->setText(QString::number(mp.kappa));
			ui.theta_Line->setText(QString::number(mp.theta));
			ui.v0_Line->setText(QString::number(mp.v0));
			ui.sigma_Line->setText(QString::number(mp.sigma));
			ui.rho_Line->setText(QString::number(mp.rho));
		}
	}
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

	N = ui.N_SpinBox->text().toInt();
	bool *b = new bool[9];
	data.S = ui.S_Line->text().toDouble(&b[0]);
	data.K = ui.K_Line->text().toDouble(&b[1]);
	data.T = ui.T_Line->text().toDouble(&b[2]);
	data.r = ui.r_Line->text().toDouble(&b[3]);
	data.v0 = ui.v0_Line->text().toDouble(&b[4]);
	data.theta = ui.theta_Line->text().toDouble(&b[5]);
	data.kappa = ui.kappa_Line->text().toDouble(&b[6]);
	data.sigma = ui.sigma_Line->text().toDouble(&b[7]);
	data.rho = ui.rho_Line->text().toDouble(&b[8]);

	if (!(b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6] && b[7] && b[8] && b[9]))
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Параметры должны быть числами"), QMessageBox::Ok);
	}
	else
	{
		QString *s = new QString[9];
		s[0] = "S";
		s[1] = "K";
		s[2] = "r";
		s[3] = "T";
		s[4] = "v0";
		s[5] = "theta";
		s[6] = "kappa";
		s[7] = "sigma";
		s[8] = "rho";
		QString msg = "";

		b[0] = data.S > 0. & data.S <= 100000.;
		b[1] = data.K > 0. & data.K <= 100000.;
		b[2] = data.r >= 0. & data.r <= 1.;
		b[3] = data.T > 0. & data.T <= 3.;
		b[4] = data.v0 > 0. & data.v0 < 1.;
		b[5] = data.theta >= 0. & data.theta <= 1.;
		b[6] = data.kappa >= 0. & data.kappa <= 50.;
		b[7] = data.sigma > 0. & data.sigma < 1.;
		b[8] = data.rho >= -1. & data.rho <= 1.;

		if (!(b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6] && b[7] && b[8]))
		{
			QString temp = "";
			for (int j = 0; j < 9; j++)
			{
				if (!b[j])
				{
					temp += s[j] + ", ";
				}
			}
			temp.remove(temp.length() - 2, 2);
			msg += temp;
			msg += QString::fromLocal8Bit(" вне границ\n");

			QMessageBox::StandardButton Load;
			Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), msg, QMessageBox::Ok);
		}
		else
		{
			double price = callPriceFFT(N, data.S, data.K, data.T, data.r, data.v0, data.theta, data.kappa, data.sigma, data.rho);
			ui.Opt_Line->setText(QString::number(price));
		}

		delete b;
	}
}

void App::parAutoWrite_checked()
{
	if (ui.parHandWrite->isChecked())
	{
		// Снимаем галку с режима "Ручной ввод"
		ui.parHandWrite->setChecked(false);

		// Разблокируем строку пути к файлу, кнопку "Выберите файл" и 
		// заблокируем кнопки "Расчет параметров" и "Расчет цены" для их последовательного вызова
		ui.filePathLine->setEnabled(true);
		ui.getFileButton->setEnabled(true);
		ui.getParametersButton->setEnabled(false);
		ui.getCostButton->setEnabled(false);

		// Очистим и заблокируем ячейки в блоке "Параметры рынка" до расчета параметров
		doSmthWithMarketParamsBlock(true);

		// Разблокируем блок "Параметры алгоритма" и установим дефолтные параметры
		doSmthWithAlgoParamsBlock(true);

		// Очистим ячеку со стоимостью опциона
		ui.Opt_Line->clear();
	}
	else
	{
		ui.parAutoWrite->setChecked(true);
	}
}

void App::parHandWrite_checked()
{
	if (ui.parAutoWrite->isChecked())
	{
		// Снимаем галку с режима "Ручной ввод"
		ui.parAutoWrite->setChecked(false);

		// Заблокируем и очистим строку пути к файлу, очистим переменную в которую пихался файл 
		ui.filePathLine->clear();
		ui.filePathLine->setEnabled(false);
		if (!dataForParams.isEmpty())
		{
			dataForParams.clear();
		}

		// Заблокируем кнопки "Выберите файл" и "Расчет параметров",
		// разблокируем кнопку "Расчет цены"
		ui.getFileButton->setEnabled(false);
		ui.getParametersButton->setEnabled(false);
		ui.getCostButton->setEnabled(true);

		// Очистим и разблокируем ячейки в блоке "Параметры рынка" до расчета параметров
		doSmthWithMarketParamsBlock(false);

		// Заблокируем блок "Параметры алгоритма"
		doSmthWithAlgoParamsBlock(false);

		// Очистим ячеку со стоимостью опциона
		ui.Opt_Line->clear();
	}
	else
	{
		ui.parHandWrite->setChecked(true);
	}
}

void App::doSmthWithAlgoParamsBlock(bool b)
{
	ui.parAlgoLabel->setEnabled(b);

	ui.popSize_Label->setEnabled(b);
	ui.genCount_Label->setEnabled(b);
	ui.crosProb_Label->setEnabled(b);
	ui.mutProb_Label->setEnabled(b);

	ui.popSize_Line->setEnabled(b);
	ui.genCount_Line->setEnabled(b);
	ui.crosProb_Line->setEnabled(b);
	ui.mutProb_Line->setEnabled(b);

	if (b)
	{
		ui.popSize_Line->setText("5");
		ui.genCount_Line->setText("3");
		ui.crosProb_Line->setText("0.9");
		ui.mutProb_Line->setText("0.2");
	}
	else
	{
		ui.popSize_Line->clear();
		ui.genCount_Line->clear();
		ui.crosProb_Line->clear();
		ui.mutProb_Line->clear();
	}
}

void App::doSmthWithMarketParamsBlock(bool b)
{
	ui.v0_Line->clear();
	ui.theta_Line->clear();
	ui.kappa_Line->clear();
	ui.sigma_Line->clear();
	ui.rho_Line->clear();

	ui.v0_Line->setReadOnly(b);
	ui.theta_Line->setReadOnly(b);
	ui.kappa_Line->setReadOnly(b);
	ui.sigma_Line->setReadOnly(b);
	ui.rho_Line->setReadOnly(b);
}

void App::menuHelp_clicked()
{
	makeDialogWithString(help);
}

void App::makeDialogWithString(const QString s)
{
	QDialog *dialog = new QDialog(this);
	QLabel *label = new QLabel(this);
	QScrollArea *scroll = new QScrollArea(this);
	QVBoxLayout *layout = new QVBoxLayout();

	label->setText(s);
	scroll->setWidget(label);
	scroll->setWidgetResizable(true);
	layout->addWidget(scroll);
	dialog->setLayout(layout);

	dialog->exec();
	delete dialog;
}