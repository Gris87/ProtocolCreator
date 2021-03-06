#include "wordxmltableborders.h"

WordXMLTableBorders::WordXMLTableBorders(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableBorders;

    reset();
}

WordXMLTableBorders::~WordXMLTableBorders()
{
}

void WordXMLTableBorders::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:tblBorders>\r\n";

        if (top!=tbNone)
        {
            aStream<<space<<" <w:top w:val=\"";

            switch (top)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (left!=tbNone)
        {
            aStream<<space<<" <w:left w:val=\"";

            switch (left)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (bottom!=tbNone)
        {
            aStream<<space<<" <w:bottom w:val=\"";

            switch (bottom)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (right!=tbNone)
        {
            aStream<<space<<" <w:right w:val=\"";

            switch (right)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (insideH!=tbNone)
        {
            aStream<<space<<" <w:insideH w:val=\"";

            switch (insideH)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (insideV!=tbNone)
        {
            aStream<<space<<" <w:insideV w:val=\"";

            switch (insideV)
            {
                case tbSingle:
                {
                    aStream<<"single";
                }
                case tbNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        aStream<<space<<"</w:tblBorders>\r\n";
    }
}

void WordXMLTableBorders::reset()
{
    WordXMLBase::reset();

    top=tbNone;
    left=tbNone;
    bottom=tbNone;
    right=tbNone;
    insideH=tbNone;
    insideV=tbNone;
}

bool WordXMLTableBorders::isModified()
{
    return (
            (top!=tbNone)
            ||
            (left!=tbNone)
            ||
            (bottom!=tbNone)
            ||
            (right!=tbNone)
            ||
            (insideH!=tbNone)
            ||
            (insideV!=tbNone)
           );
}

void WordXMLTableBorders::setBorders(ETableBorder type)
{
    top=type;
    left=type;
    bottom=type;
    right=type;
    insideH=type;
    insideV=type;
}

WordXMLTableBorders& WordXMLTableBorders::operator=(const WordXMLTableBorders &another)
{
    top=another.top;
    left=another.left;
    bottom=another.bottom;
    right=another.right;
    insideH=another.insideH;
    insideV=another.insideV;

    return *this;
}

