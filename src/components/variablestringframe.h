#ifndef VARIABLESTRINGFRAME_H
#define VARIABLESTRINGFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variablestringframe.h"

namespace Ui {
    class VariableStringFrame;
}

class VariableStringFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableStringFrame *ui;

    explicit VariableStringFrame(QWidget *parent = 0);
    ~VariableStringFrame();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
};

#endif // VARIABLESTRINGFRAME_H
