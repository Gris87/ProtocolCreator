#include "src/other/global.h"

VariableListFrame::VariableListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableListFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Список");
    ui->varNameEdit->setText("List");
}

VariableListFrame::~VariableListFrame()
{
    delete ui;
}

QString VariableListFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableListFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableListFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarList");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    QString aItems="";

    for (int i=0; i<ui->valueComboBox->count(); i++)
    {
        if (i>0)
        {
            aItems.append("\n");
        }

        aItems.append(ui->valueComboBox->itemText(i));
    }

    aStream << QString("Items");
    aStream << aItems;

    bool aEdit=isEditable();

    aStream << QString("Edit");
    aStream << aEdit;

    bool aLock=!ui->valueComboBox->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    QString aValue=ui->valueComboBox->currentText();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableListFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Items")
        {
            QString aItems;

            aStream >> aItems;

            ui->linesTextEdit->setPlainText(aItems);
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
            ui->valueComboBox->setEnabled(!aLock);

            updateLock();
        }
        else
        if (aMagicWord=="Value")
        {
            QString aValue;

            aStream >> aValue;

            int index=ui->valueComboBox->findText(aValue);

            if (index<0)
            {
                ui->valueComboBox->setEditText(aValue);
            }
            else
            {
                ui->valueComboBox->setCurrentIndex(index);
            }
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableListFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableListFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableListFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableListFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableListFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableListFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableListFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableListFrame::on_linesTextEdit_textChanged()
{
    QString aValue=ui->valueComboBox->currentText();

    QStringList aItems=ui->linesTextEdit->toPlainText().split("\n");

    aItems.removeDuplicates();
    aItems.removeOne("");
    aItems.sort();

    ui->valueComboBox->clear();
    ui->valueComboBox->addItems(aItems);

    int index=ui->valueComboBox->findText(aValue);

    if (index<0)
    {
        ui->valueComboBox->setEditText(aValue);
    }
    else
    {
        ui->valueComboBox->setCurrentIndex(index);
    }
}

void VariableListFrame::on_lockButton_clicked()
{
    ui->valueComboBox->setEnabled(!ui->valueComboBox->isEnabled());

    updateLock();
}

void VariableListFrame::updateLock()
{
    if (ui->valueComboBox->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableListFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableListFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

QVariant VariableListFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    calculationResult=ui->valueComboBox->currentText();

    mCalculationList.clear();

    for (int i=0; i<ui->valueComboBox->count(); i++)
    {
        mCalculationList.append(ui->valueComboBox->itemText(i));
    }

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableListFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin || isEditable())
    {
        if (isAdmin)
        {
            aList.append(ui->nameEdit);
            aList.append(ui->varNameEdit);
            aList.append(ui->linesTextEdit);
        }

        aList.append(ui->valueComboBox);
    }
}
