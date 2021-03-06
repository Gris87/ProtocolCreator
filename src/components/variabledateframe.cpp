#include "src/other/global.h"

VariableDateFrame::VariableDateFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableDateFrame)
{
    ui->setupUi(this);

    init();
}

VariableDateFrame::~VariableDateFrame()
{
    delete ui;
}

void VariableDateFrame::init()
{
    ui->nameEdit->setText("����");
    ui->varNameEdit->setText("Date");

    ui->editButton->setFlat(false);

    ui->valueEdit->setEnabled(true);
    updateLock();

    ui->valueEdit->setDate(QDate(2000, 1, 1));
}

QString VariableDateFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableDateFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableDateFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarDate");

    if (ui->nameEdit->text()!="����")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Date")
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

    if (ui->valueEdit->date()!=QDate(2000, 1, 1))
    {
        QDate aValue=ui->valueEdit->date();

        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void VariableDateFrame::loadFromStream(QDataStream &aStream)
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

void VariableDateFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableDateFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableDateFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableDateFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableDateFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableDateFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableDateFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueLabel->setToolTip(aName);
}

void VariableDateFrame::on_lockButton_clicked()
{
    ui->valueEdit->setEnabled(!ui->valueEdit->isEnabled());

    updateLock();
}

void VariableDateFrame::updateLock()
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

void VariableDateFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableDateFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableDateFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=ui->valueEdit->date();

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableDateFrame::getWidgetList(QList<QWidget*> &aList)
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
