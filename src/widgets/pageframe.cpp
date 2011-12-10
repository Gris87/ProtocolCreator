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

void PageFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    aStream << QString("PageEnd");
}

void PageFrame::loadFromStream(QDataStream &aStream)
{
    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream << aMagicWord;

        if (aMagicWord=="Name")
        {
            aStream << aMagicWord;
            ui->nameEdit->setText(aMagicWord);
        }
        else
        if (aMagicWord=="VarName")
        {
            aStream << aMagicWord;
            ui->varNameEdit->setText(aMagicWord);
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
}
