#ifndef VARIABLEEXTENDEDLISTFRAME_H
#define VARIABLEEXTENDEDLISTFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variableextendedlistframe.h"

#include "src/other/types.h"
#include "src/widgets/cellalignmentwidget.h"

namespace Ui {
    class VariableExtendedListFrame;
}

enum ECopyRules{crAll, crWithText, crWithoutText};

class VariableExtendedListFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableExtendedListFrame *ui;
    CellAlignmentWidget *mCellAlignmentWidget;

    bool mIsTable;
    Qt::AlignmentFlag mTableAlignment;
    double mTableOffset;
    QString mLinkForMiddleRow;
    QString mLinkForAnotherList;
    QString mLinkForCopyingAnotherList;
    quint8 mCopyColumnCount;
    bool mCopyMiddleRow;
    ECopyRules mCopyRules;
    QStringList mRulesText;
    quint8 mRulesColumn;

    bool needUpdateHeight;

    QList<QList<STableCell> > headerCells;
    QList<int> headerColumnWidths;
    bool cloneHeader;

    QList<STableColumn> typeColumns;
    QList<int> typeColumnWidths;

    QString middleRowFontString;
    quint8 middleRowAlignment;
    QColor middleRowBackgroundColor;
    QColor middleRowTextColor;

    QList<QList<QVariant> > mCellResults;

    explicit VariableExtendedListFrame(QWidget *parent = 0);
    ~VariableExtendedListFrame();

    void init();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

    void componentShown();

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void updateLock();

    bool isEditable();

    void checkLink(QStringList &aErrorList, QString aLink, bool needColumnIndex);
    void checkForErrors(QStringList &aErrorList);
    QVariant calculate(QStringList *aErrorList=0);

    void getWidgetList(QList<QWidget*> &aList);

    void setItemsForRow(int row);
    void setItemsForMiddleRow(int row);
    void insertRow(int row);
    void insertMiddleRow(int row);
    void setItemsAlignment(int aAlignment);

private slots:
    void on_switchButton_clicked();
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
    void on_varNameEdit_textChanged(const QString &aName);
    void on_useCheckBox_toggled(bool checked);
    void on_lockButton_clicked();
    void on_editButton_clicked();
    void on_expandButton_clicked();
    void on_configButton_clicked();
    void on_addRowButton_clicked();
    void dataTableInsertRowBefore();
    void dataTableInsertRowAfter();
    void on_addMiddleRowButton_clicked();
    void dataTableInsertMiddleRowBefore();
    void dataTableInsertMiddleRowAfter();
    void dataTableCopyRows();
    void dataTablePasteBefore();
    void dataTablePasteAfter();
    void on_deleteRowButton_clicked();
    void on_clearButton_clicked();
    void on_addFromAnotherButton_clicked();
    void on_copyFromAnotherTableButton_clicked();
    void dataTableFont();
    void dataTableBackgroundColor();
    void dataTableTextColor();
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
    void on_dataTableWidget_customContextMenuRequested(const QPoint &pos);
    void dataTableHeaderMove(int logicalIndex, int oldVisualIndex, int newVisualIndex);

signals:
    void switchPressed(VariableExtendedListFrame*  aComponent);
};

#endif // VARIABLEEXTENDEDLISTFRAME_H
