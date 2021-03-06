#include "src/other/global.h"

GlobalDialog::GlobalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    addAction(mainWindow->ui->actionFind);
    addAction(mainWindow->ui->actionReplace);
    addAction(mainWindow->ui->actionFindNext);
    addAction(mainWindow->ui->actionFindPrev);

    connect(ui->actionVariableInteger,      SIGNAL(triggered()), this, SLOT(on_numberButton_clicked()));
    connect(ui->actionVariableString,       SIGNAL(triggered()), this, SLOT(on_stringButton_clicked()));
    connect(ui->actionVariableBool,         SIGNAL(triggered()), this, SLOT(on_boolButton_clicked()));
    connect(ui->actionVariableDate,         SIGNAL(triggered()), this, SLOT(on_dateButton_clicked()));
    connect(ui->actionVariableTime,         SIGNAL(triggered()), this, SLOT(on_timeButton_clicked()));
    connect(ui->actionVariableList,         SIGNAL(triggered()), this, SLOT(on_listButton_clicked()));
    connect(ui->actionVariableExtendedList, SIGNAL(triggered()), this, SLOT(on_extendedListButton_clicked()));
    connect(ui->actionVariableExpression,   SIGNAL(triggered()), this, SLOT(on_expressionButton_clicked()));

    addAction(ui->actionVariableInteger);
    addAction(ui->actionVariableString);
    addAction(ui->actionVariableBool);
    addAction(ui->actionVariableDate);
    addAction(ui->actionVariableTime);
    addAction(ui->actionVariableList);
    addAction(ui->actionVariableExtendedList);
    addAction(ui->actionVariableExpression);
}

GlobalDialog::~GlobalDialog()
{
    delete ui;
}

void GlobalDialog::variableUp(PageComponent* aComponent)
{
    int index=variables.indexOf(aComponent);

    if (index==0)
    {
        return;
    }

    variables.swap(index, index-1);

    ui->variableLayout->removeWidget(aComponent);
    ui->variableLayout->insertWidget(index-1, aComponent);

    if (index==variables.length()-1 || index==1)
    {
        aComponent->setUpDownEnabled(index>1, true);
        variables.at(index)->setUpDownEnabled(true, index<variables.length()-1);
    }
}

void GlobalDialog::variableDown(PageComponent* aComponent)
{
    int index=variables.indexOf(aComponent);

    if (index==variables.length()-1)
    {
        return;
    }

    variables.swap(index, index+1);

    ui->variableLayout->removeWidget(aComponent);
    ui->variableLayout->insertWidget(index+1, aComponent);

    if (index==variables.length()-2 || index==0)
    {
        aComponent->setUpDownEnabled(true, index<variables.length()-2);
        variables.at(index)->setUpDownEnabled(index>0,true);
    }
}

void GlobalDialog::variableCopy(PageComponent* aComponent)
{
    copyVariable(aComponent);
}

void GlobalDialog::variableDelete(PageComponent* aComponent)
{
    if (QMessageBox::question(this, protocolCreatorVersion, "�� ������ ������� ���������� \""+aComponent->name()+"\"", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes)
    {
        variables.removeOne(aComponent);

        ui->variableLayout->removeWidget(aComponent);
        delete aComponent;
    }
}

void GlobalDialog::on_numberButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableIntegerFrame(this));
}

void GlobalDialog::on_stringButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableStringFrame(this));
}

void GlobalDialog::on_boolButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableBoolFrame(this));
}

void GlobalDialog::on_dateButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableDateFrame(this));
}

void GlobalDialog::on_timeButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableTimeFrame(this));
}

void GlobalDialog::on_listButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableListFrame(this));
}

void GlobalDialog::on_extendedListButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    VariableExtendedListFrame *aVar=new VariableExtendedListFrame(this);

    aVar->mIsTable=false;
    aVar->ui->useCheckBox->setVisible(false);
    aVar->ui->switchButton->setVisible(false);

    addVariable(aVar);
}

void GlobalDialog::on_expressionButton_clicked()
{
    if (!isAdmin)
    {
        return;
    }

    addVariable(new VariableExpressionFrame(this));
}

void GlobalDialog::addVariable(PageComponent* aComponent)
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

void GlobalDialog::saveToStream(QDataStream &aStream)
{
    aStream << QString("Global");

    if (variables.length()>0)
    {
        aStream << QString("Variables");

        for (int i=0; i<variables.length(); i++)
        {
            variables.at(i)->saveToStream(aStream);
        }

        aStream << QString("Stop");
    }

    aStream << QString("GlobalEnd");
}

void GlobalDialog::loadFromStream(QDataStream &aStream)
{
    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream >> aMagicWord;

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
                else
                if (aMagicWord=="VarExtendedList")
                {
                    aVariable=new VariableExtendedListFrame(this);

                    ((VariableExtendedListFrame*)aVariable)->mIsTable=false;
                    ((VariableExtendedListFrame*)aVariable)->ui->useCheckBox->setVisible(false);
                    ((VariableExtendedListFrame*)aVariable)->ui->switchButton->setVisible(false);
                }
                else
                if (aMagicWord=="VarExpression")
                {
                    aVariable=new VariableExpressionFrame(this);
                }

                if (aVariable)
                {
                    addVariable(aVariable);
                    aVariable->loadFromStream(aStream);
                }
            }
        }
        else
        if (aMagicWord=="GlobalEnd")
        {
            break;
        }
    }
}

void GlobalDialog::updateAdmin()
{
    ui->adminWidget->setVisible(isAdmin);

    for (int i=0; i<variables.length(); i++)
    {
        variables.at(i)->updateAdmin();
        variables.at(i)->setVisible(isAdmin || variables.at(i)->isEditable());
    }
}
