#include "wordxmltable.h"

WordXMLTable::WordXMLTable(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTable;

    reset();
}

WordXMLTable::~WordXMLTable()
{
}

void WordXMLTable::writeToStream(QTextStream &aStream)
{
}

void WordXMLTable::reset()
{
    WordXMLBase::reset();
}

bool WordXMLTable::isModified()
{
    return true;
}

WordXMLTable& WordXMLTable::operator=(const WordXMLTable &another)
{
    return *this;
}
