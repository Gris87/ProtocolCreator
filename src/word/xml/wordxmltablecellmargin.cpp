#include "wordxmltablecellmargin.h"

WordXMLTableCellMargin::WordXMLTableCellMargin(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLTableCellMargin::~WordXMLTableCellMargin()
{
}

void WordXMLTableCellMargin::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<"    <w:tblCellMar>\r\n";

        if (top>=0)
        {
            aStream<<"     <w:top w:w=\""+QString::number(top)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (left>=0)
        {
            aStream<<"     <w:left w:w=\""+QString::number(left)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (bottom>=0)
        {
            aStream<<"     <w:bottom w:w=\""+QString::number(bottom)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (right>=0)
        {
            aStream<<"     <w:right w:w=\""+QString::number(right)+"\" w:type=\"dxa\"/>\r\n";
        }

        aStream<<"    </w:tblCellMar>\r\n";
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

void WordXMLTableCellMargin::setMargin(double aTop, double aLeft, double aBottom, double aRight)
{
    top=aTop;
    left=aLeft;
    bottom=aBottom;
    right=aRight;
}
