#include "src/other/global.h"

VariableIntegerFrame::VariableIntegerFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableIntegerFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Число");
    ui->varNameEdit->setText("Number");
}

VariableIntegerFrame::~VariableIntegerFrame()
{
    delete ui;
}

void VariableIntegerFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarInteger");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    quint8 aDecimals=ui->valueSpinBox->decimals();

    aStream << QString("Decimals");
    aStream << aDecimals;

    double aValue=ui->valueSpinBox->value();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableIntegerFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Decimals")
        {
            quint8 aDecimals;

            aStream >> aDecimals;
            ui->decimalsSpinBox->setValue(aDecimals);
        }
        else
        if (aMagicWord=="Value")
        {
            double aValue;

            aStream >> aValue;
            ui->valueSpinBox->setValue(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableIntegerFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableIntegerFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableIntegerFrame::on_decimalsSpinBox_valueChanged(int aValue)
{
    ui->valueSpinBox->setDecimals(aValue);
}
