#ifndef DUMPCRASHES_H
#define DUMPCRASHES_H

#include <QDebug>

#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QTimer>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <limits.h>

#include "exceptionobject.h"



// Use this function to handle crashes
void allowDumpCrashes();

QtMsgHandler installMsgHandler(QtMsgHandler h=0, int maxLogCount=0);

QString getStackTrace();
void printStackTrace();



#endif // DUMPCRASHES_H
