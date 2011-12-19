#ifndef CELLALIGNMENTWIDGET_H
#define CELLALIGNMENTWIDGET_H

#include <QWidget>

#include "ui_cellalignmentwidget.h"

namespace Ui {
    class CellAlignmentWidget;
}

class CellAlignmentWidget : public QWidget
{
    Q_OBJECT

public:
    Ui::CellAlignmentWidget *ui;

    explicit CellAlignmentWidget(QWidget *parent = 0);
    ~CellAlignmentWidget();
};

#endif // CELLALIGNMENTWIDGET_H
