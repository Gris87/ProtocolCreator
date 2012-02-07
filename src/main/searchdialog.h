#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "ui_searchdialog.h"

namespace Ui {
    class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::SearchDialog *ui;

    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void find(int mode);

private slots:
    void on_replaceGroupBox_toggled(bool checked);
    void on_replaceButton_clicked();
    void on_replaceAllButton_clicked();
    void on_findButton_clicked();
};

#endif // SEARCHDIALOG_H
