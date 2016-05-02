#include "file_worker.h"


FileWorker::FileWorker(QWidget *parent)
	: QWidget(parent)
{

}

FileWorker::~FileWorker()
{

}

bool FileWorker::checkFile()
{
	QString *s = new QString[5];
	s[0] = "Expiration";
	s[1] = "Spot";
	s[2] = "Bid";
	s[3] = "Ask";
	s[4] = "Strike";
	QString msg = "";

	for (int i = 0; i < arr.size(); i++)
	{
		bool *b = new bool[5];
		b[0] = arr.at(i).at(0) > 0. & arr.at(i).at(0) <= 3.;
		b[1] = arr.at(i).at(1) > 0. & arr.at(i).at(1) <= 100000.;
		b[4] = arr.at(i).at(4) > 0. & arr.at(4).at(1) <= 100000.;
		double diff = abs(arr.at(i).at(1) - arr.at(i).at(4));
		b[2] = arr.at(i).at(2) >= diff & arr.at(i).at(1) <= 100. * diff;
		b[3] = arr.at(i).at(3) >= diff & arr.at(i).at(1) <= 100. * diff;

		qDebug() << b[0] << b[1] << b[2] << b[3] << b[4] << endl;

		if (!(b[0] && b[1] && b[2] && b[3] && b[4]))
		{
			msg += "Line " + QString::number(i + 1) + ":\n";
			QString temp = "";
			int counter = 0;
			for (int j = 0; j < 5; j++)
			{
				if (!b[j])
				{
					temp += s[j] + ", ";
					counter++;
				}
			}
			temp.remove(temp.length() - 2, 2);
			msg += temp;
			if (counter == 1)
			{
				msg += " is ";
			}
			else
			{
				msg += " are ";
			}
			msg += "out of bounds\n";
		}

		delete(b);
	}

	if (!msg.isEmpty())
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, "Error", msg, QMessageBox::Ok);
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
		Load = QMessageBox::critical(this, "Error", "Error of table's opening", QMessageBox::Ok);
		return false;
	}
	QFileInfo fi(filePath);
	if (fi.size() > MAX_FILE_SIZE)
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, "Error", "File's size is more than 100 KB", QMessageBox::Ok);
		return false;
	}

	QString data = file.readAll();
	QStringList rows = data.split("\n");
	
	for (int i = 0; i < rows.length() - 1; i++)
	{
		QStringList row = rows.at(i).split(";");
		
		QVector<double> temp = QVector<double>();

		if (row.length() != 5)
		{
			QMessageBox::StandardButton Load;
			Load = QMessageBox::critical(this, "Error", "The table must be 5 columns", QMessageBox::Ok);
			return false;
		}

		for (int j = 0; j < row.length(); j++)
		{
			bool result;
			double num = row.at(j).toDouble(&result);
			if (!result)
			{
				QMessageBox::StandardButton Load;
				Load = QMessageBox::critical(this, "Error", "The table must be a number", QMessageBox::Ok);
				return false;
			}
			temp.append(num);
		}

		arr.append(temp);
	}

	return checkFile();
}

Matrix FileWorker::getTable()
{
	return arr;
}
