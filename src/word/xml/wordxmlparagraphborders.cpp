#include "wordxmlparagraphborders.h"

WordXMLParagraphBorders::WordXMLParagraphBorders(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtParagraphBorders;

    reset();
}

WordXMLParagraphBorders::~WordXMLParagraphBorders()
{
}

void WordXMLParagraphBorders::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:pBdr>";

        if (
            topType!=btNone
            ||
            topSize>=0
            ||
            topBorderWidth>=0
            ||
            topSpace>=0
            ||
            topColor=""
           )
        {
            aStream<<space<<" <w:top";

            if (topType!=btNone)
            {
                aStream<<" w:val=\"";

                switch(topType)
                {
                    case btSingle:
                    {
                        aStream<<"single";
                    }
                    break;
                    case btNone:
                    {
                        // Nothing
                    }
                    break;
                }

                aStream<<"\"";
            }

            if (topSize>=0)
            {
                aStream<<" w:sz=\""+QString::number(topSize)+"\"";
            }

            if (topBorderWidth>=0)
            {
                aStream<<" w:bdrwidth=\""+QString::number(topBorderWidth)+"\"";
            }

            if (topSpace>=0)
            {
                aStream<<" w:space=\""+QString::number(topSpace)+"\"";
            }

            if (topColor!="")
            {
                aStream<<" w:color=\""+topColor+"\"";
            }
        }

        if (
            leftType!=btNone
            ||
            leftSize>=0
            ||
            leftBorderWidth>=0
            ||
            leftSpace>=0
            ||
            leftColor=""
           )
        {
            aStream<<space<<" <w:left";

            if (leftType!=btNone)
            {
                aStream<<" w:val=\"";

                switch(leftType)
                {
                    case btSingle:
                    {
                        aStream<<"single";
                    }
                    break;
                    case btNone:
                    {
                        // Nothing
                    }
                    break;
                }

                aStream<<"\"";
            }

            if (leftSize>=0)
            {
                aStream<<" w:sz=\""+QString::number(leftSize)+"\"";
            }

            if (leftBorderWidth>=0)
            {
                aStream<<" w:bdrwidth=\""+QString::number(leftBorderWidth)+"\"";
            }

            if (leftSpace>=0)
            {
                aStream<<" w:space=\""+QString::number(leftSpace)+"\"";
            }

            if (leftColor!="")
            {
                aStream<<" w:color=\""+leftColor+"\"";
            }
        }

        if (
            bottomType!=btNone
            ||
            bottomSize>=0
            ||
            bottomBorderWidth>=0
            ||
            bottomSpace>=0
            ||
            bottomColor=""
           )
        {
            aStream<<space<<" <w:bottom";

            if (bottomType!=btNone)
            {
                aStream<<" w:val=\"";

                switch(bottomType)
                {
                    case btSingle:
                    {
                        aStream<<"single";
                    }
                    break;
                    case btNone:
                    {
                        // Nothing
                    }
                    break;
                }

                aStream<<"\"";
            }

            if (bottomSize>=0)
            {
                aStream<<" w:sz=\""+QString::number(bottomSize)+"\"";
            }

            if (bottomBorderWidth>=0)
            {
                aStream<<" w:bdrwidth=\""+QString::number(bottomBorderWidth)+"\"";
            }

            if (bottomSpace>=0)
            {
                aStream<<" w:space=\""+QString::number(bottomSpace)+"\"";
            }

            if (bottomColor!="")
            {
                aStream<<" w:color=\""+bottomColor+"\"";
            }
        }

        if (
            rightType!=btNone
            ||
            rightSize>=0
            ||
            rightBorderWidth>=0
            ||
            rightSpace>=0
            ||
            rightColor=""
           )
        {
            aStream<<space<<" <w:right";

            if (rightType!=btNone)
            {
                aStream<<" w:val=\"";

                switch(rightType)
                {
                    case btSingle:
                    {
                        aStream<<"single";
                    }
                    break;
                    case btNone:
                    {
                        // Nothing
                    }
                    break;
                }

                aStream<<"\"";
            }

            if (rightSize>=0)
            {
                aStream<<" w:sz=\""+QString::number(rightSize)+"\"";
            }

            if (rightBorderWidth>=0)
            {
                aStream<<" w:bdrwidth=\""+QString::number(rightBorderWidth)+"\"";
            }

            if (rightSpace>=0)
            {
                aStream<<" w:space=\""+QString::number(rightSpace)+"\"";
            }

            if (rightColor!="")
            {
                aStream<<" w:color=\""+rightColor+"\"";
            }
        }

        aStream<<space<<"</w:pBdr>";
    }
}

void WordXMLParagraphBorders::reset()
{
    WordXMLBase::reset();

    topType=btNone;
    topSize=-1;
    topBorderWidth=-1;
    topSpace=-1;
    topColor="";

    leftType=btNone;
    leftSize=-1;
    leftBorderWidth=-1;
    leftSpace=-1;
    leftColor="";

    bottomType=btNone;
    bottomSize=-1;
    bottomBorderWidth=-1;
    bottomSpace=-1;
    bottomColor="";

    rightType=btNone;
    rightSize=-1;
    rightBorderWidth=-1;
    rightSpace=-1;
    rightColor="";
}

bool WordXMLParagraphBorders::isModified()
{
    return (
            topType!=btNone
            ||
            topSize>=0
            ||
            topBorderWidth>=0
            ||
            topSpace>=0
            ||
            topColor=""
            ||
            leftType!=btNone
            ||
            leftSize>=0
            ||
            leftBorderWidth>=0
            ||
            leftSpace>=0
            ||
            leftColor=""
            ||
            bottomType!=btNone
            ||
            bottomSize>=0
            ||
            bottomBorderWidth>=0
            ||
            bottomSpace>=0
            ||
            bottomColor=""
            ||
            rightType!=btNone
            ||
            rightSize>=0
            ||
            rightBorderWidth>=0
            ||
            rightSpace>=0
            ||
            rightColor=""
           );
}

void WordXMLParagraphBorders::setTopColor(QColor aColor)
{
    topColor=colorToString(aColor);
}

void WordXMLParagraphBorders::setLeftColor(QColor aColor)
{
    leftColor=colorToString(aColor);
}

void WordXMLParagraphBorders::setBottomColor(QColor aColor)
{
    bottomColor=colorToString(aColor);
}

void WordXMLParagraphBorders::setRightColor(QColor aColor)
{
    rightColor=colorToString(aColor);
}

WordXMLParagraphBorders& WordXMLParagraphBorders::operator=(const WordXMLParagraphBorders &another)
{
    topType=another.topType;
    topSize=another.topSize;
    topBorderWidth=another.topBorderWidth;
    topSpace=another.topSpace;
    topColor=another.topColor;

    leftType=another.leftType;
    leftSize=another.leftSize;
    leftBorderWidth=another.leftBorderWidth;
    leftSpace=another.leftSpace;
    leftColor=another.leftColor;

    bottomType=another.bottomType;
    bottomSize=another.bottomSize;
    bottomBorderWidth=another.bottomBorderWidth;
    bottomSpace=another.bottomSpace;
    bottomColor=another.bottomColor;

    rightType=another.rightType;
    rightSize=another.rightSize;
    rightBorderWidth=another.rightBorderWidth;
    rightSpace=another.rightSpace;
    rightColor=another.rightColor;

    return *this;
}

