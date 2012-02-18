#include "wordxmlendnoteproperties.h"

WordXMLEndnoteProperties::WordXMLEndnoteProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtEndnoteProperties;

    reset();
}

WordXMLEndnoteProperties::~WordXMLEndnoteProperties()
{
    clear();
}

void WordXMLEndnoteProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:endnotePr>\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            if (i>0)
            {
                aStream<<"\r\n";
            }

            mList.at(i)->writeToStream(aStream);
        }

        aStream<<space<<"</w:endnotePr>\r\n";
    }
}

void WordXMLEndnoteProperties::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLEndnoteProperties::isModified()
{
    return mList.length()>0;
}

WordXMLEndnote* WordXMLEndnoteProperties::add(QString aType)
{
    WordXMLEndnote* aNewEndnote=new WordXMLEndnote(this);

    aNewEndnote->type=aType;

    mList.append(aNewEndnote);

    return aNewEndnote;
}

WordXMLEndnote* WordXMLEndnoteProperties::copy(QString aType, QString aSecondType)
{
    return copy(get(aType), aSecondType);
}

WordXMLEndnote* WordXMLEndnoteProperties::copy(int index, QString aSecondType)
{
    return copy(mList.at(index), aSecondType);
}

WordXMLEndnote* WordXMLEndnoteProperties::copy(WordXMLEndnote* aEndnote, QString aSecondType)
{
    WordXMLEndnote* aNewEndnote=new WordXMLEndnote(this);

    *aNewEndnote=*aEndnote;
    aNewEndnote->type=aSecondType;

    mList.append(aNewEndnote);

    return aNewEndnote;
}

WordXMLEndnote* WordXMLEndnoteProperties::get(QString aType)
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

WordXMLEndnote* WordXMLEndnoteProperties::get(int index)
{
    return mList.at(index);
}

void WordXMLEndnoteProperties::remove(QString aType)
{
    mList.removeOne(get(aType));
}

void WordXMLEndnoteProperties::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLEndnoteProperties::remove(WordXMLEndnote* aEndnote)
{
    mList.removeOne(aEndnote);
}

int WordXMLEndnoteProperties::count()
{
    return mList.length();
}

void WordXMLEndnoteProperties::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLEndnoteProperties& WordXMLEndnoteProperties::operator=(const WordXMLEndnoteProperties &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLEndnote* aNewObject=new WordXMLEndnote(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
