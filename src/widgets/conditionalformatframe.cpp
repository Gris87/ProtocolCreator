#include "src/other/global.h"

ConditionalFormatFrame::ConditionalFormatFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionalFormatFrame)
{
    ui->setupUi(this);
}

ConditionalFormatFrame::~ConditionalFormatFrame()
{
    delete ui;
}

void ConditionalFormatFrame::on_delToolButton_clicked()
{
    emit delRequested(this);
}

void ConditionalFormatFrame::on_upToolButton_clicked()
{
    emit upRequested(this);
}

void ConditionalFormatFrame::on_downToolButton_clicked()
{
    emit downRequested(this);
}

void ConditionalFormatFrame::on_functionToolButton_clicked()
{
    FunctionDialog dialog(mTable, this);
    dialog.ui->expressionEdit->setText(ui->conditionEdit->text());

    if (dialog.exec())
    {
        ui->conditionEdit->setText(dialog.ui->expressionEdit->text());
    }
}
