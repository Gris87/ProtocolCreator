#include "cellalignmentwidget.h"

CellAlignmentWidget::CellAlignmentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellAlignmentWidget)
{
    ui->setupUi(this);
}

CellAlignmentWidget::~CellAlignmentWidget()
{
    delete ui;
}
