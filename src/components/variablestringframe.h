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

    void init();

    QString name();
    QString variableName();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

    void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void updateLock();

    bool isEditable();

    QVariant calculate(QStringList *aErrorList=0);

    void getWidgetList(QList<QWidget*> &aList);

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_copyButton_clicked();
    void on_deleteButton_clicked();
    void on_nameEdit_textChanged(const QString &aName);
    void on_varNameEdit_textChanged(const QString &aName);
    void on_lockButton_clicked();
    void on_editButton_clicked();
};

#endif // VARIABLESTRINGFRAME_H
