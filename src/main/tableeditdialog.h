#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>

#include "ui_tableeditdialog.h"

#include "src/components/variableextendedlistframe.h"
#include "src/widgets/cellalignmentwidget.h"

namespace Ui {
    class TableEditDialog;
}

class TableEditDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::TableEditDialog *ui;
    VariableExtendedListFrame *mTable;
    CellAlignmentWidget *mCellAlignmentWidget;
    UnitedTable *activeContextTable;

    explicit TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent = 0);
    ~TableEditDialog();

    void setItemsForRow(int row);
    void setItemsForColumn(int column);
    void setItemsAlignment(int aAlignment);

    void updateAdmin();

private slots:
    void headerInsertRowBefore();
    void headerInsertRowAfter();
    void headerInsertColBefore();
    void headerInsertColAfter();
    void headerColumnSize();
    void headerOffset();
    void tableFont();
    void tableBackgroundColor();
    void tableTextColor();
    void headerLocationLeft();
    void headerLocationCenter();
    void headerLocationRight();
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

    void on_headerAddRowButton_clicked();
    void on_headerDelRowButton_clicked();
    void on_headerAddColButton_clicked();
    void on_headerDelColButton_clicked();
    void on_headerTableWidget_customContextMenuRequested(const QPoint &pos);

    void structureInsertColBefore();
    void structureInsertColAfter();
    void on_structureAddColButton_clicked();
    void on_structureChangeColButton_clicked();
    void on_structureDelColButton_clicked();
    void on_structureTableWidget_customContextMenuRequested(const QPoint &pos);

    void structureTableHeaderMove(int logicalIndex, int oldVisualIndex, int newVisualIndex);
};

#endif // TABLEEDITDIALOG_H
