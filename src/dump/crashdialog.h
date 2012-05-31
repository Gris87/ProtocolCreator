#ifndef CRASHDIALOG_H
#define CRASHDIALOG_H

#include <QDialog>

#include <QTextEdit>

namespace Ui {
    class CrashDialog;
}

class CrashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CrashDialog(QString aReport, QWidget *parent = 0);
    ~CrashDialog();

private slots:
    void on_closeButton_clicked();

private:
    Ui::CrashDialog *ui;
};

#endif // CRASHDIALOG_H
