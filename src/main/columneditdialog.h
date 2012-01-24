#ifndef COLUMNEDITDIALOG_H
#define COLUMNEDITDIALOG_H

#include <QDialog>

#include "ui_columneditdialog.h"

#include "src/widgets/unitedtable.h"
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
    UnitedTable *mTableWidget;
    bool mEditMode;
    int mColumnIndex;

    explicit ColumnEditDialog(bool aEditMode, UnitedTable *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent = 0);
    ~ColumnEditDialog();

    void startEditing();
    void applyChanges();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);
    void on_integerNumberSpinBox_valueChanged(int value);
    void on_integerAutoIncrementCheckBox_toggled(bool checked);
    void on_listLinkPagesListWidget_currentRowChanged(int currentRow);
    void on_listLinkVariablesListWidget_currentRowChanged(int currentRow);
    void on_extListLinkPagesListWidget_currentRowChanged(int currentRow);
    void on_extListLinkVariablesListWidget_currentRowChanged(int currentRow);
    void on_extListLinkColumnsListWidget_currentRowChanged(int currentRow);
    void on_functionButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();
};

#endif // COLUMNEDITDIALOG_H
