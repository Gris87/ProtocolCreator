#include "src/other/global.h"

VariableBoolFrame::VariableBoolFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableBoolFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Логическое значение");
    ui->varNameEdit->setText("Boolean");
}

VariableBoolFrame::~VariableBoolFrame()
{
    delete ui;
}

void VariableBoolFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarBoolean");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aValue=ui->valueCheckBox->isChecked();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableBoolFrame::loadFromStream(QDataStream &aStream)
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
            bool aValue;

            aStream >> aValue;
            ui->valueCheckBox->setChecked(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableBoolFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableBoolFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueCheckBox->setText(aName);
}
