#include "src/other/global.h"

TableEditDialog::TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    mTable=aTable;

    mCellAlignmentWidget=new CellAlignmentWidget(this);
    mCellAlignmentWidget->setWindowFlags(Qt::Popup);
    connect(mCellAlignmentWidget->ui->topLeftButton,     SIGNAL(clicked()), this, SLOT(headerCellAlignTopLeft()));
    connect(mCellAlignmentWidget->ui->topButton,         SIGNAL(clicked()), this, SLOT(headerCellAlignTop()));
    connect(mCellAlignmentWidget->ui->topRightButton,    SIGNAL(clicked()), this, SLOT(headerCellAlignTopRight()));
    connect(mCellAlignmentWidget->ui->leftButton,        SIGNAL(clicked()), this, SLOT(headerCellAlignLeft()));
    connect(mCellAlignmentWidget->ui->centerButton,      SIGNAL(clicked()), this, SLOT(headerCellAlignCenter()));
    connect(mCellAlignmentWidget->ui->rightButton,       SIGNAL(clicked()), this, SLOT(headerCellAlignRight()));
    connect(mCellAlignmentWidget->ui->bottomLeftButton,  SIGNAL(clicked()), this, SLOT(headerCellAlignBottomLeft()));
    connect(mCellAlignmentWidget->ui->bottomButton,      SIGNAL(clicked()), this, SLOT(headerCellAlignBottom()));
    connect(mCellAlignmentWidget->ui->bottomRightButton, SIGNAL(clicked()), this, SLOT(headerCellAlignBottomRight()));

    ui->headerWidget->setVisible(mTable->ui->useCheckBox->isVisible());

    updateAdmin();
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}

void TableEditDialog::setItemsForRow(int row)
{
    for (int i=0; i<ui->headerTableWidget->columnCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        ui->headerTableWidget->setItem(row, i, aItem);
    }
}

void TableEditDialog::setItemsForColumn(int column)
{
    for (int i=0; i<ui->headerTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        ui->headerTableWidget->setItem(i, column, aItem);
    }
}

void TableEditDialog::on_headerAddRowButton_clicked()
{
    ui->headerTableWidget->setRowCount(ui->headerTableWidget->rowCount()+1);

    setItemsForRow(ui->headerTableWidget->rowCount()-1);
}

void TableEditDialog::on_headerDelRowButton_clicked()
{
    QList<int> aRows;

    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    if (aItems.length()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите строку");
        return;
    }

    for (int i=0; i<aItems.length(); i++)
    {
        if (!aRows.contains(aItems.at(i)->row()))
        {
            aRows.append(aItems.at(i)->row());
        }
    }

    for (int e=0; e<aRows.length()-1; e++)
    {
        int max=aRows.at(e);
        int maxIndex=e;

        for (int i=e+1; i<aRows.length(); i++)
        {
            if (aRows.at(i)>max)
            {
                max=aRows.at(i);
                maxIndex=i;
            }
        }

        aRows.swap(e, maxIndex);
    }

    for (int i=0; i<aRows.length(); i++)
    {
        ui->headerTableWidget->removeRow(aRows.at(i));
    }
}

void TableEditDialog::on_headerAddColButton_clicked()
{
    ui->headerTableWidget->setColumnCount(ui->headerTableWidget->columnCount()+1);

    setItemsForColumn(ui->headerTableWidget->columnCount()-1);
}

void TableEditDialog::on_headerDelColButton_clicked()
{
    QList<int> aColumns;

    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    if (aItems.length()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    for (int i=0; i<aItems.length(); i++)
    {
        if (!aColumns.contains(aItems.at(i)->column()))
        {
            aColumns.append(aItems.at(i)->column());
        }
    }

    for (int e=0; e<aColumns.length()-1; e++)
    {
        int max=aColumns.at(e);
        int maxIndex=e;

        for (int i=e+1; i<aColumns.length(); i++)
        {
            if (aColumns.at(i)>max)
            {
                max=aColumns.at(i);
                maxIndex=i;
            }
        }

        aColumns.swap(e, maxIndex);
    }

    for (int i=0; i<aColumns.length(); i++)
    {
        ui->headerTableWidget->removeColumn(aColumns.at(i));
    }
}

void TableEditDialog::headerInsertRowBefore()
{
    int row=ui->headerTableWidget->currentRow();

    if (row<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите строку");
        return;
    }

    ui->headerTableWidget->insertRow(row);

    setItemsForRow(row);
}

void TableEditDialog::headerInsertRowAfter()
{
    int row=ui->headerTableWidget->currentRow()+1;

    if (row<=0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите строку");
        return;
    }

    ui->headerTableWidget->insertRow(row);

    setItemsForRow(row);
}

void TableEditDialog::headerInsertColBefore()
{
    int column=ui->headerTableWidget->currentColumn();

    if (column<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    ui->headerTableWidget->insertColumn(column);

    setItemsForColumn(column);
}

void TableEditDialog::headerInsertColAfter()
{
    int column=ui->headerTableWidget->currentColumn()+1;

    if (column<=0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    ui->headerTableWidget->insertColumn(column);

    setItemsForColumn(column);
}

void TableEditDialog::headerUnite()
{
    QList<QTableWidgetSelectionRange> aRanges=ui->headerTableWidget->selectedRanges();

    if (aRanges.length()==0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите ячейку");
        return;
    }

    if (aRanges.length()>1)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите одну зону");
        return;
    }

    ui->headerTableWidget->unite(aRanges.at(0).leftColumn(), aRanges.at(0).topRow(), aRanges.at(0).rightColumn(), aRanges.at(0).bottomRow());
}

void TableEditDialog::headerSeparate()
{
    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    if (aItems.length()>0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите ячейку");
        return;
    }

    for (int i=0; i<aItems.length(); i++)
    {
        ui->headerTableWidget->separate(aItems.at(i)->row(), aItems.at(i)->column());
    }
}

void TableEditDialog::headerColumnSize()
{

}

void TableEditDialog::headerFont()
{

}

void TableEditDialog::headerOffset()
{

}

void TableEditDialog::headerLocationLeft()
{
    mTable->mTableAlignment=Qt::AlignLeft;
}

void TableEditDialog::headerLocationCenter()
{
    mTable->mTableAlignment=Qt::AlignCenter;
}

void TableEditDialog::headerLocationRight()
{
    mTable->mTableAlignment=Qt::AlignRight;
}

void TableEditDialog::headerAlignmentShow()
{
    mCellAlignmentWidget->show();
}

void TableEditDialog::headerAlignmentHide()
{
    mCellAlignmentWidget->hide();
}

void TableEditDialog::setItemsAlignment(int aAlignment)
{
    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    for (int i=0; i<aItems.length(); i++)
    {
        aItems[i]->setTextAlignment(aAlignment);
    }
}

void TableEditDialog::headerCellAlignTopLeft()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void TableEditDialog::headerCellAlignTop()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void TableEditDialog::headerCellAlignTopRight()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignRight);
}

void TableEditDialog::headerCellAlignLeft()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void TableEditDialog::headerCellAlignCenter()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

void TableEditDialog::headerCellAlignRight()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void TableEditDialog::headerCellAlignBottomLeft()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

void TableEditDialog::headerCellAlignBottom()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void TableEditDialog::headerCellAlignBottomRight()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignRight);
}

void TableEditDialog::on_headerTableWidget_customContextMenuRequested(const QPoint &pos)
{
    QAction *aAction;
    QMenu *contextMenu=new QMenu;

    if (isAdmin)
    {
        contextMenu->addAction("Вставить строку перед текущей",   this, SLOT(headerInsertRowBefore()));
        contextMenu->addAction("Вставить строку после текущей",   this, SLOT(headerInsertRowAfter()));
        contextMenu->addAction("Удалить строку(и)",               this, SLOT(on_headerDelRowButton_clicked()));
        contextMenu->addAction("Вставить столбец перед текущим",  this, SLOT(headerInsertColBefore()));
        contextMenu->addAction("Вставить столбец после текущего", this, SLOT(headerInsertColAfter()));
        contextMenu->addAction("Удалить столбец(цы)",             this, SLOT(on_headerDelColButton_clicked()));
        contextMenu->addSeparator();
    }

    contextMenu->addAction("Объединить ячейки",          this, SLOT(headerUnite()));
    contextMenu->addAction("Разъеденить ячейки",         this, SLOT(headerSeparate()));
    contextMenu->addSeparator();
    contextMenu->addAction("Ширина",                     this, SLOT(headerColumnSize()));
    contextMenu->addAction("Шрифт",                      this, SLOT(headerFont()));
    contextMenu->addAction("Сдвиг таблицы",              this, SLOT(headerOffset()));
    contextMenu->addSeparator();

    QMenu *tableAlignMenu=contextMenu->addMenu("Положение таблицы в тексте");

    aAction=tableAlignMenu->addAction("Слева",      this, SLOT(headerLocationLeft()));
    aAction->setCheckable(true);
    aAction->setChecked(mTable->mTableAlignment==Qt::AlignLeft);

    aAction=tableAlignMenu->addAction("Посередине", this, SLOT(headerLocationCenter()));
    aAction->setCheckable(true);
    aAction->setChecked(mTable->mTableAlignment==Qt::AlignCenter);

    aAction=tableAlignMenu->addAction("Справа",     this, SLOT(headerLocationRight()));
    aAction->setCheckable(true);
    aAction->setChecked(mTable->mTableAlignment==Qt::AlignRight);

    QMenu *cellAlignMenu=contextMenu->addMenu("Положение в ячейке");

    if (ui->headerTableWidget->selectedItems().length()==0)
    {
        cellAlignMenu->setEnabled(false);
    }
    else
    {
        int aTextAlignment=ui->headerTableWidget->currentItem()->textAlignment();

        mCellAlignmentWidget->ui->topLeftButton    ->setIcon(aTextAlignment==33  ? QIcon(":/images/CellTopLeftSelected.png")     : QIcon(":/images/CellTopLeft.png"));
        mCellAlignmentWidget->ui->topButton        ->setIcon(aTextAlignment==36  ? QIcon(":/images/CellTopSelected.png")         : QIcon(":/images/CellTop.png"));
        mCellAlignmentWidget->ui->topRightButton   ->setIcon(aTextAlignment==34  ? QIcon(":/images/CellTopRightSelected.png")    : QIcon(":/images/CellTopRight.png"));
        mCellAlignmentWidget->ui->leftButton       ->setIcon(aTextAlignment==129 ? QIcon(":/images/CellLeftSelected.png")        : QIcon(":/images/CellLeft.png"));
        mCellAlignmentWidget->ui->centerButton     ->setIcon(aTextAlignment==132 ? QIcon(":/images/CellCenterSelected.png")      : QIcon(":/images/CellCenter.png"));
        mCellAlignmentWidget->ui->rightButton      ->setIcon(aTextAlignment==130 ? QIcon(":/images/CellRightSelected.png")       : QIcon(":/images/CellRight.png"));
        mCellAlignmentWidget->ui->bottomLeftButton ->setIcon(aTextAlignment==65  ? QIcon(":/images/CellBottomLeftSelected.png")  : QIcon(":/images/CellBottomLeft.png"));
        mCellAlignmentWidget->ui->bottomButton     ->setIcon(aTextAlignment==68  ? QIcon(":/images/CellBottomSelected.png")      : QIcon(":/images/CellBottom.png"));
        mCellAlignmentWidget->ui->bottomRightButton->setIcon(aTextAlignment==66  ? QIcon(":/images/CellBottomRightSelected.png") : QIcon(":/images/CellBottomRight.png"));

        int aWidthSize=mCellAlignmentWidget->width();
        int aHeightSize=mCellAlignmentWidget->height();

        int aX=cursor().pos().x()+200;
        int aY=cursor().pos().y()+240;

        QDesktopWidget *desktop = QApplication::desktop();
        int aWidth = desktop->width();
        int aHeight = desktop->height();

        if (aX+aWidthSize>aWidth)
        {
            aX=aWidth-aWidthSize;
        }

        if (aY+aHeightSize>aHeight)
        {
            aY=aHeight-aHeightSize;
        }

        mCellAlignmentWidget->setGeometry(aX, aY, aWidthSize, aHeightSize);

        connect(cellAlignMenu, SIGNAL(aboutToShow()), this, SLOT(headerAlignmentShow()));
        connect(cellAlignMenu, SIGNAL(aboutToHide()), this, SLOT(headerAlignmentHide()));
    }

    contextMenu->setGeometry(cursor().pos().x(),cursor().pos().y(),contextMenu->sizeHint().width(),contextMenu->sizeHint().height());
    contextMenu->show();
}

void TableEditDialog::updateAdmin()
{
    ui->headerAdminWidget->setVisible(isAdmin);
}
