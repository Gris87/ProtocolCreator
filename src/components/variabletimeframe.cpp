#include "src/other/global.h"

VariableTimeFrame::VariableTimeFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableTimeFrame)
{
    ui->setupUi(this);

    init();
}

VariableTimeFrame::~VariableTimeFrame()
{
    delete ui;
}

void VariableTimeFrame::init()
{
    ui->nameEdit->setText("Время");
    ui->varNameEdit->setText("Time");

    ui->editButton->setFlat(false);

    ui->valueEdit->setEnabled(true);
    updateLock();

    ui->valueEdit->setTime(QTime(0, 0));
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

    if (ui->nameEdit->text()!="Время")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Time")
    {
        aStream << QString("VarName");
        aStream << ui->varNameEdit->text();
    }

    if (!isEditable())
    {
        bool aEdit=false;

        aStream << QString("Edit");
        aStream << aEdit;
    }

    if (!ui->valueEdit->isEnabled())
    {
        bool aLock=true;

        aStream << QString("Lock");
        aStream << aLock;
    }

    if (ui->valueEdit->time()!=QTime(0, 0))
    {
        QTime aValue=ui->valueEdit->time();

        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void VariableTimeFrame::loadFromStream(QDataStream &aStream)
{
    init();

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
        if (aMagicWord=="Edit")
        {
            bool aEdit;

            aStream >> aEdit;
            ui->editButton->setFlat(!aEdit);
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

void VariableTimeFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueLabel->setToolTip(aName);
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

void VariableTimeFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableTimeFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableTimeFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=ui->valueEdit->time();

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableTimeFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin || isEditable())
    {
        if (isAdmin)
        {
            aList.append(ui->nameEdit);
            aList.append(ui->varNameEdit);
        }

        aList.append(ui->valueEdit);
    }
}
