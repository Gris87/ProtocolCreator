#ifndef VARIABLEBOOLFRAME_H
#define VARIABLEBOOLFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variableboolframe.h"

namespace Ui {
    class VariableBoolFrame;
}

class VariableBoolFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableBoolFrame *ui;

    explicit VariableBoolFrame(QWidget *parent = 0);
    ~VariableBoolFrame();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
};

#endif // VARIABLEBOOLFRAME_H
