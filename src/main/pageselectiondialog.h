#ifndef PAGESELECTIONDIALOG_H
#define PAGESELECTIONDIALOG_H

#include <QDialog>

#include "ui_pageselectiondialog.h"

namespace Ui {
    class PageSelectionDialog;
}

class PageSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::PageSelectionDialog *ui;

    explicit PageSelectionDialog(QWidget *parent = 0);
    ~PageSelectionDialog();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_pagesListWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // PAGESELECTIONDIALOG_H
