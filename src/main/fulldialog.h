#ifndef FULLDIALOG_H
#define FULLDIALOG_H

#include <QDialog>
#include <QDesktopWidget>

#include "ui_fulldialog.h"

namespace Ui {
    class FullDialog;
}

class FullDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::FullDialog *ui;

    explicit FullDialog(QWidget *parent = 0);
    ~FullDialog();
};

#endif // FULLDIALOG_H
