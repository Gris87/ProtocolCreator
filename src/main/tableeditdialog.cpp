#include "src/other/global.h"

TableEditDialog::TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    mTable=aTable;

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
        ui->headerTableWidget->setItem(row, i, new QTableWidgetItem());
    }
}

void TableEditDialog::setItemsForColumn(int column)
{
    for (int i=0; i<ui->headerTableWidget->rowCount(); i++)
    {
        ui->headerTableWidget->setItem(i, column, new QTableWidgetItem());
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

void TableEditDialog::headerLocation()
{

}

void TableEditDialog::headerOffset()
{

}

void TableEditDialog::headerColumnSize()
{

}

void TableEditDialog::headerAlignment()
{

}

void TableEditDialog::headerFont()
{

}

void TableEditDialog::on_headerTableWidget_customContextMenuRequested(const QPoint &pos)
{
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
    contextMenu->addAction("Положение таблицы в тексте", this, SLOT(headerLocation()));
    contextMenu->addAction("Положение в ячейке",         this, SLOT(headerAlignment()));

    contextMenu->setGeometry(cursor().pos().x(),cursor().pos().y(),contextMenu->sizeHint().width(),contextMenu->sizeHint().height());
    contextMenu->show();
}

void TableEditDialog::updateAdmin()
{
    ui->headerAdminWidget->setVisible(isAdmin);
}
