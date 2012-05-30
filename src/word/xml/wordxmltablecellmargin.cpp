#include "wordxmltablecellmargin.h"

WordXMLTableCellMargin::WordXMLTableCellMargin(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableCellMargin;

    tagName="tblCellMar";

    reset();
}

WordXMLTableCellMargin::~WordXMLTableCellMargin()
{
}

void WordXMLTableCellMargin::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:"+tagName+">\r\n";

        if (top>=0)
        {
            aStream<<space<<" <w:top w:w=\""+QString::number(top)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (left>=0)
        {
            aStream<<space<<" <w:left w:w=\""+QString::number(left)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (bottom>=0)
        {
            aStream<<space<<" <w:bottom w:w=\""+QString::number(bottom)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (right>=0)
        {
            aStream<<space<<" <w:right w:w=\""+QString::number(right)+"\" w:type=\"dxa\"/>\r\n";
        }

        aStream<<space<<"</w:"+tagName+">\r\n";
    }
}

void WordXMLTableCellMargin::reset()
{
    WordXMLBase::reset();

    top=-1;
    left=-1;
    bottom=-1;
    right=-1;
}

bool WordXMLTableCellMargin::isModified()
{
    return (
            (top>=0)
            ||
            (left>=0)
            ||
            (bottom>=0)
            ||
            (right>=0)
           );
}

WordXMLTableCellMargin& WordXMLTableCellMargin::operator=(const WordXMLTableCellMargin &another)
{
    top=another.top;
    left=another.left;
    bottom=another.bottom;
    right=another.right;

    return *this;
}

void WordXMLTableCellMargin::setMargin(double aTop, double aLeft, double aBottom, double aRight)
{
    top=aTop;
    left=aLeft;
    bottom=aBottom;
    right=aRight;
}
