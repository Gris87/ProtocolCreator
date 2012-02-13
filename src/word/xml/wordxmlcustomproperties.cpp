#include "wordxmlcustomproperties.h"

WordXMLCustomProperties::WordXMLCustomProperties() : QList<SCustomProperty>()
{
    reset();
}

WordXMLCustomProperties::~WordXMLCustomProperties()
{
}

void WordXMLCustomProperties::writeToStream(QTextStream &aStream)
{
    if (length()>0)
    {
        aStream<<"  <o:CustomDocumentProperties>\r\n";

        for (int i=0; i<length(); i++)
        {
            QString aName=at(i).name;
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

            switch (at(i).type)
            {
                case cptText:
                {
                    aStream<<"string\">"+at(i).textValue;
                }
                break;
                case cptDate:
                {
                    aStream<<"dateTime.tz\">"+at(i).dateValue.toString("yyyy-MM-ddThh:mm:ss'Z'");
                }
                break;
                case cptNumber:
                {
                    aStream<<"float\">"+QString::number(at(i).floatValue);
                }
                break;
                case cptYesNo:
                {
                    aStream<<"boolean\">";

                    if (at(i).booleanValue)
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
