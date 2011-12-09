#include "src/other/global.h"

PageFrame::PageFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageFrame)
{
    ui->setupUi(this);
}

PageFrame::~PageFrame()
{
    delete ui;
}

void PageFrame::on_nameEdit_textChanged(const QString &aNewText)
{
    emit nameChanged(this);
}

void PageFrame::on_useCheckBox_toggled(bool checked)
{
    ui->scrollArea->setEnabled(checked);

    emit useToggled(this);
}

void PageFrame::updateAdmin()
{
    ui->adminVarNameWidget->setVisible(isAdmin);
}
