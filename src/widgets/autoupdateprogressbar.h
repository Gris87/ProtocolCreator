#ifndef AUTOUPDATEPROGRESSBAR_H
#define AUTOUPDATEPROGRESSBAR_H

#include <QProgressBar>
#include <QApplication>

class AutoUpdateProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit AutoUpdateProgressBar(QWidget *parent = 0);

    void setValue(int value);
};

#endif // AUTOUPDATEPROGRESSBAR_H
