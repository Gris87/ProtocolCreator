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
};

#endif // TABLEEDITDIALOG_H
