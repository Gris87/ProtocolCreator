#include "src/other/global.h"

SectionControlDialog::SectionControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SectionControlDialog)
{
    ui->setupUi(this);
}

SectionControlDialog::~SectionControlDialog()
{
    delete ui;
}
