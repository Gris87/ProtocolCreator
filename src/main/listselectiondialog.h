#ifndef LISTSELECTIONDIALOG_H
#define LISTSELECTIONDIALOG_H

#include <QDialog>

#include "ui_listselectiondialog.h"

#include <QSplitter>

namespace Ui {
    class ListSelectionDialog;
}

class ListSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::ListSelectionDialog *ui;
    QSplitter *dividerSplitter;
    QString mResult;

    explicit ListSelectionDialog(QString aBaseList="", QWidget *parent = 0);
    ~ListSelectionDialog();

private slots:
    void on_pagesListWidget_currentRowChanged(int currentRow);
    void on_variablesListWidget_currentRowChanged(int currentRow);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // LISTSELECTIONDIALOG_H
