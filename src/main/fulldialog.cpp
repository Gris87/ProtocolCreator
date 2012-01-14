#include "fulldialog.h"

FullDialog::FullDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Drawer);
    setWindowState(Qt::WindowMaximized);
}

FullDialog::~FullDialog()
{
    delete ui;
}
