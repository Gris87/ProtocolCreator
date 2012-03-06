#ifndef CONDITIONALFORMATFRAME_H
#define CONDITIONALFORMATFRAME_H

#include <QWidget>

#include "ui_conditionalformatframe.h"

#include "src/components/variableextendedlistframe.h"
#include "src/widgets/cellalignmentwidget.h"

namespace Ui {
    class ConditionalFormatFrame;
}

class ConditionalFormatFrame : public QWidget
{
    Q_OBJECT

public:
    Ui::ConditionalFormatFrame *ui;
    VariableExtendedListFrame *mTable;
    CellAlignmentWidget *mCellAlignmentWidget;

    explicit ConditionalFormatFrame(VariableExtendedListFrame *aTable, QWidget *parent = 0);
    ~ConditionalFormatFrame();

    void setItemsAlignment(int aAlignment);

signals:
    void delRequested(ConditionalFormatFrame *);
    void upRequested(ConditionalFormatFrame *);
    void downRequested(ConditionalFormatFrame *);

private slots:
    void on_delToolButton_clicked();
    void on_upToolButton_clicked();
    void on_downToolButton_clicked();
    void on_functionToolButton_clicked();
    void tableFont();
    void tableBackgroundColor();
    void tableTextColor();
    void tableAlignmentShow();
    void tableAlignmentHide();
    void tableCellAlignTopLeft();
    void tableCellAlignTop();
    void tableCellAlignTopRight();
    void tableCellAlignLeft();
    void tableCellAlignCenter();
    void tableCellAlignRight();
    void tableCellAlignBottomLeft();
    void tableCellAlignBottom();
    void tableCellAlignBottomRight();
    void on_exampleTableWidget_customContextMenuRequested(const QPoint &pos);
};

#endif // CONDITIONALFORMATFRAME_H
