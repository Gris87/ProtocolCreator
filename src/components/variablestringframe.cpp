#include "src/other/global.h"

VariableStringFrame::VariableStringFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableStringFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Строка");
    ui->varNameEdit->setText("String");
}

VariableStringFrame::~VariableStringFrame()
{
    delete ui;
}

void VariableStringFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarString");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    QString aValue=ui->valueEdit->text();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableStringFrame::loadFromStream(QDataStream &aStream)
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
            QString aValue;

            aStream >> aValue;
            ui->valueEdit->setText(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableStringFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableStringFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}
