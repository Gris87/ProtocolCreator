#ifndef INSERTLINKTABLEDIALOG_H
#define INSERTLINKTABLEDIALOG_H

#include <QDialog>

#include "ui_insertlinktabledialog.h"

#include "src/components/variableextendedlistframe.h"

namespace Ui {
    class InsertLinkTableDialog;
}

class InsertLinkTableDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::InsertLinkTableDialog *ui;
    VariableExtendedListFrame *mSourceTable;
    VariableExtendedListFrame *mDestTable;
    int mBeginPos;

    explicit InsertLinkTableDialog(VariableExtendedListFrame *aSourceTable, VariableExtendedListFrame *aDestTable, QWidget *parent = 0);
    ~InsertLinkTableDialog();

private slots:
    void updateTableHeights();

    void on_closeButton_clicked();
    void on_addButton_clicked();
    void on_sourceTableWidget_doubleClicked(const QModelIndex &index);
};

#endif // INSERTLINKTABLEDIALOG_H
