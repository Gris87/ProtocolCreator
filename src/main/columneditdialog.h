#ifndef COLUMNEDITDIALOG_H
#define COLUMNEDITDIALOG_H

#include <QDialog>

#include "ui_columneditdialog.h"

namespace Ui {
    class ColumnEditDialog;
}

class ColumnEditDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::ColumnEditDialog *ui;

    explicit ColumnEditDialog(QWidget *parent = 0);
    ~ColumnEditDialog();
};

#endif // COLUMNEDITDIALOG_H
