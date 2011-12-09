#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    setWindowTitle(protocolCreatorVersion);

    DecryptString("HELLOW", "sdf");
}

MainWindow::~MainWindow()
{
    delete ui;
}
