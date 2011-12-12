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

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
};

#endif // VARIABLEDATEFRAME_H