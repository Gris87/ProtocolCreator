#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

#include "ui_passworddialog.h"

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::PasswordDialog *ui;

    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();
private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
};

#endif // PASSWORDDIALOG_H
