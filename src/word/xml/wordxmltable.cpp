#include "wordxmltable.h"

WordXMLTable::WordXMLTable(WordXMLBase* aParent) : WordXMLBase(aParent)
{
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
