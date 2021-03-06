#include "src/other/global.h"

VariableIntegerFrame::VariableIntegerFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableIntegerFrame)
{
    ui->setupUi(this);

    init();
}

VariableIntegerFrame::~VariableIntegerFrame()
{
    delete ui;
}

void VariableIntegerFrame::init()
{
    ui->nameEdit->setText("�����");
    ui->varNameEdit->setText("Number");

    ui->decimalsSpinBox->setValue(0);

    ui->editButton->setFlat(false);

    ui->valueSpinBox->setEnabled(true);
    updateLock();

    ui->valueSpinBox->setValue(0);
}

QString VariableIntegerFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableIntegerFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableIntegerFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarInteger");

    if (ui->nameEdit->text()!="�����")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Number")
    {
        aStream << QString("VarName");
        aStream << ui->varNameEdit->text();
    }

    if (ui->decimalsSpinBox->value()!=0)
    {
        quint8 aDecimals=ui->decimalsSpinBox->value();

        aStream << QString("Decimals");
        aStream << aDecimals;
    }

    if (!isEditable())
    {
        bool aEdit=false;

        aStream << QString("Edit");
        aStream << aEdit;
    }

    if (!ui->valueSpinBox->isEnabled())
    {
        bool aLock=true;

        aStream << QString("Lock");
        aStream << aLock;
    }

    if (ui->valueSpinBox->value()!=0)
    {
        double aValue=ui->valueSpinBox->value();

        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void VariableIntegerFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Decimals")
        {
            quint8 aDecimals;

            aStream >> aDecimals;
            ui->decimalsSpinBox->setValue(aDecimals);
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
            ui->valueSpinBox->setEnabled(!aLock);

            updateLock();
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

void VariableIntegerFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableIntegerFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableIntegerFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableIntegerFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableIntegerFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableIntegerFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableIntegerFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueLabel->setToolTip(aName);
}

void VariableIntegerFrame::on_decimalsSpinBox_valueChanged(int aValue)
{
    ui->valueSpinBox->setDecimals(aValue);
}

void VariableIntegerFrame::on_lockButton_clicked()
{
    ui->valueSpinBox->setEnabled(!ui->valueSpinBox->isEnabled());

    updateLock();
}

void VariableIntegerFrame::updateLock()
{
    ui->decimalsSpinBox->setEnabled(ui->valueSpinBox->isEnabled());

    if (ui->valueSpinBox->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableIntegerFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableIntegerFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableIntegerFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=ui->valueSpinBox->value();

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableIntegerFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin || isEditable())
    {
        if (isAdmin)
        {
            aList.append(ui->nameEdit);
            aList.append(ui->varNameEdit);
            aList.append(ui->decimalsSpinBox);
        }

        aList.append(ui->valueSpinBox);
    }
}
