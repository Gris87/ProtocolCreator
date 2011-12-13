#ifndef HEADERFOOTERDIALOG_H
#define HEADERFOOTERDIALOG_H

#include <QDialog>

namespace Ui {
    class HeaderFooterDialog;
}

class HeaderFooterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeaderFooterDialog(QWidget *parent = 0);
    ~HeaderFooterDialog();

private:
    Ui::HeaderFooterDialog *ui;
};

#endif // HEADERFOOTERDIALOG_H
