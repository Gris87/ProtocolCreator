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
    bool mEditMode;
    int mColumnIndex;

    explicit ColumnEditDialog(bool aEditMode, QTableWidget *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent = 0);
    ~ColumnEditDialog();

    void applyChanges();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);
    void on_integerNumberSpinBox_valueChanged(double value);
};

#endif // COLUMNEDITDIALOG_H
