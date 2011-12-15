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
    QString headerText;
    QString footerText;
    bool isPortaitOrientation;
    double leftLimit;
    double topLimit;
    double rightLimit;
    double bottomLimit;

    explicit PageFrame(QWidget *parent = 0);
    ~PageFrame();

    void addVariable(PageComponent* aComponent);
    void addComponent(PageComponent* aComponent);

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();
    void updateHideButton();

private slots:
    void variableUp(PageComponent* aComponent);
    void variableDown(PageComponent* aComponent);
    void variableCopy(PageComponent* aComponent);
    void variableDelete(PageComponent* aComponent);
    void componentUp(PageComponent* aComponent);
    void componentDown(PageComponent* aComponent);
    void componentCopy(PageComponent* aComponent);
    void componentDelete(PageComponent* aComponent);

    void on_nameEdit_textChanged(const QString &aNewText);
    void on_useCheckBox_toggled(bool checked);
    void on_hideButton_clicked();

signals:
    void nameChanged(PageFrame* parentPage);
    void useToggled(PageFrame* parentPage);
};

#endif // PAGEFRAME_H
