#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
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

    PageFrame *contentFrame=new PageFrame(this);

    ui->pagesTabWidget->addTab(contentFrame, QString::fromUtf8("Содержание"));

    contentFrame=new PageFrame(this);
    ui->pagesTabWidget->addTab(contentFrame, QString::fromUtf8("Содержание"));

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

}
