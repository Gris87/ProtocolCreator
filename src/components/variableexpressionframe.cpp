#include "src/other/global.h"

VariableExpressionFrame::VariableExpressionFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExpressionFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Выражение");
    ui->varNameEdit->setText("Expression");
}

VariableExpressionFrame::~VariableExpressionFrame()
{
    delete ui;
}

QString VariableExpressionFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableExpressionFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableExpressionFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarExpression");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aEdit=isEditable();

    aStream << QString("Edit");
    aStream << aEdit;

    bool aLock=!ui->valueEdit->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    QString aValue=ui->valueEdit->text();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableExpressionFrame::loadFromStream(QDataStream &aStream)
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

void VariableExpressionFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableExpressionFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableExpressionFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableExpressionFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableExpressionFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableExpressionFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableExpressionFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableExpressionFrame::on_lockButton_clicked()
{
    ui->valueEdit->setEnabled(!ui->valueEdit->isEnabled());

    updateLock();
}

void VariableExpressionFrame::updateLock()
{
    ui->functionButton->setEnabled(ui->valueEdit->isEnabled());

    if (ui->valueEdit->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableExpressionFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExpressionFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExpressionFrame::on_functionButton_clicked()
{
    FunctionDialog dialog(0, this);
    dialog.ui->expressionEdit->setText(ui->valueEdit->text());

    if (dialog.exec())
    {
        ui->valueEdit->setText(dialog.ui->expressionEdit->text());
    }
}

QVariant VariableExpressionFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    calculationResult=calculatePart(ui->valueEdit->text(), this);

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}
