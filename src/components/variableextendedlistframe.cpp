#include "src/other/global.h"

VariableExtendedListFrame::VariableExtendedListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExtendedListFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("����������� ������");
    ui->varNameEdit->setText("ExtendedList");
}

VariableExtendedListFrame::~VariableExtendedListFrame()
{
    delete ui;
}

QString VariableExtendedListFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableExtendedListFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableExtendedListFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarExtendedList");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aUsed=ui->useCheckBox->isChecked();

    aStream << QString("Used");
    aStream << aUsed;

    bool aEdit=isEditable();

    aStream << QString("Edit");
    aStream << aEdit;

    bool aLock=!ui->userWidget->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    aStream << QString("VarEnd");
}

void VariableExtendedListFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Used")
        {
            bool aUsed;

            aStream >> aUsed;
            ui->useCheckBox->setChecked(aUsed);
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
            ui->userWidget->setEnabled(!aLock);

            updateLock();
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableExtendedListFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
    on_useCheckBox_toggled(ui->useCheckBox->isChecked());
}

void VariableExtendedListFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableExtendedListFrame::on_switchButton_clicked()
{
    emit switchPressed(this);
}

void VariableExtendedListFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableExtendedListFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableExtendedListFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableExtendedListFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableExtendedListFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->useCheckBox->setText(aName);
    ui->titleLabel->setText(aName);
}

void VariableExtendedListFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(checked && (isAdmin || isEditable()));
}

void VariableExtendedListFrame::on_lockButton_clicked()
{
    ui->userWidget->setEnabled(!ui->userWidget->isEnabled());

    updateLock();
}

void VariableExtendedListFrame::updateLock()
{
    if (ui->userWidget->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableExtendedListFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExtendedListFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExtendedListFrame::checkForErrors(QStringList &aErrorList)
{
    PageComponent::checkForErrors(aErrorList);
}

QVariant VariableExtendedListFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    return calculationResult;
}
