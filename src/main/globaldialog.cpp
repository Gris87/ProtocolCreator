#include "src/other/global.h"

GlobalDialog::GlobalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

GlobalDialog::~GlobalDialog()
{
    delete ui;
}

void GlobalDialog::on_numberButton_clicked()
{
    addVariable(new VariableIntegerFrame(this));
}

void GlobalDialog::on_stringButton_clicked()
{
    addVariable(new VariableStringFrame(this));
}

void GlobalDialog::on_boolButton_clicked()
{
    addVariable(new VariableBoolFrame(this));
}

void GlobalDialog::on_dateButton_clicked()
{
    addVariable(new VariableDateFrame(this));
}

void GlobalDialog::on_timeButton_clicked()
{
    addVariable(new VariableTimeFrame(this));
}

void GlobalDialog::on_listButton_clicked()
{
    addVariable(new VariableListFrame(this));
}

void GlobalDialog::on_extendedListButton_clicked()
{

}

void GlobalDialog::on_expressionButton_clicked()
{

}

void GlobalDialog::addVariable(PageComponent* aComponent)
{
    variables.append(aComponent);
    ui->variableLayout->addWidget(aComponent);
}

void GlobalDialog::saveToStream(QDataStream &aStream)
{

}

void GlobalDialog::loadFromStream(QDataStream &aStream)
{

}

void GlobalDialog::updateAdmin()
{
    ui->adminWidget->setVisible(isAdmin);

    for (int i=0; i<variables.length(); i++)
    {
        variables.at(i)->updateAdmin();
    }
}
