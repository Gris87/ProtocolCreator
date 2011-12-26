#ifndef VARIABLEEXTENDEDLISTFRAME_H
#define VARIABLEEXTENDEDLISTFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variableextendedlistframe.h"

#include "src/other/types.h"

namespace Ui {
    class VariableExtendedListFrame;
}

class VariableExtendedListFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableExtendedListFrame *ui;
    Qt::AlignmentFlag mTableAlignment;
    double mTableOffset;
    QList<QList<STableCell> > headerCells;
    QList<int> headerColumnWidths;
    QList<STableColumn> typeColumns;

    explicit VariableExtendedListFrame(QWidget *parent = 0);
    ~VariableExtendedListFrame();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void updateLock();

    bool isEditable();

    void checkForErrors(QStringList &aErrorList);
    QVariant calculate();

private slots:
    void on_switchButton_clicked();
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
    void on_useCheckBox_toggled(bool checked);
    void on_lockButton_clicked();
    void on_editButton_clicked();
    void on_configButton_clicked();

signals:
    void switchPressed(VariableExtendedListFrame*  aComponent);
};

#endif // VARIABLEEXTENDEDLISTFRAME_H
