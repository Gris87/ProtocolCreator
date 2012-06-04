#include "src/other/global.h"

AutoUpdateProgressBar::AutoUpdateProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void AutoUpdateProgressBar::setValue(int value)
{
    mainWindow->wantAutoSave=false;

    QProgressBar::setValue(value);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    mainWindow->wantAutoSave=true;
}
