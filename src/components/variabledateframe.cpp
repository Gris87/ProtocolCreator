#include "src/other/global.h"

VariableDateFrame::VariableDateFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableDateFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Дата");
    ui->varNameEdit->setText("Date");
}

VariableDateFrame::~VariableDateFrame()
{
    delete ui;
}

void VariableDateFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarDate");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    QDate aValue=ui->valueEdit->date();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableDateFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Value")
        {
            QDate aValue;

            aStream >> aValue;
            ui->valueEdit->setDate(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableDateFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableDateFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}
