#ifndef SECTIONCONTROLDIALOG_H
#define SECTIONCONTROLDIALOG_H

#include <QDialog>

#include "ui_sectioncontroldialog.h"

#include "src/widgets/wordeditframe.h"

namespace Ui {
    class SectionControlDialog;
}

class SectionControlDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::SectionControlDialog *ui;
    WordEditFrame *headerWordEdit;
    WordEditFrame *footerWordEdit;

    explicit SectionControlDialog(QWidget *parent = 0);
    ~SectionControlDialog();
private slots:
    void on_pagesListWidget_itemSelectionChanged();
};

#endif // SECTIONCONTROLDIALOG_H
