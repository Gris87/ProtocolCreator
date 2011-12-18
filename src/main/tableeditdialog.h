#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>

#include "ui_tableeditdialog.h"

#include "src/components/variableextendedlistframe.h"

namespace Ui {
    class TableEditDialog;
}

class TableEditDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::TableEditDialog *ui;
    VariableExtendedListFrame *mTable;

    explicit TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent = 0);
    ~TableEditDialog();

    void updateAdmin();

private slots:
    void headerUnion();
    void headerDeunion();
    void headerColumnSize();
    void headerFont();
    void headerOffset();
    void headerLocation();
    void headerAlignment();

    void on_headerAddRowButton_clicked();
    void on_headerDelRowButton_clicked();
    void on_headerAddColButton_clicked();
    void on_headerDelColButton_clicked();

    void on_headerTableWidget_customContextMenuRequested(const QPoint &pos);
};

#endif // TABLEEDITDIALOG_H
