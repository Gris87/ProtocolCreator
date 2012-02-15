#include "wordxmlcustomproperty.h"

WordXMLCustomProperty::WordXMLCustomProperty() : WordXMLBase()
{
    reset();
}

WordXMLCustomProperty::~WordXMLCustomProperty()
{
}

void WordXMLCustomProperty::writeToStream(QTextStream &aStream)
{
    QString resName="";

    for (int j=0; j<name.length(); j++)
    {
        if (
            (name.at(j)>='A' && name.at(j)<='Z')
            ||
            (name.at(j)>='a' && name.at(j)<='z')
            ||
            (name.at(j)>='À' && name.at(j)<='ß')
            ||
            (name.at(j)>='à' && name.at(j)<='ÿ')
            ||
            (name.at(j)>='0' && name.at(j)<='9')
           )
        {
            resName.append(name.at(j));
        }
        else
        {
            QString aCode=QString::number(name.at(j).unicode(), 16).toUpper();

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

    switch (type)
    {
        case cptText:
        {
            aStream<<"string\">"+textValue;
        }
        break;
        case cptDate:
        {
            aStream<<"dateTime.tz\">"+dateValue.toString("yyyy-MM-ddThh:mm:ss'Z'");
        }
        break;
        case cptNumber:
        {
            aStream<<"float\">"+QString::number(floatValue);
        }
        break;
        case cptYesNo:
        {
            aStream<<"boolean\">";

            if (booleanValue)
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

void WordXMLCustomProperty::reset()
{
    name="";
    type=cptText;
    textValue="";
    dateValue=QDateTime(QDate(1900, 1, 1));
    floatValue=0;
    booleanValue=false;
}

QVariant WordXMLCustomProperty::getValue()
{
    switch (type)
    {
        case cptText   : return textValue;
        case cptDate   : return dateValue;
        case cptNumber : return floatValue;
        case cptYesNo  : return booleanValue;
    }

    return 0;
}

void WordXMLCustomProperty::setValue(QVariant aValue)
{
    switch (aValue.type())
    {
        case QVariant::String:
        {
            type=cptText;
            textValue=aValue.toString();
        }
        break;
        case QVariant::DateTime:
        {
            type=cptDate;
            dateValue=aValue.toDateTime();
        }
        break;
        case QVariant::Date:
        {
            type=cptDate;
            dateValue.setDate(aValue.toDate());
        }
        break;

        case QVariant::Time:
        {
            type=cptDate;
            dateValue.setTime(aValue.toTime());
        }
        break;
        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        {
            type=cptNumber;
            floatValue=aValue.toDouble();
        }
        break;
        case QVariant::Bool:
        {
            type=cptYesNo;
            booleanValue=aValue.toBool();
        }
        break;
        default:
        {
            throw "Wrong property value";
        }
        break;
    }
}
