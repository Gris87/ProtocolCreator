#include "wordxmlstyles.h"

WordXMLStyles::WordXMLStyles(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLStyles::~WordXMLStyles()
{
    clear();
}

void WordXMLStyles::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:styles>\r\n";
    aStream<<space<<" <w:versionOfBuiltInStylenames w:val=\"4\"/>\r\n";
    aStream<<space<<" <w:latentStyles w:defLockedState=\"off\" w:latentStyleCount=\"156\"/>\r\n";

    for (int i=0; i<mList.length(); i++)
    {
        aStream<<"\r\n";
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<space<<"</w:styles>\r\n";
    aStream<<"\r\n";
}

void WordXMLStyles::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLStyles::isModified()
{
    return mList.length()>0;
}

void WordXMLStyles::setDefaultStyles2003()
{
    clear();

    WordXMLStyle *aStyle;

    aStyle=add("Normal", stParagraph, true);
    aStyle->name="Normal";
    //aStyle->rsid="008F1615";

    aStyle->runProperties.font="Times New Roman";
    aStyle->runProperties.fontSize=12;
    aStyle->runProperties.fontSizeCS=12;
    aStyle->runProperties.language="RU";
    aStyle->runProperties.languageFarEast="KO";
    aStyle->runProperties.languageBIDI="AR-SA";

    //---------------------------------------

    aStyle=add("DefaultParagraphFont", stCharacter, true);
    aStyle->name="Default Paragraph Font";
    aStyle->isSemiHidden=true;

    //---------------------------------------

    aStyle=add("TableNormal", stTable, true);
    aStyle->name="Normal Table";
    aStyle->uiName="Table Normal";
    aStyle->isSemiHidden=true;
    aStyle->runProperties.font="Times New Roman";
    aStyle->tableProperties.indentation=0;
    aStyle->tableProperties.cellMargin.setMargin(0, 108, 0, 108);

    //---------------------------------------

    aStyle=add("NoList", stList, true);
    aStyle->name="No List";
    aStyle->isSemiHidden=true;

    //---------------------------------------

    aStyle=add("Footer", stParagraph, false);
    aStyle->name="footer";
    aStyle->uiName="Footer";
    aStyle->basedOn="Normal";
    //aStyle->rsid="00857F11";
    aStyle->paragraphProperties.paragraphStyle="Footer";
    aStyle->paragraphProperties.tabs.add(tlCenter, 4677);
    aStyle->paragraphProperties.tabs.add(tlRight, 9355);
    aStyle->runProperties.font="Times New Roman";

    //---------------------------------------

    aStyle=add("PageNumber", stCharacter, false);
    aStyle->name="page number";
    aStyle->uiName="Page Number";
    aStyle->basedOn="DefaultParagraphFont";
    //aStyle->rsid="00857F11";
}

WordXMLStyle* WordXMLStyles::add(QString aStyleID, EStyleType aType, bool isDefault)
{
    WordXMLStyle* aNewStyle=new WordXMLStyle(this);

    aNewStyle->styleID=aStyleID;
    aNewStyle->type=aType;
    aNewStyle->isDefault=isDefault;

    mList.append(aNewStyle);

    return aNewStyle;
}

WordXMLStyle* WordXMLStyles::copy(QString aStyleID, QString aSecondStyleID)
{
    return copy(get(aStyleID), aSecondStyleID);
}

WordXMLStyle* WordXMLStyles::copy(int index, QString aSecondStyleID)
{
    return copy(mList.at(index), aSecondStyleID);
}

WordXMLStyle* WordXMLStyles::copy(WordXMLStyle* aStyle, QString aSecondStyleID)
{
    WordXMLStyle* aNewStyle=new WordXMLStyle(this);

    *aNewStyle=*aStyle;
    aNewStyle->styleID=aSecondStyleID;
    aNewStyle->isDefault=false;

    mList.append(aNewStyle);

    return aNewStyle;
}

WordXMLStyle* WordXMLStyles::get(QString aStyleID)
{
    for (int i=0; i<mList.length(); i++)
    {
        if (mList.at(i)->styleID==aStyleID)
        {
            return mList.at(i);
        }
    }

    return 0;
}

WordXMLStyle* WordXMLStyles::get(int index)
{
    return mList.at(index);
}

void WordXMLStyles::remove(QString aStyleID)
{
    mList.removeOne(get(aStyleID));
}

void WordXMLStyles::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLStyles::remove(WordXMLStyle* aStyle)
{
    mList.removeOne(aStyle);
}

int WordXMLStyles::count()
{
    return mList.length();
}

void WordXMLStyles::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}
