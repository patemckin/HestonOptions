#ifndef FILE_WORKER_H
#define FILE_WORKER_H

#include <QtWidgets/QWidget>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
#include <QFileInfo>

#define MAX_FILE_SIZE 102400

typedef QVector<QVector<double>> Matrix;

class FileWorker : public QWidget
{
	Q_OBJECT

public:
	FileWorker(QWidget *parent = 0);
	~FileWorker();

	bool loadFile(const QString filePath);
	bool checkFile();
	Matrix getTable();

private:
	Matrix arr;
};

#endif // FILE_WORKER_H