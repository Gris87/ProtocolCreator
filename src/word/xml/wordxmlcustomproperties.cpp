#include "wordxmlcustomproperties.h"

WordXMLCustomProperties::WordXMLCustomProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLCustomProperties::~WordXMLCustomProperties()
{
    clear();
}

void WordXMLCustomProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<o:CustomDocumentProperties>\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            mList.at(i)->writeToStream(aStream);
        }

        aStream<<space<<"</o:CustomDocumentProperties>\r\n";
    }
}

void WordXMLCustomProperties::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLCustomProperties::isModified()
{
    return mList.length()>0;
}

WordXMLCustomProperty* WordXMLCustomProperties::add(QString aName, QVariant aValue)
{
    WordXMLCustomProperty* aNewProperty=new WordXMLCustomProperty(this);

    aNewProperty->name=aName;
    aNewProperty->setValue(aValue);

    mList.append(aNewProperty);

    return aNewProperty;
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(QString aName, QString aSecondName)
{
    return copy(get(aName), aSecondName);
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(int index, QString aSecondName)
{
    return copy(mList.at(index), aSecondName);
}

WordXMLCustomProperty* WordXMLCustomProperties::copy(WordXMLCustomProperty* aProperty, QString aSecondName)
{
    WordXMLCustomProperty* aNewProperty=new WordXMLCustomProperty(this);

    *aNewProperty=*aProperty;
    aNewProperty->name=aSecondName;

    mList.append(aNewProperty);

    return aNewProperty;
}

WordXMLCustomProperty* WordXMLCustomProperties::get(QString aName)
{
    for (int i=0; i<mList.length(); i++)
    {
        if (mList.at(i)->name==aName)
        {
            return mList.at(i);
        }
    }

    return 0;
}

WordXMLCustomProperty* WordXMLCustomProperties::get(int index)
{
    return mList.at(index);
}

void WordXMLCustomProperties::remove(QString aName)
{
    mList.removeOne(get(aName));
}

void WordXMLCustomProperties::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLCustomProperties::remove(WordXMLCustomProperty* aProperty)
{
    mList.removeOne(aProperty);
}

int WordXMLCustomProperties::count()
{
    return mList.length();
}

void WordXMLCustomProperties::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLCustomProperties& WordXMLCustomProperties::operator=(const WordXMLCustomProperties &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLCustomProperty* aNewObject=new WordXMLCustomProperty(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
