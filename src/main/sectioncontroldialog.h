#ifndef SECTIONCONTROLDIALOG_H
#define SECTIONCONTROLDIALOG_H

#include <QDialog>

#include "ui_sectioncontroldialog.h"

namespace Ui {
    class SectionControlDialog;
}

class SectionControlDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::SectionControlDialog *ui;

    explicit SectionControlDialog(QWidget *parent = 0);
    ~SectionControlDialog();
};

#endif // SECTIONCONTROLDIALOG_H
