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

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void updateLock();

    bool isEditable();

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
    void on_linesTextEdit_textChanged();
    void on_lockButton_clicked();
    void on_editButton_clicked();
};

#endif // VARIABLELISTFRAME_H
