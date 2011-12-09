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
