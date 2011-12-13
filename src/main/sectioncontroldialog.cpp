#include "src/other/global.h"

SectionControlDialog::SectionControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SectionControlDialog)
{
    ui->setupUi(this);

    headerWordEdit=new WordEditFrame(this);
    ui->userLayout->insertWidget(5,headerWordEdit);

    footerWordEdit=new WordEditFrame(this);
    ui->userLayout->insertWidget(7,footerWordEdit);

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->nameEdit->text());
    }
}

SectionControlDialog::~SectionControlDialog()
{
    delete ui;
}

void SectionControlDialog::on_pagesListWidget_itemSelectionChanged()
{

}
