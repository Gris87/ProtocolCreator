#ifndef COMPONENTTEXTFRAME_H
#define COMPONENTTEXTFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_componenttextframe.h"

#include "src/widgets/wordeditframe.h"

namespace Ui {
    class ComponentTextFrame;
}

class ComponentTextFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::ComponentTextFrame *ui;
    WordEditFrame *wordEdit;

    explicit ComponentTextFrame(QWidget *parent = 0);
    ~ComponentTextFrame();

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
    void on_useCheckBox_toggled(bool checked);
};

#endif // COMPONENTTEXTFRAME_H
