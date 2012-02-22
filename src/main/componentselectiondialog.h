#ifndef COMPONENTSELECTIONDIALOG_H
#define COMPONENTSELECTIONDIALOG_H

#include <QDialog>

#include "ui_componentselectiondialog.h"

#include <QSplitter>

namespace Ui {
    class ComponentSelectionDialog;
}

class ComponentSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::ComponentSelectionDialog *ui;
    QSplitter *dividerSplitter;
    QString mResult;

    explicit ComponentSelectionDialog(QWidget *parent = 0);
    ~ComponentSelectionDialog();

private slots:
    void on_pagesListWidget_currentRowChanged(int currentRow);
    void on_variablesListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // COMPONENTSELECTIONDIALOG_H
