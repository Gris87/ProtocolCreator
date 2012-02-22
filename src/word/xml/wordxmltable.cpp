#include "wordxmltable.h"

WordXMLTable::WordXMLTable(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTable;

    properties.parent=this;

    reset();
}

WordXMLTable::~WordXMLTable()
{
    clear();
}

void WordXMLTable::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tbl>\r\n";

    properties.writeToStream(aStream);

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<space<<"</w:tbl>\r\n";
}

void WordXMLTable::reset()
{
    WordXMLBase::reset();

    properties.reset();

    clear();
}

bool WordXMLTable::isModified()
{
    return properties.isModified()
           ||
           mList.length()>0;
}

WordXMLTableRow* WordXMLTable::add()
{
    WordXMLTableRow* aNewRow=new WordXMLTableRow(this);

    mList.append(aNewRow);

    return aNewRow;
}

WordXMLTableRow* WordXMLTable::copy(int index)
{
    return copy(mList.at(index));
}

WordXMLTableRow* WordXMLTable::copy(WordXMLTableRow* aRow)
{
    WordXMLTableRow* aNewRow=new WordXMLTableRow(this);

    *aNewRow=*aRow;

    mList.append(aNewRow);

    return aNewRow;
}

WordXMLTableRow* WordXMLTable::get(int index)
{
    return mList.at(index);
}

void WordXMLTable::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLTable::remove(WordXMLTableRow* aRow)
{
    mList.removeOne(aRow);
}

int WordXMLTable::count()
{
    return mList.length();
}

void WordXMLTable::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLTable& WordXMLTable::operator=(const WordXMLTable &another)
{
    properties=another.properties;

    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLTableRow* aNewObject=new WordXMLTableRow(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
