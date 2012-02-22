#include "wordxmltablerow.h"

WordXMLTableRow::WordXMLTableRow(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableRow;

    properties.parent=this;

    reset();
}

WordXMLTableRow::~WordXMLTableRow()
{
    clearCells();
}

void WordXMLTableRow::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tr";

    if (rsidR!="")
    {
        aStream<<" wsp:rsidR=\""+rsidR+"\"";
    }

    if (rsidTr!="")
    {
        aStream<<" wsp:rsidTr=\""+rsidTr+"\"";
    }

    aStream<<">\r\n";

    properties.writeToStream(aStream);

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<space<<"</w:tr>\r\n";
}

void WordXMLTableRow::reset()
{
    WordXMLBase::reset();

    rsidR="";
    rsidTr="";

    properties.reset();

    clearCells();
}

bool WordXMLTableRow::isModified()
{
    return rsidR!=""
           ||
           rsidTr!=""
           ||
           properties.isModified()
           ||
           mList.length()>0;
}

WordXMLTableCell* WordXMLTableRow::addCell()
{
    WordXMLTableCell* aNewCell=new WordXMLTableCell(this);

    mList.append(aNewCell);

    return aNewCell;
}

WordXMLTableCell* WordXMLTableRow::copyCell(int index)
{
    return copyCell(mList.at(index));
}

WordXMLTableCell* WordXMLTableRow::copyCell(WordXMLTableCell* aCell)
{
    WordXMLTableCell* aNewCell=new WordXMLTableCell(this);

    *aNewCell=*aCell;

    mList.append(aNewCell);

    return aNewCell;
}

WordXMLTableCell* WordXMLTableRow::getCell(int index)
{
    return mList.at(index);
}

void WordXMLTableRow::removeCell(int index)
{
    mList.removeAt(index);
}

void WordXMLTableRow::removeCell(WordXMLTableCell* aCell)
{
    mList.removeOne(aCell);
}

int WordXMLTableRow::cellCount()
{
    return mList.length();
}

void WordXMLTableRow::clearCells()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLTableRow& WordXMLTableRow::operator=(const WordXMLTableRow &another)
{
    rsidR=another.rsidR;
    rsidTr=another.rsidTr;

    properties=another.properties;

    clearCells();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLTableCell* aNewObject=new WordXMLTableCell(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
