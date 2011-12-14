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

QString VariableStringFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableStringFrame::variableName()
{
    return ui->varNameEdit->text();
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

void VariableStringFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableStringFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableStringFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableStringFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableStringFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableStringFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}
