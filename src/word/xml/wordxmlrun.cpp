#include "wordxmlrun.h"

WordXMLRun::WordXMLRun() : WordXMLBase()
{
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
    properties.reset();
}

bool WordXMLRun::isModified()
{
    return properties.isModified();
}
