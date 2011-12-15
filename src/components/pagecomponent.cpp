#include "src/other/global.h"

PageComponent::PageComponent(QWidget *parent) :
    QWidget(parent)
{
    resetCalculation();
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

bool PageComponent::isEditable()
{
    return true;
}

void PageComponent::checkForErrors(QStringList &aErrorList)
{
    QString aName=variableName();

    for (int i=0; i<aName.length(); i++)
    {
        if (
            !(aName.at(i)>='a' && aName.at(i)<='z')
            &&
            !(aName.at(i)>='A' && aName.at(i)<='Z')
            &&
            !(aName.at(i)>='0' && aName.at(i)<='9')
            &&
            aName.at(i)!='_'
           )
        {
            aErrorList.append("Error: Недопустимый символ \""+aName.at(i)+"\" в имени");
        }
    }

    if (aName.length()>0 && aName.at(0)>='0' && aName.at(0)<='9')
    {
        aErrorList.append("Error: Имя не может начинаться с цифры");
    }
}

void PageComponent::resetCalculation()
{
    isWasCalculated=false;
    isInCalculation=false;
    calculationResult=0;
}

QVariant PageComponent::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    if (isInCalculation)
    {
        throw "This component already in calculation process";
    }

    isInCalculation=true;
}
