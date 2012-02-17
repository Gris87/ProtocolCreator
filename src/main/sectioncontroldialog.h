#ifndef SECTIONCONTROLDIALOG_H
#define SECTIONCONTROLDIALOG_H

#include <QDialog>

#include "ui_sectioncontroldialog.h"

#include "src/widgets/wordeditframe.h"
#include "src/widgets/pageframe.h"

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
    QList<PageFrame*> selectedPages;

    explicit SectionControlDialog(QWidget *parent = 0);
    ~SectionControlDialog();

private slots:
    void topSpinBox_valueChanged(double aValue);
    void leftSpinBox_valueChanged(double aValue);
    void bottomSpinBox_valueChanged(double aValue);
    void rightSpinBox_valueChanged(double aValue);
    void header_textChanged();
    void footer_textChanged();

    void on_pagesListWidget_itemSelectionChanged();
    void on_bookButton_clicked();
    void on_albumButton_clicked();
};

#endif // SECTIONCONTROLDIALOG_H
