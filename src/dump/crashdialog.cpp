#include "crashdialog.h"
#include "ui_crashdialog.h"

CrashDialog::CrashDialog(QString aReport,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrashDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    aReport.remove("\r");
    aReport.replace("\n", "<br>");
    aReport.replace(" ", "<span style=\"color:#ffffff;\">.</span>");

    aReport.replace("Registers:<br>", "<span style=\"font-weight:600;\">Registers:</span><br>");
    aReport.replace("Call<span style=\"color:#ffffff;\">.</span>stack:<br>", "<span style=\"font-weight:600;\">Call<span style=\"color:#ffffff;\">.</span>stack:</span><br>");
    aReport.replace("Logs:<br>", "<span style=\"font-weight:600;\">Logs:</span><br>");

    ui->dumpTextEdit->setHtml(aReport);
}

CrashDialog::~CrashDialog()
{
    delete ui;
}

void CrashDialog::on_closeButton_clicked()
{
    reject();
}
