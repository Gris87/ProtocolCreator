#ifndef PAGECOMPONENT_H
#define PAGECOMPONENT_H

#include <QWidget>

#include <QVariant>

class PageComponent : public QWidget
{
    Q_OBJECT
public:
    QWidget *superParent;
    bool isWasCalculated;
    bool isInCalculation;
    QString calculationError;
    QVariant calculationResult;

    explicit PageComponent(QWidget *parent = 0);

    virtual QString name();
    virtual QString variableName();

    virtual void saveToStream(QDataStream &aStream);
    virtual void loadFromStream(QDataStream &aStream);

    virtual void updateAdmin();

    virtual void setUpDownEnabled(bool aUpEnabled, bool aDownEnabled);

    void createConnections(const QObject *receiver, const char *memberUp, const char *memberDown, const char *memberCopy, const char *memberDelete);

    virtual bool isEditable();

    virtual void checkForErrors(QStringList &aErrorList);
    void resetCalculation();
    virtual QVariant calculate();

    virtual void getWidgetList(QList<QWidget*> &aList);
    void setWidgetCursor(QWidget* aWidget, bool isForward);
    bool find(bool isForward);

signals:
    void upPressed(PageComponent *aComponent);
    void downPressed(PageComponent *aComponent);
    void copyPressed(PageComponent *aComponent);
    void deletePressed(PageComponent *aComponent);
};

#endif // PAGECOMPONENT_H
