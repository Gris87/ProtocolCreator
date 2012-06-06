#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QDialog>

#include "ui_functiondialog.h"

#include <QSplitter>
#include "src/components/variableextendedlistframe.h"

namespace Ui {
    class FunctionDialog;
}

class FunctionDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::FunctionDialog *ui;
    QSplitter *dividerSplitter;
    QSplitter *dividerSplitter2;
    VariableExtendedListFrame* mExtList;

    explicit FunctionDialog(VariableExtendedListFrame *aExtList=0, QWidget *parent = 0);
    ~FunctionDialog();

    QString pageSection();

private slots:
    void on_functionsListWidget_currentRowChanged(int currentRow);
    void on_functionsListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pagesListWidget_currentRowChanged(int currentRow);
    void on_variablesListWidget_currentRowChanged(int currentRow);
    void on_variablesListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_columnListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // FUNCTIONDIALOG_H
