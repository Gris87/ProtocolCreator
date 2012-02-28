#ifndef FULLDIALOG_H
#define FULLDIALOG_H

#include <QDialog>
#include <QDesktopWidget>

#include "ui_fulldialog.h"

#include "src/components/pagecomponent.h"

namespace Ui {
    class FullDialog;
}

class FullDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::FullDialog *ui;
    PageComponent *pageComponent;

    explicit FullDialog(QWidget *parent = 0);
    ~FullDialog();
};

#endif // FULLDIALOG_H
