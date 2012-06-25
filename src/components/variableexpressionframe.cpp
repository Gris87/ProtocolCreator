#include "src/other/global.h"

VariableExpressionFrame::VariableExpressionFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExpressionFrame)
{
    ui->setupUi(this);

    init();
}

VariableExpressionFrame::~VariableExpressionFrame()
{
    delete ui;
}

void VariableExpressionFrame::init()
{
    ui->nameEdit->setText("Выражение");
    ui->varNameEdit->setText("Expression");

    ui->editButton->setFlat(true);

    ui->valueEdit->setEnabled(true);
    updateLock();

    ui->valueEdit->setText("");
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

    if (ui->nameEdit->text()!="Выражение")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Expression")
    {
        aStream << QString("VarName");
        aStream << ui->varNameEdit->text();
    }

    if (isEditable())
    {
        bool aEdit=true;

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

void VariableExpressionFrame::loadFromStream(QDataStream &aStream)
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

void VariableExpressionFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->valueLabel->setToolTip(aName);
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

QVariant VariableExpressionFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    calculationResult=calculatePart(ui->valueEdit->text(), aErrorList, this);

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableExpressionFrame::getWidgetList(QList<QWidget*> &aList)
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
