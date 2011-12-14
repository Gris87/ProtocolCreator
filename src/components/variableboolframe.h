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

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
};

#endif // VARIABLEBOOLFRAME_H
