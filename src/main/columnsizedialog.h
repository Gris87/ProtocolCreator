#ifndef COLUMNSIZEDIALOG_H
#define COLUMNSIZEDIALOG_H

#include <QDialog>

#include "ui_columnsizedialog.h"

namespace Ui {
    class ColumnSizeDialog;
}

class ColumnSizeDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::ColumnSizeDialog *ui;

    explicit ColumnSizeDialog(QWidget *parent = 0);
    ~ColumnSizeDialog();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // COLUMNSIZEDIALOG_H
