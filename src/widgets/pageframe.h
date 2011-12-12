#ifndef PAGEFRAME_H
#define PAGEFRAME_H

#include <QWidget>

#include "ui_pageframe.h"

#include "src/components/pagecomponent.h"

namespace Ui {
    class PageFrame;
}

class PageFrame : public QWidget
{
    Q_OBJECT

public:
    Ui::PageFrame *ui;
    QList<PageComponent*> variables;
    QList<PageComponent*> components;

    explicit PageFrame(QWidget *parent = 0);
    ~PageFrame();

    void addVariable(PageComponent* aComponent);
    void addComponent(PageComponent* aComponent);

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aNewText);
    void on_useCheckBox_toggled(bool checked);
    void on_hideButton_clicked();

signals:
    void nameChanged(PageFrame* parentPage);
    void useToggled(PageFrame* parentPage);
};

#endif // PAGEFRAME_H
