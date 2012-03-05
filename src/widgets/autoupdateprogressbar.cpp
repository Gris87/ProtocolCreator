#include "autoupdateprogressbar.h"

AutoUpdateProgressBar::AutoUpdateProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void AutoUpdateProgressBar::setValue(int value)
{
    QProgressBar::setValue(value);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}
