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

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
    void on_decimalsSpinBox_valueChanged(int aValue);
};

#endif // VARIABLEINTEGERFRAME_H
