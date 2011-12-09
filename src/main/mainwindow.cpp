#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    connect(ui->pagesTabWidget->tabBar(), SIGNAL(tabMoved(int,int)), this, SLOT(pageMoved(int,int)));

    loadState();

    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    saveState();

    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentName="";
    isAdmin=true;
    docPass="";
    adminPass="";

    updateHeader();

    while (ui->pagesTabWidget->count()>0)
    {
        delete ui->pagesTabWidget->widget(0);
    }

    contentPage=0;
    addPage("Содержание", "Section_Content");

    updateAdmin();
}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionSaveAs_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionLogin_triggered()
{
    isAdmin=!isAdmin;

    updateAdmin();
}

void MainWindow::on_actionSetDocPass_triggered()
{

}

void MainWindow::on_actionSetAdminPass_triggered()
{

}

void MainWindow::on_actionAddPage_triggered()
{
    addPage("Новый раздел", "");
}

void MainWindow::pageMoved(int from, int to)
{
    QLayoutItem* aItem=contentPage->ui->pageLayout->takeAt(from);

    contentPage->ui->pageLayout->insertWidget(to, aItem->widget());

    delete aItem;
}

void MainWindow::page_nameChanged(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ui->pagesTabWidget->setTabText(pageIndex, parentPage->ui->nameEdit->text());
    ((KnownCheckBox*)contentPage->ui->pageLayout->itemAt(pageIndex)->widget())->setText(parentPage->ui->nameEdit->text());
}

void MainWindow::page_useToggled(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ((KnownCheckBox*)contentPage->ui->pageLayout->itemAt(pageIndex)->widget())->setChecked(parentPage->ui->useCheckBox->isChecked());
}

void MainWindow::contentCheckBoxToggled(KnownCheckBox *aCheckBox, bool checked)
{
    int pageIndex=contentPage->ui->pageLayout->indexOf(aCheckBox);

    if (pageIndex<0)
    {
        return;
    }

    ((PageFrame *)ui->pagesTabWidget->widget(pageIndex))->ui->useCheckBox->setChecked(checked);
}

void MainWindow::on_pagesTabWidget_tabCloseRequested(int index)
{
    if (ui->pagesTabWidget->widget(index)!=contentPage)
    {
        if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить раздел \""+ui->pagesTabWidget->tabText(index)+"\"", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes)
        {
            delete contentPage->ui->pageLayout->itemAt(index)->widget();
            delete contentPage->ui->pageLayout->takeAt(index);
            delete ui->pagesTabWidget->widget(index);
        }
    }
}

void MainWindow::addPage(QString aName, QString aVarName)
{
    PageFrame* aNewPage=new PageFrame(this);
    ui->pagesTabWidget->addTab(aNewPage, aName);

    if (contentPage==0)
    {
        contentPage=aNewPage;
    }

    KnownCheckBox *aCheckBox=new KnownCheckBox(contentPage);
    aCheckBox->setText(aName);
    aCheckBox->setChecked(true);
    contentPage->ui->pageLayout->addWidget(aCheckBox);

    aNewPage->ui->nameEdit->setText(aName);
    aNewPage->ui->varNameEdit->setText(aVarName);

    connect(aCheckBox, SIGNAL(checked(KnownCheckBox*,bool)), this, SLOT(contentCheckBoxToggled(KnownCheckBox*,bool)));
    connect(aNewPage, SIGNAL(nameChanged(PageFrame*)), this, SLOT(page_nameChanged(PageFrame*)));
    connect(aNewPage, SIGNAL(useToggled(PageFrame*)), this, SLOT(page_useToggled(PageFrame*)));
}

void MainWindow::updateHeader()
{
    if (currentName=="")
    {
        setWindowTitle(protocolCreatorVersion);
    }
    else
    {
        setWindowTitle(protocolCreatorVersion+"["+currentName.mid(currentName.lastIndexOf("/")+1)+"]");
    }
}

void MainWindow::updateAdmin()
{
    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        ((PageFrame*)ui->pagesTabWidget->widget(i))->updateAdmin();
    }

    ui->pagesTabWidget->setTabsClosable(isAdmin);

    if (isAdmin)
    {
        ui->actionLogin->setText("Выйти из под администратора");
    }
    else
    {
        ui->actionLogin->setText("Войти под администратором");
    }

    ui->actionSetDocPass->setVisible(isAdmin);
    ui->actionSetAdminPass->setVisible(isAdmin);
    ui->actionAddPage->setVisible(isAdmin);
}

void MainWindow::saveState()
{
    QDir(dir).mkpath(dir+"data");
    QFile::remove(dir+"data/Config.ini");
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);

    aSettings.beginGroup("States");

    aSettings.setValue("Geometry",saveGeometry());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);
    aSettings.beginGroup("States");

    restoreGeometry(aSettings.value("Geometry").toByteArray());

    aSettings.endGroup();
}
