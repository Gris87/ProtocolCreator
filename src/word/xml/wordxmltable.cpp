#include "wordxmltable.h"

WordXMLTable::WordXMLTable(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTable;

    properties.parent=this;

    reset();
}

WordXMLTable::~WordXMLTable()
{
    clearRows();
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

    clearRows();
}

bool WordXMLTable::isModified()
{
    return properties.isModified()
           ||
           mList.length()>0;
}

WordXMLTableRow* WordXMLTable::addRow()
{
    WordXMLTableRow* aNewRow=new WordXMLTableRow(this);

    mList.append(aNewRow);

    return aNewRow;
}

WordXMLTableRow* WordXMLTable::copyRow(int index)
{
    return copyRow(mList.at(index));
}

WordXMLTableRow* WordXMLTable::copyRow(WordXMLTableRow* aRow)
{
    WordXMLTableRow* aNewRow=new WordXMLTableRow(this);

    *aNewRow=*aRow;

    mList.append(aNewRow);

    return aNewRow;
}

WordXMLTableRow* WordXMLTable::getRow(int index)
{
    return mList.at(index);
}

void WordXMLTable::removeRow(int index)
{
    mList.removeAt(index);
}

void WordXMLTable::removeRow(WordXMLTableRow* aRow)
{
    mList.removeOne(aRow);
}

int WordXMLTable::rowCount()
{
    return mList.length();
}

void WordXMLTable::clearRows()
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

    clearRows();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLTableRow* aNewObject=new WordXMLTableRow(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
