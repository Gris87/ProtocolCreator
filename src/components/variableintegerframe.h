#ifndef VARIABLEINTEGERFRAME_H
#define VARIABLEINTEGERFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variableintegerframe.h"

namespace Ui {
    class VariableIntegerFrame;
}

class VariableIntegerFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableIntegerFrame *ui;

    explicit VariableIntegerFrame(QWidget *parent = 0);
    ~VariableIntegerFrame();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void updateLock();

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
    void on_decimalsSpinBox_valueChanged(int aValue);
    void on_lockButton_clicked();
};

#endif // VARIABLEINTEGERFRAME_H
