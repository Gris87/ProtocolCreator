#include "wordxmltablecell.h"

WordXMLTableCell::WordXMLTableCell(WordXMLBase* aParent) : WordXMLMultiPart(aParent)
{
    componentType=wxtTableCell;

    properties.parent=this;

    reset();
}

WordXMLTableCell::~WordXMLTableCell()
{
}

void WordXMLTableCell::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tc>\r\n";

    properties.writeToStream(aStream);
    WordXMLMultiPart::writeToStream(aStream);

    if (mList.length()==0 || mList.at(mList.length()-1)->getComponentType()!=wxtParagraph)
    {
        WordXMLParagraph *aParagraph=new WordXMLParagraph(this);
        aParagraph->writeToStream(aStream);
        delete aParagraph;
    }

    aStream<<space<<"</w:tc>\r\n";
}

void WordXMLTableCell::reset()
{
    WordXMLMultiPart::reset();

    properties.reset();
}

bool WordXMLTableCell::isModified()
{
    return properties.isModified()
           ||
           WordXMLMultiPart::isModified();
}

WordXMLTableCell& WordXMLTableCell::operator=(const WordXMLTableCell &another)
{
    properties=another.properties;

    WordXMLMultiPart *aThis=(WordXMLMultiPart*)this;
    *aThis=another;

    return *this;
}
