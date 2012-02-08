#include "src/other/global.h"

ComponentTextFrame::ComponentTextFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::ComponentTextFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Текст");
    ui->varNameEdit->setText("Text");

    wordEdit=new WordEditFrame(this);
    ui->userLayout->addWidget(wordEdit);
}

ComponentTextFrame::~ComponentTextFrame()
{
    delete ui;
}

QString ComponentTextFrame::name()
{
    return ui->nameEdit->text();
}

QString ComponentTextFrame::variableName()
{
    return ui->varNameEdit->text();
}

void ComponentTextFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("ComponentText");

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

    QString aValue=wordEdit->ui->valueEdit->toHtml();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void ComponentTextFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Value")
        {
            QString aValue;

            aStream >> aValue;
            wordEdit->ui->valueEdit->setHtml(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void ComponentTextFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
    on_useCheckBox_toggled(ui->useCheckBox->isChecked());
}

void ComponentTextFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void ComponentTextFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void ComponentTextFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void ComponentTextFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void ComponentTextFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void ComponentTextFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->useCheckBox->setText(aName);
}

void ComponentTextFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(checked && (isAdmin || isEditable()));
}

void ComponentTextFrame::on_expandButton_clicked()
{
    FullDialog dialog(this);
    dialog.setWindowTitle(ui->nameEdit->text());

    //------------------------------------------

    ui->mainVerticalLayout->removeWidget(ui->userWidget);

    bool wasVisible=ui->useCheckBox->isChecked();
    ui->useCheckBox->setChecked(true);

    ui->userWidget->setParent(&dialog);
    ui->userWidget->show();
    dialog.ui->verticalLayout->addWidget(ui->userWidget);

    dialog.exec();

    //------------------------------------------

    ui->useCheckBox->setChecked(wasVisible);

    ui->userWidget->setParent(this);
    ui->userWidget->show();
    ui->mainVerticalLayout->addWidget(ui->userWidget);
}

void ComponentTextFrame::on_lockButton_clicked()
{
    ui->userWidget->setEnabled(!ui->userWidget->isEnabled());

    updateLock();
}

void ComponentTextFrame::updateLock()
{
    ui->expandButton->setEnabled(ui->userWidget->isEnabled());

    if (ui->userWidget->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void ComponentTextFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool ComponentTextFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void ComponentTextFrame::checkForErrors(QStringList &aErrorList)
{
    PageComponent::checkForErrors(aErrorList);
}

QVariant ComponentTextFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    return calculationResult;
}

void ComponentTextFrame::getWidgetList(QList<QWidget*> &aList)
{
    aList.append(ui->nameEdit);
    aList.append(ui->varNameEdit);
    aList.append(wordEdit->ui->valueEdit);
}
