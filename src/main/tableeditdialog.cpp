#include "src/other/global.h"

TableEditDialog::TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    mTable=aTable;

    updateAdmin();
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}

void TableEditDialog::on_headerAddRowButton_clicked()
{
    ui->headerTableWidget->setRowCount(ui->headerTableWidget->rowCount()+1);

    int row=ui->headerTableWidget->rowCount()-1;

    for (int i=0; i<ui->headerTableWidget->columnCount(); i++)
    {
        ui->headerTableWidget->setItem(row, i, new QTableWidgetItem());
    }
}

void TableEditDialog::on_headerDelRowButton_clicked()
{
    QList<int> aRows;

    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    if (aItems.length()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "�������� ������");
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

    int col=ui->headerTableWidget->columnCount()-1;

    for (int i=0; i<ui->headerTableWidget->rowCount(); i++)
    {
        ui->headerTableWidget->setItem(i, col, new QTableWidgetItem());
    }
}

void TableEditDialog::on_headerDelColButton_clicked()
{
    QList<int> aColumns;

    QList<QTableWidgetItem *> aItems=ui->headerTableWidget->selectedItems();

    if (aItems.length()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "�������� �������");
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

void TableEditDialog::headerUnion()
{

}

void TableEditDialog::headerDeunion()
{

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
        contextMenu->addAction("�������� ������",    this,SLOT(on_headerAddRowButton_clicked()));
        contextMenu->addAction("������� ������(�)",  this,SLOT(on_headerDelRowButton_clicked()));
        contextMenu->addAction("�������� �������",   this,SLOT(on_headerAddColButton_clicked()));
        contextMenu->addAction("������� �������(��)",this,SLOT(on_headerDelColButton_clicked()));
        contextMenu->addSeparator();
    }

    contextMenu->addAction("���������� ������",  this,SLOT(headerUnion()));
    contextMenu->addAction("����������� ������", this,SLOT(headerDeunion()));
    contextMenu->addSeparator();
    contextMenu->addAction("������",this,SLOT(headerColumnSize()));
    contextMenu->addAction("�����",this,SLOT(headerFont()));
    contextMenu->addAction("����� �������",this,SLOT(headerOffset()));
    contextMenu->addSeparator();
    contextMenu->addAction("��������� ������� � ������",this,SLOT(headerLocation()));
    contextMenu->addAction("��������� � ������",this,SLOT(headerAlignment()));

    contextMenu->setGeometry(cursor().pos().x(),cursor().pos().y(),contextMenu->sizeHint().width(),contextMenu->sizeHint().height());
    contextMenu->show();
}

void TableEditDialog::updateAdmin()
{
    ui->headerAdminWidget->setVisible(isAdmin);
}
