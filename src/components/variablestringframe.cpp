#include "src/other/global.h"

VariableStringFrame::VariableStringFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableStringFrame)
{
    ui->setupUi(this);

    init();
}

VariableStringFrame::~VariableStringFrame()
{
    delete ui;
}

void VariableStringFrame::init()
{
    ui->nameEdit->setText("Строка");
    ui->varNameEdit->setText("String");

    ui->editButton->setFlat(false);

    ui->valueEdit->setEnabled(true);
    updateLock();

    ui->valueEdit->setText("");
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

    if (ui->nameEdit->text()!="Строка")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="String")
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

    if (ui->valueEdit->text()!="")
    {
        QString aValue=ui->valueEdit->text();

        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void VariableStringFrame::loadFromStream(QDataStream &aStream)
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

void VariableStringFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueLabel->setToolTip(aName);
}

void VariableStringFrame::on_lockButton_clicked()
{
    ui->valueEdit->setEnabled(!ui->valueEdit->isEnabled());

    updateLock();
}

void VariableStringFrame::updateLock()
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

void VariableStringFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableStringFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableStringFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=ui->valueEdit->text();

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableStringFrame::getWidgetList(QList<QWidget*> &aList)
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
