#ifndef COLUMNEDITDIALOG_H
#define COLUMNEDITDIALOG_H

#include <QDialog>

#include "ui_columneditdialog.h"

#include <QTableWidget>
#include "src/components/variableextendedlistframe.h"

namespace Ui {
    class ColumnEditDialog;
}

class ColumnEditDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::ColumnEditDialog *ui;
    VariableExtendedListFrame *mTable;
    QTableWidget *mTableWidget;

    explicit ColumnEditDialog(bool editMode, QTableWidget *aTableWidget, VariableExtendedListFrame *aTable, int columnIndex, QWidget *parent = 0);
    ~ColumnEditDialog();
};

#endif // COLUMNEDITDIALOG_H
