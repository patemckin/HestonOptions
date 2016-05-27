#include "file_worker.h"
#include <QDebug>


FileWorker::FileWorker(QWidget *parent)
	: QWidget(parent)
{
	arr = new QVector<QVector<double> >();
}

FileWorker::~FileWorker()
{
	delete arr;
}

bool FileWorker::checkFile()
{
	QString *s = new QString[6];
	s[0] = "T";
	s[1] = "S";
	s[2] = "Bid";
	s[3] = "Ask";
	s[4] = "K";
	s[5] = "r";
	QString msg = "";

	for (int i = 0; i < arr->size(); i++)
	{
		bool *b = new bool[6];
		b[0] = (*arr)[i][0] > 0. & (*arr)[i][0] <= 3.;
		b[1] = (*arr)[i][1] > 0. & (*arr)[i][1] <= 100000.;
		b[4] = (*arr)[i][4] > 0. & (*arr)[i][4] <= 100000.;
		double diff = abs((*arr)[i][1] - (*arr)[i][4]);
		b[2] = (*arr)[i][2] >= 0.01 * diff & (*arr)[i][2] <= 100. * diff; 
		b[3] = (*arr)[i][3] >= 0.01 * diff & (*arr)[i][3] <= 100. * diff;
		b[5] = (*arr)[i][5] >= 0. & (*arr)[i][5] <= 1.;

		if (!(b[0] && b[1] && b[2] && b[3] && b[4] && b[5]))
		{
			msg += QString::fromLocal8Bit("Строка ") + QString::number(i + 1) + ":\n";
			QString temp = "";
			for (int j = 0; j < 6; j++)
			{
				if (!b[j])
				{
					temp += s[j] + ", ";
				}
			}
			temp.remove(temp.length() - 2, 2);
			msg += temp;
			msg += QString::fromLocal8Bit(" вне границ\n");
		}

		delete(b);
	}

	if (!msg.isEmpty())
	{
		QMessageBox *mb = new QMessageBox(this);
		mb->setText(QString::fromLocal8Bit("История не валидна"));
		mb->setDetailedText(msg);
		mb->exec();
		return false;
	}

	return true;
}

bool FileWorker::loadFile(const QString filePath)
{
	QFile file(filePath);
	if (filePath.isEmpty())
	{
		file.close();
		return false;
	}
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ошибка открытия файла"), QMessageBox::Ok);
		return false;
	}
	QFileInfo fi(filePath);
	if (fi.size() > MAX_FILE_SIZE)
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Размер файла больше 100 Кбайт"), QMessageBox::Ok);
		return false;
	}

	QString data = file.readAll();
	QStringList rows = data.split("\n");
	
	for (int i = 0; i < rows.length() - 1; i++)
	{
		QStringList row = rows.at(i).split(";");
		
		QVector<double> temp = QVector<double>();

		if (row.length() != 6)
		{
			QMessageBox::StandardButton Load;
			Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("В таблице должно быть 6 столбцов"), QMessageBox::Ok);
			return false;
		}

		for (int j = 0; j < row.length(); j++)
		{
			bool result;
			double num = row.at(j).toDouble(&result);
			if (!result)
			{
				QMessageBox::StandardButton Load;
				Load = QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("В таблице должны быть числа"), QMessageBox::Ok);
				return false;
			}
			temp.append(num);
		}

		arr->append(temp);
	}

	return checkFile();
}

Table* FileWorker::getTable()
{
	Table* res = new QVector<optionParams>(arr->size());

	for (int i = 0; i < arr->size(); i++)
	{
		(*res)[i].S = (*arr)[i][1];
		(*res)[i].K = (*arr)[i][4];
		(*res)[i].T = (*arr)[i][0];
		(*res)[i].r = (*arr)[i][5];
		(*res)[i].price = ((*arr)[i][3] + (*arr)[i][2]) / 2;
		(*res)[i].bid = (*arr)[i][2];
		(*res)[i].ask = (*arr)[i][3];

		//qDebug() << (*arr)[i][0] << (*arr)[i][1] << (*arr)[i][2] << (*arr)[i][3] << (*arr)[i][4] << (*arr)[i][5] << endl;
	}

	return res;
}
