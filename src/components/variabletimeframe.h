#ifndef VARIABLETIMEFRAME_H
#define VARIABLETIMEFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variabletimeframe.h"

namespace Ui {
    class VariableTimeFrame;
}

class VariableTimeFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableTimeFrame *ui;

    explicit VariableTimeFrame(QWidget *parent = 0);
    ~VariableTimeFrame();

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
    void on_lockButton_clicked();
};

#endif // VARIABLETIMEFRAME_H
