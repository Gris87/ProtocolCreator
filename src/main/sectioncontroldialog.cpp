#include "src/other/global.h"

SectionControlDialog::SectionControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SectionControlDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    headerWordEdit=new WordEditFrame(this);
    ui->userLayout->insertWidget(5,headerWordEdit);

    footerWordEdit=new WordEditFrame(this);
    ui->userLayout->insertWidget(7,footerWordEdit);

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->nameEdit->text());
    }

    ui->pagesListWidget->setCurrentRow(0);
}

SectionControlDialog::~SectionControlDialog()
{
    delete ui;
}

void SectionControlDialog::on_pagesListWidget_itemSelectionChanged()
{
    disconnect(ui->topSpinBox,                SIGNAL(valueChanged(double)), this, SLOT(topSpinBox_valueChanged(double)));
    disconnect(ui->leftSpinBox,               SIGNAL(valueChanged(double)), this, SLOT(leftSpinBox_valueChanged(double)));
    disconnect(ui->bottomSpinBox,             SIGNAL(valueChanged(double)), this, SLOT(bottomSpinBox_valueChanged(double)));
    disconnect(ui->rightSpinBox,              SIGNAL(valueChanged(double)), this, SLOT(rightSpinBox_valueChanged(double)));
    disconnect(headerWordEdit->ui->valueEdit, SIGNAL(textChanged()),        this, SLOT(header_textChanged()));
    disconnect(footerWordEdit->ui->valueEdit, SIGNAL(textChanged()),        this, SLOT(footer_textChanged()));


    selectedPages.clear();

    for (int i=0; i<ui->pagesListWidget->count(); i++)
    {
        if (ui->pagesListWidget->item(i)->isSelected())
        {
            selectedPages.append((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i));
        }
    }

    if (selectedPages.length()==0)
    {
        ui->bookButton->setFlat(true);
        ui->albumButton->setFlat(true);

        ui->topSpinBox->setValue(0);
        ui->leftSpinBox->setValue(0);
        ui->bottomSpinBox->setValue(0);
        ui->rightSpinBox->setValue(0);

        headerWordEdit->ui->valueEdit->clear();
        footerWordEdit->ui->valueEdit->clear();

        return;
    }

    bool isPortait=selectedPages.at(0)->isPortaitOrientation;
    bool good=true;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->isPortaitOrientation!=isPortait)
        {
            good=false;
            break;
        }
    }

    ui->bookButton->setFlat(!good || !isPortait);
    ui->albumButton->setFlat(!good || isPortait);

    double aLimit;

    aLimit=selectedPages.at(0)->leftLimit;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->leftLimit!=aLimit)
        {
            aLimit=0;
            break;
        }
    }

    ui->leftSpinBox->setValue(aLimit);

    aLimit=selectedPages.at(0)->topLimit;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->topLimit!=aLimit)
        {
            aLimit=0;
            break;
        }
    }

    ui->topSpinBox->setValue(aLimit);

    aLimit=selectedPages.at(0)->rightLimit;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->rightLimit!=aLimit)
        {
            aLimit=0;
            break;
        }
    }

    ui->rightSpinBox->setValue(aLimit);

    aLimit=selectedPages.at(0)->bottomLimit;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->bottomLimit!=aLimit)
        {
            aLimit=0;
            break;
        }
    }

    ui->bottomSpinBox->setValue(aLimit);

    QString aText;

    aText=selectedPages.at(0)->headerText;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->headerText!=aText)
        {
            aText="";
            break;
        }
    }

    headerWordEdit->ui->valueEdit->setHtml(aText);

    aText=selectedPages.at(0)->footerText;

    for (int i=1; i<selectedPages.length(); i++)
    {
        if (selectedPages.at(i)->footerText!=aText)
        {
            aText="";
            break;
        }
    }

    footerWordEdit->ui->valueEdit->setHtml(aText);



    connect(ui->topSpinBox,                SIGNAL(valueChanged(double)), this, SLOT(topSpinBox_valueChanged(double)));
    connect(ui->leftSpinBox,               SIGNAL(valueChanged(double)), this, SLOT(leftSpinBox_valueChanged(double)));
    connect(ui->bottomSpinBox,             SIGNAL(valueChanged(double)), this, SLOT(bottomSpinBox_valueChanged(double)));
    connect(ui->rightSpinBox,              SIGNAL(valueChanged(double)), this, SLOT(rightSpinBox_valueChanged(double)));
    connect(headerWordEdit->ui->valueEdit, SIGNAL(textChanged()),        this, SLOT(header_textChanged()));
    connect(footerWordEdit->ui->valueEdit, SIGNAL(textChanged()),        this, SLOT(footer_textChanged()));
}

void SectionControlDialog::on_bookButton_clicked()
{
    ui->bookButton->setFlat(false);
    ui->albumButton->setFlat(true);

    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->isPortaitOrientation=true;
    }
}

void SectionControlDialog::on_albumButton_clicked()
{
    ui->bookButton->setFlat(true);
    ui->albumButton->setFlat(false);

    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->isPortaitOrientation=false;
    }
}

void SectionControlDialog::topSpinBox_valueChanged(double aValue)
{
    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->topLimit=aValue;
    }
}

void SectionControlDialog::leftSpinBox_valueChanged(double aValue)
{
    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->leftLimit=aValue;
    }
}

void SectionControlDialog::bottomSpinBox_valueChanged(double aValue)
{
    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->bottomLimit=aValue;
    }
}

void SectionControlDialog::rightSpinBox_valueChanged(double aValue)
{
    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->rightLimit=aValue;
    }
}

void SectionControlDialog::header_textChanged()
{
    QString aText=headerWordEdit->ui->valueEdit->toHtml();

    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->headerText=aText;
    }
}

void SectionControlDialog::footer_textChanged()
{
    QString aText=footerWordEdit->ui->valueEdit->toHtml();

    for (int i=0; i<selectedPages.length(); i++)
    {
        selectedPages.at(i)->footerText=aText;
    }
}
