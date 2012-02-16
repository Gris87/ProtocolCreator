#include "wordxmlbase.h"

WordXMLBase::WordXMLBase(WordXMLBase* aParent)
{
    parent=aParent;

    space="";
}

WordXMLBase::~WordXMLBase()
{
    // Nothing
}

void WordXMLBase::writeToStream(QTextStream &aStream)
{
    // Nothing
}

void WordXMLBase::reset()
{
    if (parent)
    {
        space=parent->space+"  ";
    }
    else
    {
        space="";
    }
}

bool WordXMLBase::isModified()
{
    return true;
}
