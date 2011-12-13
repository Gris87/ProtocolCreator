#ifndef VARIABLELISTFRAME_H
#define VARIABLELISTFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variablelistframe.h"

namespace Ui {
    class VariableListFrame;
}

class VariableListFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableListFrame *ui;

    explicit VariableListFrame(QWidget *parent = 0);
    ~VariableListFrame();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
    void on_linesTextEdit_textChanged();
};

#endif // VARIABLELISTFRAME_H
