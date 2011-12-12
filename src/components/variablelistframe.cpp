#include "src/other/global.h"

VariableListFrame::VariableListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableListFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("������");
    ui->varNameEdit->setText("List");
}

VariableListFrame::~VariableListFrame()
{
    delete ui;
}

void VariableListFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarList");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    QString aItems="";

    for (int i=0; i<ui->valueComboBox->count(); i++)
    {
        if (i>0)
        {
            aItems.append("\n");
        }

        aItems.append(ui->valueComboBox->itemText(i));
    }

    aStream << QString("Items");
    aStream << aItems;

    QString aValue=ui->valueComboBox->currentText();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableListFrame::loadFromStream(QDataStream &aStream)
{
    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream >> aMagicWord;

        if (aMagicWord=="Name")
        {
            aStream >> aMagicWord;
            ui->nameEdit->setText(aMagicWord);
        }
        else
        if (aMagicWord=="VarName")
        {
            aStream >> aMagicWord;
            ui->varNameEdit->setText(aMagicWord);
        }
        else
        if (aMagicWord=="Items")
        {
            QString aItems;

            aStream >> aItems;

            ui->linesTextEdit->setPlainText(aItems);
        }
        else
        if (aMagicWord=="Value")
        {
            QString aValue;

            aStream >> aValue;

            int index=ui->valueComboBox->findText(aValue);

            if (index<0)
            {
                ui->valueComboBox->setEditText(aValue);
            }
            else
            {
                ui->valueComboBox->setCurrentIndex(index);
            }
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableListFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableListFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableListFrame::on_linesTextEdit_textChanged()
{
    QString aValue=ui->valueComboBox->currentText();

    QStringList aItems=ui->linesTextEdit->toPlainText().split("\n");

    aItems.removeDuplicates();
    aItems.removeOne("");

    ui->valueComboBox->clear();
    ui->valueComboBox->addItems(aItems);

    int index=ui->valueComboBox->findText(aValue);

    if (index<0)
    {
        ui->valueComboBox->setEditText(aValue);
    }
    else
    {
        ui->valueComboBox->setCurrentIndex(index);
    }
}