#ifndef EXCEPTIONOBJECT_H
#define EXCEPTIONOBJECT_H

#include <QObject>

#include <QThread>

#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QTextStream>

#include "crashdialog.h"

class ExceptionObject : public QObject
{
    Q_OBJECT
private:
    QString mReport;

public:
    explicit ExceptionObject(const QString &aReport, QObject *parent = 0);

public slots:
    void showDialog();
};

#endif // EXCEPTIONOBJECT_H
