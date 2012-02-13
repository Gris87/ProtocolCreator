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
            QString aName=propList.at(i)->name;
            QString resName="";

            for (int j=0; j<aName.length(); j++)
            {
                if (
                    (aName.at(j)>='A' && aName.at(j)<='Z')
                    ||
                    (aName.at(j)>='a' && aName.at(j)<='z')
                    ||
                    (aName.at(j)>='À' && aName.at(j)<='ß')
                    ||
                    (aName.at(j)>='à' && aName.at(j)<='ÿ')
                    ||
                    (aName.at(j)>='0' && aName.at(j)<='9')
                   )
                {
                    resName.append(aName.at(j));
                }
                else
                {
                    QString aCode=QString::number(aName.at(j).unicode(), 16).toUpper();

                    while (aCode.length()<4)
                    {
                        aCode.insert(0, "0");
                    }

                    resName.append("_x");
                    resName.append(aCode);
                    resName.append("_");
                }
            }

            aStream<<"   <o:"+resName+" dt:dt=\"";

            switch (propList.at(i)->type)
            {
                case cptText:
                {
                    aStream<<"string\">"+propList.at(i)->textValue;
                }
                break;
                case cptDate:
                {
                    aStream<<"dateTime.tz\">"+propList.at(i)->dateValue.toString("yyyy-MM-ddThh:mm:ss'Z'");
                }
                break;
                case cptNumber:
                {
                    aStream<<"float\">"+QString::number(propList.at(i)->floatValue);
                }
                break;
                case cptYesNo:
                {
                    aStream<<"boolean\">";

                    if (propList.at(i)->booleanValue)
                    {
                        aStream<<"1";
                    }
                    else
                    {
                        aStream<<"0";
                    }
                }
                break;
            }

            aStream<<"</o:"+resName+">\r\n";
        }

        aStream<<"  </o:CustomDocumentProperties>\r\n";
    }
}

void WordXMLCustomProperties::reset()
{
    clear();
}

QVariant WordXMLCustomProperties::getValue(SCustomProperty* aProperty)
{
    switch (aProperty->type)
    {
        case cptText   : return aProperty->textValue;
        case cptDate   : return aProperty->dateValue;
        case cptNumber : return aProperty->floatValue;
        case cptYesNo  : return aProperty->booleanValue;
    }

    return 0;
}

void WordXMLCustomProperties::setValue(SCustomProperty* aProperty, QVariant aValue)
{
    switch (aValue.type())
    {
        case QVariant::String:
        {
            aProperty->type=cptText;
            aProperty->textValue=aValue.toString();
        }
        break;
        case QVariant::DateTime:
        {
            aProperty->type=cptDate;
            aProperty->dateValue=aValue.toDateTime();
        }
        break;
        case QVariant::Date:
        {
            aProperty->type=cptDate;
            aProperty->dateValue.setDate(aValue.toDate());
        }
        break;

        case QVariant::Time:
        {
            aProperty->type=cptDate;
            aProperty->dateValue.setTime(aValue.toTime());
        }
        break;
        case QVariant::Double:
        {
            aProperty->type=cptNumber;
            aProperty->floatValue=aValue.toDouble();
        }
        break;
        case QVariant::Bool:
        {
            aProperty->type=cptYesNo;
            aProperty->booleanValue=aValue.toBool();
        }
        break;
        default:
        {
            throw "Wrong property value";
        }
        break;
    }
}

SCustomProperty* WordXMLCustomProperties::add(QString aName, QVariant aValue)
{
    SCustomProperty* aNewProperty=new SCustomProperty;

    aNewProperty->name=aName;
    setValue(aNewProperty, aValue);

    propList.append(aNewProperty);

    return aNewProperty;
}

SCustomProperty* WordXMLCustomProperties::copy(QString aName, QString aSecondName)
{
    return copy(get(aName), aSecondName);
}

SCustomProperty* WordXMLCustomProperties::copy(int index, QString aSecondName)
{
    return copy(propList.at(index), aSecondName);
}

SCustomProperty* WordXMLCustomProperties::copy(SCustomProperty* aProperty, QString aSecondName)
{
    SCustomProperty* aNewProperty=new SCustomProperty;

    aNewProperty->name=aSecondName;
    aNewProperty->type=aProperty->type;
    aNewProperty->textValue=aProperty->textValue;
    aNewProperty->dateValue=aProperty->dateValue;
    aNewProperty->floatValue=aProperty->floatValue;
    aNewProperty->booleanValue=aProperty->booleanValue;

    propList.append(aNewProperty);

    return aNewProperty;
}

SCustomProperty* WordXMLCustomProperties::get(QString aName)
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

SCustomProperty* WordXMLCustomProperties::get(int index)
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

void WordXMLCustomProperties::remove(SCustomProperty* aProperty)
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
