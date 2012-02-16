#include "wordxmlrun.h"

WordXMLRun::WordXMLRun(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    properties.parent=this;

    reset();
}

WordXMLRun::~WordXMLRun()
{
}

void WordXMLRun::writeToStream(QTextStream &aStream)
{
    properties.writeToStream(aStream);
}

void WordXMLRun::reset()
{
    WordXMLBase::reset();

    properties.reset();
}

bool WordXMLRun::isModified()
{
    return properties.isModified();
}
