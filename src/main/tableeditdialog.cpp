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
    connect(mCellAlignmentWidget->ui->topLeftButton,     SIGNAL(clicked()), this, SLOT(tableCellAlignTopLeft()));
    connect(mCellAlignmentWidget->ui->topButton,         SIGNAL(clicked()), this, SLOT(tableCellAlignTop()));
    connect(mCellAlignmentWidget->ui->topRightButton,    SIGNAL(clicked()), this, SLOT(tableCellAlignTopRight()));
    connect(mCellAlignmentWidget->ui->leftButton,        SIGNAL(clicked()), this, SLOT(tableCellAlignLeft()));
    connect(mCellAlignmentWidget->ui->centerButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignCenter()));
    connect(mCellAlignmentWidget->ui->rightButton,       SIGNAL(clicked()), this, SLOT(tableCellAlignRight()));
    connect(mCellAlignmentWidget->ui->bottomLeftButton,  SIGNAL(clicked()), this, SLOT(tableCellAlignBottomLeft()));
    connect(mCellAlignmentWidget->ui->bottomButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignBottom()));
    connect(mCellAlignmentWidget->ui->bottomRightButton, SIGNAL(clicked()), this, SLOT(tableCellAlignBottomRight()));

    ui->headerWidget->setVisible(mTable->mIsTable);

    ui->headerTableWidget->setStyleSheet( "QTableView { gridline-color: black; }" );

    int rowCount=mTable->headerCells.length();
    int columnCount=rowCount==0? 0 : mTable->headerCells.at(0).length();

    ui->headerTableWidget->setRowCount(rowCount);
    ui->headerTableWidget->setColumnCount(columnCount);

    for (int i=0; i<columnCount; i++)
    {
        ui->headerTableWidget->setColumnWidth(i, mTable->headerColumnWidths.at(i));
    }

    for (int i=0; i<rowCount; i++)
    {
        for (int j=0; j<columnCount; j++)
        {
            QTableWidgetItem *aItem=new QTableWidgetItem();
            STableCell *aCell=&mTable->headerCells[i][j];

            if (aCell->spanX>1 || aCell->spanY>1)
            {
                ui->headerTableWidget->setSpan(i, j, aCell->spanY, aCell->spanX);
            }

            QFont aFont;
            aFont.fromString(aCell->fontString);

            aItem->setFont(aFont);
            aItem->setText(aCell->text);
            aItem->setTextAlignment(aCell->alignment);
            aItem->setBackground(QBrush(QColor(aCell->backgroundColorR, aCell->backgroundColorG, aCell->backgroundColorB)));
            aItem->setTextColor(QColor(aCell->textColorR, aCell->textColorG, aCell->textColorB));

            ui->headerTableWidget->setItem(i, j, aItem);
        }
    }

    QTimer::singleShot(0, this, SLOT(updateHeaderHeights()));

    ui->structureTableWidget->setStyleSheet( "QTableView { gridline-color: black; }" );

    ui->structureTableWidget->setRowCount(2);
    ui->structureTableWidget->setColumnCount(mTable->typeColumns.length());

    for (int i=0; i<mTable->typeColumns.length(); i++)
    {
        STableColumn *aColumn=&mTable->typeColumns[i];

        QTableWidgetItem *aItem;

        aItem=new QTableWidgetItem(aColumn->name);
        aItem->setTextColor(aColumn->visible? QColor(0, 0, 0) : QColor(128, 128, 128));

        if (aColumn->conditions.length()>0)
        {
            QFont aFont=aItem->font();
            aFont.setUnderline(true);
            aItem->setFont(aFont);
        }

        ui->structureTableWidget->setHorizontalHeaderItem(i, aItem);



        aItem=new QTableWidgetItem(aColumn->column->typeDescription());

        QFont aFont;
        aFont.fromString(aColumn->fontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(aColumn->alignment);
        aItem->setBackground(QBrush(QColor(aColumn->backgroundColorR, aColumn->backgroundColorG, aColumn->backgroundColorB)));
        aItem->setTextColor(QColor(aColumn->textColorR, aColumn->textColorG, aColumn->textColorB));

        ui->structureTableWidget->setItem(0, i, aItem);

        aItem=new QTableWidgetItem("Промежуточная строка");

        aFont.fromString(mTable->middleRowFontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(mTable->middleRowAlignment);
        aItem->setBackground(QBrush(mTable->middleRowBackgroundColor));
        aItem->setTextColor(mTable->middleRowTextColor);

        ui->structureTableWidget->setItem(1, i, aItem);

        ui->structureTableWidget->setColumnWidth(i, mTable->typeColumnWidths.at(i));
    }

    if (mTable->typeColumns.length()>0)
    {
        ui->structureTableWidget->unite(0, 1, ui->structureTableWidget->columnCount()-1, 1);
    }

    connect(ui->structureTableWidget->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(structureTableHeaderMove(int,int,int)));

    updateAdmin();
}

TableEditDialog::~TableEditDialog()
{
    mTable->headerCells.clear();
    mTable->headerColumnWidths.clear();

    int rowCount=ui->headerTableWidget->rowCount();
    int columnCount=ui->headerTableWidget->columnCount();

    if (rowCount>0 && columnCount>0)
    {
        for (int i=0; i<columnCount; i++)
        {
            mTable->headerColumnWidths.append(ui->headerTableWidget->columnWidth(i));
        }

        QList<STableCell> aNewRow;

        for (int i=0; i<rowCount; i++)
        {
            mTable->headerCells.append(aNewRow);

            for (int j=0; j<columnCount; j++)
            {
                STableCell aNewCell;
                QTableWidgetItem *aItem=ui->headerTableWidget->item(i, j);

                aNewCell.spanX=ui->headerTableWidget->columnSpan(i, j);
                aNewCell.spanY=ui->headerTableWidget->rowSpan(i, j);
                aNewCell.fontString=aItem->font().toString();
                aNewCell.text=aItem->text();
                aNewCell.alignment=aItem->textAlignment();

                QColor backgroundColor=aItem->background().color();

                aNewCell.backgroundColorR=backgroundColor.red();
                aNewCell.backgroundColorG=backgroundColor.green();
                aNewCell.backgroundColorB=backgroundColor.blue();

                QColor textColor=aItem->textColor();

                aNewCell.textColorR=textColor.red();
                aNewCell.textColorG=textColor.green();
                aNewCell.textColorB=textColor.blue();

                mTable->headerCells[i].append(aNewCell);
            }
        }
    }

    mTable->typeColumnWidths.clear();

    for (int i=0; i<ui->structureTableWidget->columnCount(); i++)
    {
        mTable->typeColumnWidths.append(ui->structureTableWidget->columnWidth(i));

        STableColumn *aColumn=&mTable->typeColumns[i];
        QTableWidgetItem *aItem=ui->structureTableWidget->item(0, i);

        aColumn->fontString=aItem->font().toString();
        aColumn->alignment=aItem->textAlignment();

        QColor aColor;

        aColor=aItem->background().color();

        aColumn->backgroundColorR=aColor.red();
        aColumn->backgroundColorG=aColor.green();
        aColumn->backgroundColorB=aColor.blue();

        aColor=aItem->textColor();

        aColumn->textColorR=aColor.red();
        aColumn->textColorG=aColor.green();
        aColumn->textColorB=aColor.blue();
    }

    if (ui->structureTableWidget->columnCount()>0)
    {
        QTableWidgetItem *aItem=ui->structureTableWidget->item(1, 0);

        mTable->middleRowFontString=aItem->font().toString();
        mTable->middleRowAlignment=aItem->textAlignment();
        mTable->middleRowBackgroundColor=aItem->background().color();
        mTable->middleRowTextColor=aItem->textColor();
    }

    delete ui;
}

void TableEditDialog::setItemsForRow(int row)
{
    QBrush aHeaderBrush(QColor(220, 220, 220));

    for (int i=0; i<ui->headerTableWidget->columnCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setBackground(aHeaderBrush);
        aItem->setFont(QFont("Times New Roman", 12));
        ui->headerTableWidget->setItem(row, i, aItem);
    }
}

void TableEditDialog::setItemsForColumn(int column)
{
    QBrush aHeaderBrush(QColor(220, 220, 220));

    for (int i=0; i<ui->headerTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setBackground(aHeaderBrush);
        aItem->setFont(QFont("Times New Roman", 12));
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
    QList<QTableWidgetSelectionRange> aRanges=ui->headerTableWidget->selectedRanges();

    if (aRanges.length()==0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите строку");
        return;
    }

    QList<int> aRows;

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).bottomRow(); j>=aRanges.at(i).topRow(); j--)
        {
            if (!aRows.contains(j))
            {
                aRows.append(j);
            }
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
    QList<QTableWidgetSelectionRange> aRanges=ui->headerTableWidget->selectedRanges();

    if (aRanges.length()==0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    QList<int> aColumns;

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).rightColumn(); j>=aRanges.at(i).leftColumn(); j--)
        {
            if (!aColumns.contains(j))
            {
                aColumns.append(j);
            }
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

void TableEditDialog::updateHeaderHeights()
{
    ui->headerTableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void TableEditDialog::headerInsertRowBefore()
{
    int row=ui->headerTableWidget->currentRow();

    ui->headerTableWidget->insertRow(row);

    setItemsForRow(row);
}

void TableEditDialog::headerInsertRowAfter()
{
    int row=ui->headerTableWidget->currentRow()+1;

    ui->headerTableWidget->insertRow(row);

    setItemsForRow(row);
}

void TableEditDialog::headerInsertColBefore()
{
    int column=ui->headerTableWidget->currentColumn();

    ui->headerTableWidget->insertColumn(column);

    setItemsForColumn(column);
}

void TableEditDialog::headerInsertColAfter()
{
    int column=ui->headerTableWidget->currentColumn()+1;

    ui->headerTableWidget->insertColumn(column);

    setItemsForColumn(column);
}

void TableEditDialog::headerColumnSize()
{
    ColumnSizeDialog dialog(this);

    dialog.ui->widthSpinBox->setValue(ui->headerTableWidget->columnWidth(ui->headerTableWidget->currentColumn())*PIXEL_TO_SANTIMETER);

    if (dialog.exec())
    {
        QList<QTableWidgetSelectionRange> aRanges=ui->headerTableWidget->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).leftColumn(); j<=aRanges.at(i).rightColumn(); j++)
            {
                ui->headerTableWidget->setColumnWidth(j, (int)(dialog.ui->widthSpinBox->value()/PIXEL_TO_SANTIMETER));
            }
        }
    }
}

void TableEditDialog::headerOffset()
{
    ColumnSizeDialog dialog(this);

    dialog.setWindowTitle("Сдвиг таблицы");
    dialog.ui->titleLabel->setText("Сдвиг:");

    dialog.ui->widthSpinBox->setMinimum(0);
    dialog.ui->widthSpinBox->setValue(mTable->mTableOffset);

    if (dialog.exec())
    {
        mTable->mTableOffset=dialog.ui->widthSpinBox->value();
    }
}

void TableEditDialog::headerCloneToPages()
{
    mTable->cloneHeader=!mTable->cloneHeader;
}

void TableEditDialog::tableFont()
{
    QFontDialog dialog(activeContextTable->currentItem()->font(), this);

    if (dialog.exec())
    {
        QList<QTableWidgetSelectionRange> aRanges=activeContextTable->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    activeContextTable->item(j, k)->setFont(dialog.selectedFont());
                }
            }
        }
    }
}

void TableEditDialog::tableBackgroundColor()
{
    QColorDialog dialog(activeContextTable->currentItem()->background().color(), this);

    if (dialog.exec())
    {
        QBrush aNewBrush(dialog.selectedColor());

        QList<QTableWidgetSelectionRange> aRanges=activeContextTable->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    activeContextTable->item(j, k)->setBackground(aNewBrush);
                }
            }
        }
    }
}

void TableEditDialog::tableTextColor()
{
    QColorDialog dialog(activeContextTable->currentItem()->textColor(), this);

    if (dialog.exec())
    {
        QList<QTableWidgetSelectionRange> aRanges=activeContextTable->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    activeContextTable->item(j, k)->setTextColor(dialog.selectedColor());
                }
            }
        }
    }
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

void TableEditDialog::tableAlignmentShow()
{
    mCellAlignmentWidget->show();
}

void TableEditDialog::tableAlignmentHide()
{
    mCellAlignmentWidget->hide();
}

void TableEditDialog::setItemsAlignment(int aAlignment)
{
    QList<QTableWidgetSelectionRange> aRanges=activeContextTable->selectedRanges();

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
        {
            for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
            {
                activeContextTable->item(j, k)->setTextAlignment(aAlignment);
            }
        }
    }
}

void TableEditDialog::tableCellAlignTopLeft()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void TableEditDialog::tableCellAlignTop()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void TableEditDialog::tableCellAlignTopRight()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignRight);
}

void TableEditDialog::tableCellAlignLeft()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void TableEditDialog::tableCellAlignCenter()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

void TableEditDialog::tableCellAlignRight()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void TableEditDialog::tableCellAlignBottomLeft()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

void TableEditDialog::tableCellAlignBottom()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void TableEditDialog::tableCellAlignBottomRight()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignRight);
}

void TableEditDialog::on_headerTableWidget_customContextMenuRequested(const QPoint &pos)
{
    activeContextTable=ui->headerTableWidget;

    bool itemSelected=activeContextTable->selectedItems().length()>0;

    QAction *aAction;
    QMenu *contextMenu=new QMenu;

    if (isAdmin)
    {
        contextMenu->addAction("Вставить строку перед текущей",   this, SLOT(headerInsertRowBefore()))->setEnabled(itemSelected);
        contextMenu->addAction("Вставить строку после текущей",   this, SLOT(headerInsertRowAfter()))->setEnabled(itemSelected);
        contextMenu->addAction("Удалить строку(и)",               this, SLOT(on_headerDelRowButton_clicked()))->setEnabled(itemSelected);
        contextMenu->addAction("Вставить столбец перед текущим",  this, SLOT(headerInsertColBefore()))->setEnabled(itemSelected);
        contextMenu->addAction("Вставить столбец после текущего", this, SLOT(headerInsertColAfter()))->setEnabled(itemSelected);
        contextMenu->addAction("Удалить столбец(цы)",             this, SLOT(on_headerDelColButton_clicked()))->setEnabled(itemSelected);
        contextMenu->addSeparator();
    }

    contextMenu->addAction("Объединить ячейки",          activeContextTable, SLOT(uniteSelection()))->setEnabled(itemSelected);
    contextMenu->addAction("Разъеденить ячейки",         activeContextTable, SLOT(separateSelection()))->setEnabled(itemSelected);
    contextMenu->addSeparator();
    contextMenu->addAction("Ширина",                     this, SLOT(headerColumnSize()))->setEnabled(itemSelected);
    contextMenu->addAction("Сдвиг таблицы",              this, SLOT(headerOffset()));

    aAction=contextMenu->addAction("Повторять на страницах",     this, SLOT(headerCloneToPages()));
    aAction->setCheckable(true);
    aAction->setChecked(mTable->cloneHeader);

    contextMenu->addSeparator();
    contextMenu->addAction("Шрифт",                      this, SLOT(tableFont()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет ячейки",                this, SLOT(tableBackgroundColor()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет текста",                this, SLOT(tableTextColor()))->setEnabled(itemSelected);
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

    if (itemSelected)
    {
        int aTextAlignment=activeContextTable->currentItem()->textAlignment();

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
    }
    else
    {
        cellAlignMenu->setEnabled(false);
    }

    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}

void TableEditDialog::structureInsertColBefore()
{
    ColumnEditDialog dialog(false, ui->structureTableWidget, mTable, ui->structureTableWidget->currentColumn(), this);
    dialog.exec();
}

void TableEditDialog::structureInsertColAfter()
{
    ColumnEditDialog dialog(false, ui->structureTableWidget, mTable, ui->structureTableWidget->currentColumn()+1, this);
    dialog.exec();
}

void TableEditDialog::on_structureAddColButton_clicked()
{
    ColumnEditDialog dialog(false, ui->structureTableWidget, mTable, ui->structureTableWidget->columnCount(), this);
    dialog.exec();
}

void TableEditDialog::on_structureChangeColButton_clicked()
{
    if (ui->structureTableWidget->currentColumn()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    ColumnEditDialog dialog(true, ui->structureTableWidget, mTable, ui->structureTableWidget->currentColumn(), this);
    dialog.exec();
}

void TableEditDialog::on_structureDelColButton_clicked()
{
    QList<QTableWidgetSelectionRange> aRanges=ui->structureTableWidget->selectedRanges();

    if (aRanges.length()==0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите столбец");
        return;
    }

    QList<int> aColumns;

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).rightColumn(); j>=aRanges.at(i).leftColumn(); j--)
        {
            if (!aColumns.contains(j))
            {
                aColumns.append(j);
            }
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

    if (aColumns.length()==1)
    {
        if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить столбец \""+mTable->typeColumns.at(aColumns.at(0)).name+"\"?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
        {
            return;
        }
    }
    else
    if (aColumns.length()<5)
    {
        if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить "+QString::number(aColumns.length())+" столбца?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
        {
            return;
        }
    }
    else
    {
        if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить "+QString::number(aColumns.length())+" столбцов?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
        {
            return;
        }
    }

    for (int i=0; i<aColumns.length(); i++)
    {
        int realColumn=aColumns.at(i);

        delete mTable->typeColumns.at(realColumn).column;
        mTable->typeColumns.removeAt(realColumn);

        ui->structureTableWidget->removeColumn(realColumn);



        if (mTable->ui->dataTableWidget->itemDelegateForColumn(realColumn))
        {
            delete mTable->ui->dataTableWidget->itemDelegateForColumn(realColumn);
        }

        for (int j=realColumn; j<mTable->ui->dataTableWidget->columnCount()-1; j++)
        {
            mTable->ui->dataTableWidget->setItemDelegateForColumn(j, mTable->ui->dataTableWidget->itemDelegateForColumn(j+1));
        }

        if (realColumn==0 && mTable->ui->dataTableWidget->columnCount()>1)
        {
            for (int j=0; j<mTable->ui->dataTableWidget->rowCount(); j++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(j))
                {
                    mTable->ui->dataTableWidget->item(j, 1)->setText(mTable->ui->dataTableWidget->item(j, 0)->text());
                }
            }
        }

        mTable->ui->dataTableWidget->removeColumn(realColumn);
    }

    if (ui->structureTableWidget->columnCount()>0 && aColumns.contains(0))
    {
        ui->structureTableWidget->item(1,0)->setText("Промежуточная строка");
    }
}

void TableEditDialog::on_structureTableWidget_customContextMenuRequested(const QPoint &pos)
{
    activeContextTable=ui->structureTableWidget;

    bool itemSelected=activeContextTable->selectedItems().length()>0;

    QMenu *contextMenu=new QMenu;

    if (isAdmin)
    {
        contextMenu->addAction("Вставить столбец перед текущим",  this, SLOT(structureInsertColBefore()))->setEnabled(itemSelected);
        contextMenu->addAction("Вставить столбец после текущего", this, SLOT(structureInsertColAfter()))->setEnabled(itemSelected);
        contextMenu->addAction("Удалить столбец(цы)",             this, SLOT(on_structureDelColButton_clicked()))->setEnabled(itemSelected);
        contextMenu->addSeparator();

        QMenu *linkMenu=contextMenu->addMenu("Привязка");

        linkMenu->addAction("Привязать промежуточные строки к списку",      this, SLOT(structureLinkForMiddleRow()));
        linkMenu->addAction("Убрать привязку промежуточных строк",          this, SLOT(structureDelinkForMiddleRow()))->setEnabled(mTable->mLinkForMiddleRow!="");
        linkMenu->addSeparator();
        linkMenu->addAction("Привязать к другому расширенному списку",      this, SLOT(structureLinkForAnotherList()));
        linkMenu->addAction("Убрать привязку с другим расширенным списком", this, SLOT(structureDelinkForAnotherList()))->setEnabled(mTable->mLinkForAnotherList!="");

        contextMenu->addSeparator();
    }

    contextMenu->addAction("Шрифт",                      this, SLOT(tableFont()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет ячейки",                this, SLOT(tableBackgroundColor()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет текста",                this, SLOT(tableTextColor()))->setEnabled(itemSelected);
    contextMenu->addSeparator();

    QMenu *cellAlignMenu=contextMenu->addMenu("Положение в ячейке");

    if (itemSelected)
    {
        int aTextAlignment=activeContextTable->currentItem()->textAlignment();

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
    }
    else
    {
        cellAlignMenu->setEnabled(false);
    }

    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}

void TableEditDialog::structureLinkForMiddleRow()
{
    ListSelectionDialog dialog(false, mTable->mLinkForMiddleRow, this);

    if (dialog.exec())
    {
        mTable->mLinkForMiddleRow=dialog.mResult;

        for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
        {
            if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
            {
                delete mTable->ui->dataTableWidget->itemDelegateForRow(i);
                mTable->ui->dataTableWidget->setItemDelegateForRow(i, new ListDelegate(mTable->mLinkForMiddleRow, mTable));
            }
        }
    }
}

void TableEditDialog::structureDelinkForMiddleRow()
{
    mTable->mLinkForMiddleRow="";

    for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
    {
        if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
        {
            delete mTable->ui->dataTableWidget->itemDelegateForRow(i);
            mTable->ui->dataTableWidget->setItemDelegateForRow(i, new StringDelegate(mTable));
        }
    }
}

void TableEditDialog::structureLinkForAnotherList()
{
    ListSelectionDialog dialog(true, mTable->mLinkForAnotherList, this);

    if (dialog.exec())
    {
        mTable->mLinkForAnotherList=dialog.mResult;
    }
}

void TableEditDialog::structureDelinkForAnotherList()
{
    mTable->mLinkForAnotherList="";
}

void TableEditDialog::on_structureAdditionalButton_clicked()
{
    QMenu *contextMenu=new QMenu;

    contextMenu->addAction("Привязать промежуточные строки к списку",      this, SLOT(structureLinkForMiddleRow()));
    contextMenu->addAction("Убрать привязку промежуточных строк",          this, SLOT(structureDelinkForMiddleRow()))->setEnabled(mTable->mLinkForMiddleRow!="");
    contextMenu->addSeparator();
    contextMenu->addAction("Привязать к другому расширенному списку",      this, SLOT(structureLinkForAnotherList()));
    contextMenu->addAction("Убрать привязку с другим расширенным списком", this, SLOT(structureDelinkForAnotherList()))->setEnabled(mTable->mLinkForAnotherList!="");

    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}

void TableEditDialog::structureTableHeaderMove(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    disconnect(ui->structureTableWidget->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(structureTableHeaderMove(int,int,int)));

    ui->structureTableWidget->horizontalHeader()->moveSection(newVisualIndex, oldVisualIndex);

    mTable->typeColumns.move(oldVisualIndex, newVisualIndex);

    QTableWidgetItem *takenItem=ui->structureTableWidget->takeItem(0, oldVisualIndex);
    int step=oldVisualIndex<newVisualIndex? 1 : -1;

    for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
    {
        ui->structureTableWidget->setItem(0, i, ui->structureTableWidget->takeItem(0, i+step));
    }

    ui->structureTableWidget->setItem(0, newVisualIndex, takenItem);

    takenItem=ui->structureTableWidget->takeHorizontalHeaderItem(oldVisualIndex);

    for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
    {
        ui->structureTableWidget->setHorizontalHeaderItem(i, ui->structureTableWidget->takeHorizontalHeaderItem(i+step));
    }

    ui->structureTableWidget->setHorizontalHeaderItem(newVisualIndex, takenItem);

//------------------------------------------------------------------

    for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
    {
        if (mTable->ui->dataTableWidget->itemDelegateForRow(i)==0)
        {
            takenItem=mTable->ui->dataTableWidget->takeItem(i, oldVisualIndex);

            for (int j=oldVisualIndex; j!=newVisualIndex; j+=step)
            {
                mTable->ui->dataTableWidget->setItem(i, j, mTable->ui->dataTableWidget->takeItem(i, j+step));
            }

            mTable->ui->dataTableWidget->setItem(i, newVisualIndex, takenItem);
        }
    }

    takenItem=mTable->ui->dataTableWidget->takeHorizontalHeaderItem(oldVisualIndex);

    for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
    {
        mTable->ui->dataTableWidget->setHorizontalHeaderItem(i, mTable->ui->dataTableWidget->takeHorizontalHeaderItem(i+step));
    }

    mTable->ui->dataTableWidget->setHorizontalHeaderItem(newVisualIndex, takenItem);

    connect(ui->structureTableWidget->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(structureTableHeaderMove(int,int,int)));
}

void TableEditDialog::on_structureTableWidget_cellDoubleClicked(int row, int column)
{
    if (row==0 && isAdmin)
    {
        ui->structureChangeColButton->click();
    }
}

void TableEditDialog::updateAdmin()
{
    ui->headerAdminWidget->setVisible(isAdmin);
    ui->structureAdminWidget->setVisible(isAdmin);

    ui->structureTableWidget->horizontalHeader()->setMovable(isAdmin);
}
