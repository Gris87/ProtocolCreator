#include "wordxmlfootnoteproperties.h"

WordXMLFootnoteProperties::WordXMLFootnoteProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLFootnoteProperties::~WordXMLFootnoteProperties()
{
    clear();
}

void WordXMLFootnoteProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:footnotePr>\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            if (i>0)
            {
                aStream<<"\r\n";
            }

            mList.at(i)->writeToStream(aStream);
        }

        aStream<<space<<"</w:footnotePr>\r\n";
    }
}

void WordXMLFootnoteProperties::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLFootnoteProperties::isModified()
{
    return mList.length()>0;
}

WordXMLFootnote* WordXMLFootnoteProperties::add(QString aType)
{
    WordXMLFootnote* aNewFootnote=new WordXMLFootnote(this);

    aNewFootnote->type=aType;

    mList.append(aNewFootnote);

    return aNewFootnote;
}

WordXMLFootnote* WordXMLFootnoteProperties::copy(QString aType, QString aSecondType)
{
    return copy(get(aType), aSecondType);
}

WordXMLFootnote* WordXMLFootnoteProperties::copy(int index, QString aSecondType)
{
    return copy(mList.at(index), aSecondType);
}

WordXMLFootnote* WordXMLFootnoteProperties::copy(WordXMLFootnote* aFootnote, QString aSecondType)
{
    WordXMLFootnote* aNewFootnote=new WordXMLFootnote(this);

    *aNewFootnote=*aFootnote;
    aNewFootnote->type=aSecondType;

    mList.append(aNewFootnote);

    return aNewFootnote;
}

WordXMLFootnote* WordXMLFootnoteProperties::get(QString aType)
{
    for (int i=0; i<mList.length(); i++)
    {
        if (mList.at(i)->type==aType)
        {
            return mList.at(i);
        }
    }

    return 0;
}

WordXMLFootnote* WordXMLFootnoteProperties::get(int index)
{
    return mList.at(index);
}

void WordXMLFootnoteProperties::remove(QString aType)
{
    mList.removeOne(get(aType));
}

void WordXMLFootnoteProperties::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLFootnoteProperties::remove(WordXMLFootnote* aFootnote)
{
    mList.removeOne(aFootnote);
}

int WordXMLFootnoteProperties::count()
{
    return mList.length();
}

void WordXMLFootnoteProperties::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}
