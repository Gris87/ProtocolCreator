#ifndef PAGECOMPONENT_H
#define PAGECOMPONENT_H

#include <QWidget>

class PageComponent : public QWidget
{
    Q_OBJECT
public:
    explicit PageComponent(QWidget *parent = 0);

    virtual void saveToStream(QDataStream &aStream);
    virtual void loadFromStream(QDataStream &aStream);

    virtual void updateAdmin();
};

#endif // PAGECOMPONENT_H
