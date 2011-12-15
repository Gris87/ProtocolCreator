#include "src/other/global.h"

VariableTimeFrame::VariableTimeFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableTimeFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Время");
    ui->varNameEdit->setText("Time");
}

VariableTimeFrame::~VariableTimeFrame()
{
    delete ui;
}

QString VariableTimeFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableTimeFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableTimeFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarTime");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aLock=!ui->valueEdit->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    QTime aValue=ui->valueEdit->time();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableTimeFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Lock")
        {
            bool aLock;

            aStream >> aLock;
            ui->valueEdit->setEnabled(!aLock);

            updateLock();
        }
        else
        if (aMagicWord=="Value")
        {
            QTime aValue;

            aStream >> aValue;
            ui->valueEdit->setTime(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableTimeFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableTimeFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableTimeFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableTimeFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableTimeFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableTimeFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableTimeFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableTimeFrame::on_lockButton_clicked()
{
    ui->valueEdit->setEnabled(!ui->valueEdit->isEnabled());

    updateLock();
}

void VariableTimeFrame::updateLock()
{
    if (ui->valueEdit->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}
