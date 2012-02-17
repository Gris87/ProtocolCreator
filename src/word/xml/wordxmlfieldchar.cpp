#include "wordxmlfieldchar.h"

WordXMLFieldChar::WordXMLFieldChar(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLFieldChar::~WordXMLFieldChar()
{
}

void WordXMLFieldChar::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:fldChar w:fldCharType=\"";

    switch (type)
    {
        case fctBegin:
        {
            aStream<<"begin";
        }
        break;
        case fctSeparate:
        {
            aStream<<"separate";
        }
        break;
        case fctEnd:
        {
            aStream<<"end";
        }
        break;
    }

    aStream<<"\"/>\r\n";
}

void WordXMLFieldChar::reset()
{
    WordXMLBase::reset();

    type=fctBegin;
}

bool WordXMLFieldChar::isModified()
{
    return true;
}
