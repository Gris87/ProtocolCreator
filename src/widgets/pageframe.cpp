#include "src/other/global.h"

PageFrame::PageFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageFrame)
{
    ui->setupUi(this);

    headerText="";
    footerText="";
    isPortaitOrientation=true;
    leftLimit=3;
    topLimit=2;
    rightLimit=1.5;
    bottomLimit=2;
}

PageFrame::~PageFrame()
{
    delete ui;
}

void PageFrame::variableUp(PageComponent* aComponent)
{

}

void PageFrame::variableDown(PageComponent* aComponent)
{

}

void PageFrame::variableCopy(PageComponent* aComponent)
{

}

void PageFrame::variableDelete(PageComponent* aComponent)
{

}

void PageFrame::componentUp(PageComponent* aComponent)
{

}

void PageFrame::componentDown(PageComponent* aComponent)
{

}

void PageFrame::componentCopy(PageComponent* aComponent)
{

}

void PageFrame::componentDelete(PageComponent* aComponent)
{

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

    aComponent->setUpDownEnabled(variables.length()>1, false);

    if (variables.length()>1)
    {
        variables.at(variables.length()-2)->setUpDownEnabled(variables.length()>2, true);
    }

    aComponent->createConnections(this, SLOT(variableUp(PageComponent*)), SLOT(variableDown(PageComponent*)), SLOT(variableCopy(PageComponent*)), SLOT(variableDelete(PageComponent*)));
}

void PageFrame::addComponent(PageComponent* aComponent)
{
    components.append(aComponent);
    ui->componentLayout->addWidget(aComponent);

    aComponent->setUpDownEnabled(components.length()>1, false);

    if (components.length()>1)
    {
        components.at(components.length()-2)->setUpDownEnabled(components.length()>2, true);
    }

    aComponent->createConnections(this, SLOT(componentUp(PageComponent*)), SLOT(componentDown(PageComponent*)), SLOT(componentCopy(PageComponent*)), SLOT(componentDelete(PageComponent*)));
}

void PageFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("Page");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    aStream << QString("Header");
    aStream << headerText;

    aStream << QString("Footer");
    aStream << footerText;

    aStream << QString("Portait");
    aStream << isPortaitOrientation;

    aStream << QString("Limits");
    aStream << leftLimit;
    aStream << topLimit;
    aStream << rightLimit;
    aStream << bottomLimit;

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
        if (aMagicWord=="Header")
        {
            aStream >> headerText;
        }
        else
        if (aMagicWord=="Footer")
        {
            aStream >> footerText;
        }
        else
        if (aMagicWord=="Portait")
        {
            aStream >> isPortaitOrientation;
        }
        else
        if (aMagicWord=="Limits")
        {
            aStream >> leftLimit;
            aStream >> topLimit;
            aStream >> rightLimit;
            aStream >> bottomLimit;
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
                else
                if (aMagicWord=="VarList")
                {
                    aVariable=new VariableListFrame(this);
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

                if (aMagicWord=="ComponentText")
                {
                    aComponent=new ComponentTextFrame(this);
                }

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

    ui->hideWidget->setVisible(isAdmin);

    if (!isAdmin)
    {
        for (int i=0; i<variables.length(); i++)
        {
            if (!variables.at(i)->inherits("VariableExpressionFrame"))
            {
                ui->hideWidget->setVisible(true);
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
