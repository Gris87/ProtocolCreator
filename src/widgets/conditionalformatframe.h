#ifndef CONDITIONALFORMATFRAME_H
#define CONDITIONALFORMATFRAME_H

#include <QWidget>

#include "ui_conditionalformatframe.h"

#include "src/components/variableextendedlistframe.h"

namespace Ui {
    class ConditionalFormatFrame;
}

class ConditionalFormatFrame : public QWidget
{
    Q_OBJECT

public:
    Ui::ConditionalFormatFrame *ui;
    VariableExtendedListFrame *mTable;

    explicit ConditionalFormatFrame(QWidget *parent = 0);
    ~ConditionalFormatFrame();

signals:
    void delRequested(ConditionalFormatFrame *);
    void upRequested(ConditionalFormatFrame *);
    void downRequested(ConditionalFormatFrame *);

private slots:
    void on_delToolButton_clicked();
    void on_upToolButton_clicked();
    void on_downToolButton_clicked();
    void on_functionToolButton_clicked();
};

#endif // CONDITIONALFORMATFRAME_H
