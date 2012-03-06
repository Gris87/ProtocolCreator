#include "src/other/global.h"

ConditionalFormatFrame::ConditionalFormatFrame(VariableExtendedListFrame *aTable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionalFormatFrame)
{
    ui->setupUi(this);

    mTable=aTable;

    mCellAlignmentWidget=new CellAlignmentWidget(this);
    mCellAlignmentWidget->setWindowFlags(Qt::Popup);
    connect(mCellAlignmentWidget->ui->topLeftButton,     SIGNAL(clicked()), this, SLOT(tableCellAlignTopLeft()));
    connect(mCellAlignmentWidget->ui->topButton,         SIGNAL(clicked()), this, SLOT(tableCellAlignTop()));
    connect(mCellAlignmentWidget->ui->topRightButton,    SIGNAL(clicked()), this, SLOT(tableCellAlignTopRight()));
    connect(mCellAlignmentWidget->ui->leftButton,        SIGNAL(clicked()), this, SLOT(tableCellAlignLeft()));
    connect(mCellAlignmentWidget->ui->centerButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignCenter()));
    connect(mCellAlignmentWidget->ui->rightButton,       SIGNAL(clicked()), this, SLOT(tableCellAlignRight()));
    connect(mCellAlignmentWidget->ui->bottomLeftButton,  SIGNAL(clicked()), this, SLOT(tableCellAlignBottomLeft()));
    connect(mCellAlignmentWidget->ui->bottomButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignBottom()));
    connect(mCellAlignmentWidget->ui->bottomRightButton, SIGNAL(clicked()), this, SLOT(tableCellAlignBottomRight()));

    tableCellAlignTopLeft();

    ui->exampleTableWidget->item(0, 0)->setBackground(QBrush(QColor(255, 255, 255)));
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

void ConditionalFormatFrame::tableFont()
{
    QFontDialog dialog(ui->exampleTableWidget->item(0, 0)->font(), this);

    if (dialog.exec())
    {
        ui->exampleTableWidget->item(0, 0)->setFont(dialog.selectedFont());
    }
}

void ConditionalFormatFrame::tableBackgroundColor()
{
    QColorDialog dialog(ui->exampleTableWidget->item(0, 0)->background().color(), this);

    if (dialog.exec())
    {
        QBrush aNewBrush(dialog.selectedColor());
        ui->exampleTableWidget->item(0, 0)->setBackground(aNewBrush);
    }
}

void ConditionalFormatFrame::tableTextColor()
{
    QColorDialog dialog(ui->exampleTableWidget->item(0, 0)->textColor(), this);

    if (dialog.exec())
    {
        ui->exampleTableWidget->item(0, 0)->setTextColor(dialog.selectedColor());
    }
}

void ConditionalFormatFrame::tableAlignmentShow()
{
    mCellAlignmentWidget->show();
}

void ConditionalFormatFrame::tableAlignmentHide()
{
    mCellAlignmentWidget->hide();
}

void ConditionalFormatFrame::setItemsAlignment(int aAlignment)
{
    ui->exampleTableWidget->item(0, 0)->setTextAlignment(aAlignment);
}

void ConditionalFormatFrame::tableCellAlignTopLeft()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void ConditionalFormatFrame::tableCellAlignTop()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void ConditionalFormatFrame::tableCellAlignTopRight()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignRight);
}

void ConditionalFormatFrame::tableCellAlignLeft()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void ConditionalFormatFrame::tableCellAlignCenter()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

void ConditionalFormatFrame::tableCellAlignRight()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void ConditionalFormatFrame::tableCellAlignBottomLeft()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

void ConditionalFormatFrame::tableCellAlignBottom()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void ConditionalFormatFrame::tableCellAlignBottomRight()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignRight);
}

void ConditionalFormatFrame::on_exampleTableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *contextMenu=new QMenu;

    contextMenu->addAction("Шрифт",       this, SLOT(tableFont()));
    contextMenu->addAction("Цвет ячейки", this, SLOT(tableBackgroundColor()));
    contextMenu->addAction("Цвет текста", this, SLOT(tableTextColor()));
    contextMenu->addSeparator();



    QMenu *cellAlignMenu=contextMenu->addMenu("Положение в ячейке");

    int aTextAlignment=ui->exampleTableWidget->item(0, 0)->textAlignment();

    mCellAlignmentWidget->ui->topLeftButton    ->setIcon(aTextAlignment==33  ? QIcon(":/images/CellTopLeftSelected.png")     : QIcon(":/images/CellTopLeft.png"));
    mCellAlignmentWidget->ui->topButton        ->setIcon(aTextAlignment==36  ? QIcon(":/images/CellTopSelected.png")         : QIcon(":/images/CellTop.png"));
    mCellAlignmentWidget->ui->topRightButton   ->setIcon(aTextAlignment==34  ? QIcon(":/images/CellTopRightSelected.png")    : QIcon(":/images/CellTopRight.png"));
    mCellAlignmentWidget->ui->leftButton       ->setIcon(aTextAlignment==129 ? QIcon(":/images/CellLeftSelected.png")        : QIcon(":/images/CellLeft.png"));
    mCellAlignmentWidget->ui->centerButton     ->setIcon(aTextAlignment==132 ? QIcon(":/images/CellCenterSelected.png")      : QIcon(":/images/CellCenter.png"));
    mCellAlignmentWidget->ui->rightButton      ->setIcon(aTextAlignment==130 ? QIcon(":/images/CellRightSelected.png")       : QIcon(":/images/CellRight.png"));
    mCellAlignmentWidget->ui->bottomLeftButton ->setIcon(aTextAlignment==65  ? QIcon(":/images/CellBottomLeftSelected.png")  : QIcon(":/images/CellBottomLeft.png"));
    mCellAlignmentWidget->ui->bottomButton     ->setIcon(aTextAlignment==68  ? QIcon(":/images/CellBottomSelected.png")      : QIcon(":/images/CellBottom.png"));
    mCellAlignmentWidget->ui->bottomRightButton->setIcon(aTextAlignment==66  ? QIcon(":/images/CellBottomRightSelected.png") : QIcon(":/images/CellBottomRight.png"));

    setGeometryInDesktop(mCellAlignmentWidget,
                         cursor().pos().x()+contextMenu->sizeHint().width()-10,
                         cursor().pos().y()+contextMenu->sizeHint().height()-15,
                         mCellAlignmentWidget->width(),
                         mCellAlignmentWidget->height());

    connect(cellAlignMenu, SIGNAL(aboutToShow()), this, SLOT(tableAlignmentShow()));
    connect(cellAlignMenu, SIGNAL(aboutToHide()), this, SLOT(tableAlignmentHide()));



    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}
