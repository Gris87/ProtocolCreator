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

private slots:
    void on_nameEdit_textChanged(const QString &aName);
};

#endif // VARIABLETIMEFRAME_H
