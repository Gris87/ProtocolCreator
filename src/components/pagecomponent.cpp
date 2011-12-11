#include "src/other/global.h"

PageComponent::PageComponent(QWidget *parent) :
    QWidget(parent)
{
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
