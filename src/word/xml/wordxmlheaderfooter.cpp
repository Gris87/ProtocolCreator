#include "wordxmlheaderfooter.h"

#include "wordxmlparagraph.h"
#include "wordxmltable.h"

WordXMLHeaderFooter::WordXMLHeaderFooter(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    isHeader=true;

    reset();
}

WordXMLHeaderFooter::~WordXMLHeaderFooter()
{
    clear();
}

void WordXMLHeaderFooter::writeToStream(QTextStream &aStream)
{
    if (isHeader)
    {
        aStream<<space<<"<w:hdr";
    }
    else
    {
        aStream<<space<<"<w:ftr";
    }

    switch (type)
    {
        case htOdd:
        {
            aStream<<" w:type=\"odd\"";
        }
        break;
        case htEven:
        {
            aStream<<" w:type=\"even\"";
        }
        break;
        case htNone:
        {
            //Nothing
        }
        break;
    }

    aStream<<">\r\n";

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    if (isHeader)
    {
        aStream<<space<<"</w:hdr>";
    }
    else
    {
        aStream<<space<<"</w:ftr>";
    }
}

void WordXMLHeaderFooter::reset()
{
    WordXMLBase::reset();

    type=htNone;

    clear();
}

bool WordXMLHeaderFooter::isModified()
{
    return mList.length()>0;
}

WordXMLParagraph* WordXMLHeaderFooter::addParagraph()
{
    WordXMLParagraph *aNewComponent=new WordXMLParagraph(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLTable* WordXMLHeaderFooter::addTable()
{
    WordXMLTable *aNewComponent=new WordXMLTable(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLBase* WordXMLHeaderFooter::get(int index)
{
    return mList.at(index);
}

void WordXMLHeaderFooter::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLHeaderFooter::remove(WordXMLBase* aXMLComponent)
{
    mList.removeOne(aXMLComponent);
}

int WordXMLHeaderFooter::count()
{
    return mList.length();
}

void WordXMLHeaderFooter::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

