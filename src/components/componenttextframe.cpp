#include "src/other/global.h"

ComponentTextFrame::ComponentTextFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::ComponentTextFrame)
{
    ui->setupUi(this);

    wordEdit=new WordEditFrame(this);
    ui->userLayout->addWidget(wordEdit);

    init();
}

ComponentTextFrame::~ComponentTextFrame()
{
    delete ui;
}

void ComponentTextFrame::init()
{
    ui->nameEdit->setText("Текст");
    ui->varNameEdit->setText("Text");

    ui->useCheckBox->setChecked(true);

    ui->editButton->setFlat(false);

    ui->userWidget->setEnabled(true);
    updateLock();

    wordEdit->ui->valueEdit->setHtml("");
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

    if (ui->nameEdit->text()!="Текст")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="Text")
    {
        aStream << QString("VarName");
        aStream << ui->varNameEdit->text();
    }

    if (!ui->useCheckBox->isChecked())
    {
        bool aUsed=false;

        aStream << QString("Used");
        aStream << aUsed;
    }

    if (!isEditable())
    {
        bool aEdit=false;

        aStream << QString("Edit");
        aStream << aEdit;
    }

    if (!ui->userWidget->isEnabled())
    {
        bool aLock=true;

        aStream << QString("Lock");
        aStream << aLock;
    }

    QString aValue=wordEdit->ui->valueEdit->toHtml();

    if (aValue!="")
    {
        aStream << QString("Value");
        aStream << aValue;
    }

    aStream << QString("VarEnd");
}

void ComponentTextFrame::loadFromStream(QDataStream &aStream)
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
    wordEdit->ui->valueEdit->setReadOnly(!isAdmin && !isEditable());
    wordEdit->ui->toolbarWidget->setVisible(!wordEdit->ui->valueEdit->isReadOnly());
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

void ComponentTextFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->useCheckBox->setToolTip(aName);
}

void ComponentTextFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(ui->userWidget->isEnabled() && checked);
}

void ComponentTextFrame::on_expandButton_clicked()
{
    fullDialog=new FullDialog(this);
    fullDialog->pageComponent=this;

    fullDialog->setWindowTitle(ui->nameEdit->text());

    fullDialog->addAction(mainWindow->ui->actionGlobalVars);
    fullDialog->addAction(mainWindow->ui->actionExport);
    fullDialog->addAction(mainWindow->ui->actionGenerateWord);
    fullDialog->addAction(mainWindow->ui->actionSectionControl);
    fullDialog->addAction(mainWindow->ui->actionSave);
    fullDialog->addAction(mainWindow->ui->actionSaveAs);
    fullDialog->addAction(mainWindow->ui->actionFind);
    fullDialog->addAction(mainWindow->ui->actionReplace);
    fullDialog->addAction(mainWindow->ui->actionFindNext);
    fullDialog->addAction(mainWindow->ui->actionFindPrev);

    //------------------------------------------

    ui->mainVerticalLayout->removeWidget(ui->userWidget);

    bool wasVisible=ui->useCheckBox->isChecked();
    ui->useCheckBox->setChecked(true);

    ui->userWidget->setParent(fullDialog);
    ui->userWidget->show();
    fullDialog->ui->verticalLayout->addWidget(ui->userWidget);

    fullDialog->exec();

    //------------------------------------------

    ui->useCheckBox->setChecked(wasVisible);

    ui->userWidget->setParent(this);
    ui->userWidget->show();
    ui->mainVerticalLayout->addWidget(ui->userWidget);

    delete fullDialog;
    fullDialog=0;

    searchDialog=0;
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

    ui->userWidget->setVisible(ui->userWidget->isEnabled() && ui->useCheckBox->isChecked());
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

QVariant ComponentTextFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

    return calculationResult;
}

void ComponentTextFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin || isEditable())
    {
        if (isAdmin)
        {
            aList.append(ui->nameEdit);
            aList.append(ui->varNameEdit);
        }

        if (ui->useCheckBox->isChecked())
        {
            aList.append(wordEdit->ui->valueEdit);
        }
    }
}
