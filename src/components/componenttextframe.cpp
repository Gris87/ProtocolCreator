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
    ui->userWidget->setVisible(checked);
}
