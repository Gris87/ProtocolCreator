#include "wordxmlstyles.h"

WordXMLStyles::WordXMLStyles() : QList<WordXMLStyle>()
{
    reset();
}

WordXMLStyles::~WordXMLStyles()
{
    clear();
}

void WordXMLStyles::writeToStream(QTextStream &aStream)
{
    aStream<<" <w:styles>\r\n";
    aStream<<"  <w:versionOfBuiltInStylenames w:val=\"4\"/>\r\n";
    aStream<<"  <w:latentStyles w:defLockedState=\"off\" w:latentStyleCount=\"156\"/>\r\n";

    for (int i=0; i<stylesList.length(); i++)
    {
        aStream<<"\r\n";
        stylesList.at(i)->writeToStream(aStream);
    }

    aStream<<" </w:styles>\r\n";
    aStream<<"\r\n";
}

void WordXMLStyles::reset()
{
    clear();
}

WordXMLStyle* WordXMLStyles::add(QString aStyleID, EStyleType aType, bool isDefault)
{
    WordXMLStyle* aNewStyle=new WordXMLStyle();

    aNewStyle->styleID=aStyleID;
    aNewStyle->type=aType;
    aNewStyle->isDefault=isDefault;

    stylesList.append(aNewStyle);

    return aNewStyle;
}

WordXMLStyle* WordXMLStyles::copy(QString aStyleID, QString aSecondStyleID)
{
    return copy(get(aStyleID), aSecondStyleID);
}

WordXMLStyle* WordXMLStyles::copy(int index, QString aSecondStyleID)
{
    return copy(stylesList.at(index), aSecondStyleID);
}

WordXMLStyle* WordXMLStyles::copy(WordXMLStyle* aStyle, QString aSecondStyleID)
{
    WordXMLStyle* aNewStyle=new WordXMLStyle();

    *aNewStyle=*aStyle;
    aNewStyle->styleID=aSecondStyleID;
    aNewStyle->isDefault=false;

    stylesList.append(aNewStyle);

    return aNewStyle;
}

WordXMLStyle* WordXMLStyles::get(QString aStyleID)
{
    for (int i=0; i<stylesList.length(); i++)
    {
        if (stylesList.at(i)->styleID==aStyleID)
        {
            return stylesList.at(i);
        }
    }

    return 0;
}

WordXMLStyle* WordXMLStyles::get(int index)
{
    return stylesList.at(index);
}

void WordXMLStyles::remove(QString aStyleID)
{
    stylesList.removeOne(get(aStyleID));
}

void WordXMLStyles::remove(int index)
{
    stylesList.removeAt(index);
}

void WordXMLStyles::remove(WordXMLStyle* aStyle)
{
    stylesList.removeOne(aStyle);
}

int WordXMLStyles::count()
{
    return stylesList.length();
}

void WordXMLStyles::clear()
{
    for (int i=0; i<stylesList.length(); i++)
    {
        delete stylesList[i];
    }

    stylesList.clear();
}
