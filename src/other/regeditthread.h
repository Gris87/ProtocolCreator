#ifndef REGEDITTHREAD_H
#define REGEDITTHREAD_H

#include <QThread>
#include <QProcess>

class RegEditThread : public QThread
{
    Q_OBJECT
public:
    explicit RegEditThread(QObject *parent = 0);

    void run();
};

#endif // REGEDITTHREAD_H
