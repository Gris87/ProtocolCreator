#include "src/other/global.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_cancelButton_clicked()
{
    reject();
}

void PasswordDialog::on_okButton_clicked()
{
    accept();
}
