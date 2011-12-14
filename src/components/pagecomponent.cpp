#include "src/other/global.h"

PageComponent::PageComponent(QWidget *parent) :
    QWidget(parent)
{
}

QString PageComponent::name()
{
    return "";
}

QString PageComponent::variableName()
{
    return "";
}

void PageComponent::saveToStream(QDataStream &aStream)
{
    // Nothing
}

void PageComponent::loadFromStream(QDataStream &aStream)
{
    // Nothing
}

void PageComponent::updateAdmin()
{
    // Nothing
}

void PageComponent::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    // Nothing
}

void PageComponent::createConnections(const QObject *receiver, const char *memberUp, const char *memberDown, const char *memberCopy, const char *memberDelete)
{
    connect(this, SIGNAL(upPressed(PageComponent*)),     receiver, memberUp);
    connect(this, SIGNAL(downPressed(PageComponent*)),   receiver, memberDown);
    connect(this, SIGNAL(copyPressed(PageComponent*)),   receiver, memberCopy);
    connect(this, SIGNAL(deletePressed(PageComponent*)), receiver, memberDelete);
}
