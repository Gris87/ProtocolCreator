#ifndef PAGEFRAME_H
#define PAGEFRAME_H

#include <QWidget>

#include "ui_pageframe.h"

namespace Ui {
    class PageFrame;
}

class PageFrame : public QWidget
{
    Q_OBJECT

public:
    explicit PageFrame(QWidget *parent = 0);
    ~PageFrame();

private:
    Ui::PageFrame *ui;
};

#endif // PAGEFRAME_H
