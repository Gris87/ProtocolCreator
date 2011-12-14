#ifndef VARIABLEDATEFRAME_H
#define VARIABLEDATEFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_variabledateframe.h"

namespace Ui {
    class VariableDateFrame;
}

class VariableDateFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::VariableDateFrame *ui;

    explicit VariableDateFrame(QWidget *parent = 0);
    ~VariableDateFrame();

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

#endif // VARIABLEDATEFRAME_H
