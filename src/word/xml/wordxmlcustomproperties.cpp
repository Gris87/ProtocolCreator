#include "wordxmlcustomproperties.h"

WordXMLCustomProperties::WordXMLCustomProperties()
{
    reset();
}

WordXMLCustomProperties::~WordXMLCustomProperties()
{
    clear();
}

void WordXMLCustomProperties::writeToStream(QTextStream &aStream)
{
    if (propList.length()>0)
    {
        aStream<<"  <o:CustomDocumentProperties>\r\n";

        for (int i=0; i<propList.length(); i++)
        {
            propList.at(i)->writeToStream(aStream);
        }

        aStream<<"  </o:CustomDocumentProperties>\r\n";
    }
}

void WordXMLCustomProperties::reset()
{
    clear();
}

WordXMLCustomProperty* WordXMLCustomProperties::add(QString aName, QVariant aValue)
{
    WordXMLCustomProperty* aNewProperty=new WordXMLCustomProperty();

    aNewProperty->name=aName;
    aNewProperty->setValue(aValue);

    propList.append(aNewProperty);

    return aNewProperty;
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(QString aName, QString aSecondName)
{
    return copy(get(aName), aSecondName);
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(int index, QString aSecondName)
{
    return copy(propList.at(index), aSecondName);
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(WordXMLCustomProperty* aProperty, QString aSecondName)
{
    WordXMLCustomProperty* aNewProperty=new WordXMLCustomProperty();

    *aNewProperty=*aProperty;
    aNewProperty->name=aSecondName;

    propList.append(aNewProperty);

    return aNewProperty;
}

WordXMLCustomProperty* WordXMLCustomProperties::get(QString aName)
{
    for (int i=0; i<propList.length(); i++)
    {
        if (propList.at(i)->name==aName)
        {
            return propList.at(i);
        }
    }

    return 0;
}

WordXMLCustomProperty* WordXMLCustomProperties::get(int index)
{
    return propList.at(index);
}

void WordXMLCustomProperties::remove(QString aName)
{
    propList.removeOne(get(aName));
}

void WordXMLCustomProperties::remove(int index)
{
    propList.removeAt(index);
}

void WordXMLCustomProperties::remove(WordXMLCustomProperty* aProperty)
{
    propList.removeOne(aProperty);
}

int WordXMLCustomProperties::count()
{
    return propList.length();
}

void WordXMLCustomProperties::clear()
{
    for (int i=0; i<propList.length(); i++)
    {
        delete propList[i];
    }

    propList.clear();
}
