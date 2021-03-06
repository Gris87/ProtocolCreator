#include "src/other/global.h"

VariableBoolFrame::VariableBoolFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableBoolFrame)
{
    ui->setupUi(this);

    init();
}

VariableBoolFrame::~VariableBoolFrame()
{
    delete ui;
}

void VariableBoolFrame::init()
{
    ui->nameEdit->setText("���������� ��������");
    ui->varNameEdit->setText("Boolean");

    ui->editButton->setFlat(false);

    ui->valueCheckBox->setEnabled(true);
    updateLock();

    ui->valueCheckBox->setChecked(false);
}

QString VariableBoolFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableBoolFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableBoolFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarBoolean");

    if (ui->nameEdit->text()!="���������� ��������")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Boolean")
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

    if (!ui->valueCheckBox->isEnabled())
    {
        bool aLock=true;

        aStream << QString("Lock");
        aStream << aLock;
    }

    if (ui->valueCheckBox->isChecked())
    {
        bool aValue=true;

        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void VariableBoolFrame::loadFromStream(QDataStream &aStream)
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
            ui->valueCheckBox->setEnabled(!aLock);

            updateLock();
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

void VariableBoolFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableBoolFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableBoolFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableBoolFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableBoolFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableBoolFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueCheckBox->setText(aName);
}

void VariableBoolFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueCheckBox->setToolTip(aName);
}

void VariableBoolFrame::on_lockButton_clicked()
{
    ui->valueCheckBox->setEnabled(!ui->valueCheckBox->isEnabled());

    updateLock();
}

void VariableBoolFrame::updateLock()
{
    if (ui->valueCheckBox->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableBoolFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableBoolFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableBoolFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=ui->valueCheckBox->isChecked();

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableBoolFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin)
    {
        aList.append(ui->nameEdit);
        aList.append(ui->varNameEdit);
    }
}
