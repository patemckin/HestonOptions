#include "file_worker.h"
#include <QDebug>
#include <QVector>
#include <QFileInfo>

FileWorker::FileWorker(QWidget *parent)
	: QWidget(parent)
{

}

FileWorker::~FileWorker()
{

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
		Load = QMessageBox::critical(this, "Ошибка", "Ошибка открытия таблицы", QMessageBox::Ok);
		return false;
	}
	QFileInfo fi(filePath);
	if (fi.size() > MAX_FILE_SIZE)
	{
		QMessageBox::StandardButton Load;
		Load = QMessageBox::critical(this, "Ошибка", "Размер файла больше 100 Кбайт", QMessageBox::Ok);
		return false;
	}

	QString data = file.readAll();
	QStringList rows = data.split("\n");
	
	for (int i = 0; i < rows.length() - 1; i++)
	{
		QStringList row = rows.at(i).split(";");
		
		QVector<double> temp = QVector<double>();

		for (int j = 0; j < row.length(); j++)
		{
			temp.append(row.at(j).toDouble());
		}

		arr.append(temp);
	}

	return true;
}