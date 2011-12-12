#include "src/other/global.h"

PageFrame::PageFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageFrame)
{
    ui->setupUi(this);
}

PageFrame::~PageFrame()
{
    delete ui;
}

void PageFrame::on_nameEdit_textChanged(const QString &aNewText)
{
    emit nameChanged(this);
}

void PageFrame::on_useCheckBox_toggled(bool checked)
{
    ui->scrollArea->setEnabled(checked);

    emit useToggled(this);
}

void PageFrame::on_hideButton_clicked()
{
    ui->variableWidget->setVisible(!ui->variableWidget->isVisible());

    if (ui->variableWidget->isVisible())
    {
        ui->hideButton->setIcon(QIcon(":/images/up.png"));
    }
    else
    {
        ui->hideButton->setIcon(QIcon(":/images/down.png"));
    }
}

void PageFrame::addVariable(PageComponent* aComponent)
{
    variables.append(aComponent);
    ui->variableLayout->addWidget(aComponent);
}

void PageFrame::addComponent(PageComponent* aComponent)
{
    components.append(aComponent);
    ui->componentLayout->addWidget(aComponent);
}

void PageFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    if (variables.length()>0)
    {
        aStream << QString("Variables");

        for (int i=0; i<variables.length(); i++)
        {
            variables.at(i)->saveToStream(aStream);
        }

        aStream << QString("Stop");
    }

    if (components.length()>0)
    {
        aStream << QString("Components");

        for (int i=0; i<components.length(); i++)
        {
            components.at(i)->saveToStream(aStream);
        }

        aStream << QString("Stop");
    }

    aStream << QString("PageEnd");
}

void PageFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Variables")
        {
            while (!aStream.atEnd())
            {
                aStream >> aMagicWord;

                if (aMagicWord=="Stop")
                {
                    break;
                }

                PageComponent *aVariable=0;

                if (aMagicWord=="VarInteger")
                {
                    aVariable=new VariableIntegerFrame(this);
                }
                else
                if (aMagicWord=="VarString")
                {
                    aVariable=new VariableStringFrame(this);
                }
                else
                if (aMagicWord=="VarBoolean")
                {
                    aVariable=new VariableBoolFrame(this);
                }
                else
                if (aMagicWord=="VarDate")
                {
                    aVariable=new VariableDateFrame(this);
                }
                else
                if (aMagicWord=="VarTime")
                {
                    aVariable=new VariableTimeFrame(this);
                }

                if (aVariable)
                {
                    addVariable(aVariable);
                    aVariable->loadFromStream(aStream);
                }
            }
        }
        else
        if (aMagicWord=="Components")
        {
            while (!aStream.atEnd())
            {
                aStream >> aMagicWord;

                if (aMagicWord=="Stop")
                {
                    break;
                }

                PageComponent *aComponent=0;

                if (aComponent)
                {
                    addComponent(aComponent);
                    aComponent->loadFromStream(aStream);
                }
            }
        }
        else
        if (aMagicWord=="PageEnd")
        {
            break;
        }
    }
}

void PageFrame::updateAdmin()
{
    ui->adminVarNameWidget->setVisible(isAdmin);

    ui->hideButton->setVisible(isAdmin);

    if (!isAdmin)
    {
        for (int i=0; i<variables.length(); i++)
        {
            if (!variables.at(i)->inherits("VariableExpressionFrame"))
            {
                ui->hideButton->setVisible(true);
            }
        }
    }

    for (int i=0; i<variables.length(); i++)
    {
        variables.at(i)->updateAdmin();
    }

    for (int i=0; i<components.length(); i++)
    {
        components.at(i)->updateAdmin();
    }
}
