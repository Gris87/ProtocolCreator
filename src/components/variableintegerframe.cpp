#include "src/other/global.h"

VariableIntegerFrame::VariableIntegerFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableIntegerFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Число");
    ui->varNameEdit->setText("Number");
}

VariableIntegerFrame::~VariableIntegerFrame()
{
    delete ui;
}

void VariableIntegerFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableIntegerFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableIntegerFrame::on_decimalsSpinBox_valueChanged(int aValue)
{
    ui->valueSpinBox->setDecimals(aValue);
}
