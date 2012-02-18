#include "wordxmlcompatibility.h"

WordXMLCompatibility::WordXMLCompatibility(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLCompatibility::~WordXMLCompatibility()
{
}

void WordXMLCompatibility::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:compat>\r\n";

        if (isBreakWrappedTables)
        {
            aStream<<space<<" <w:breakWrappedTables/>\r\n";
        }

        if (isSnapToGridInCell)
        {
            aStream<<space<<" <w:snapToGridInCell/>\r\n";
        }

        if (isWrapTextWithPunct)
        {
            aStream<<space<<" <w:wrapTextWithPunct/>\r\n";
        }

        if (isUseAsianBreakRules)
        {
            aStream<<space<<" <w:useAsianBreakRules/>\r\n";
        }

        if (isDontGrowAutofit)
        {
            aStream<<space<<" <w:dontGrowAutofit/>\r\n";
        }

        if (isUseFELayout)
        {
            aStream<<space<<" <w:useFELayout/>\r\n";
        }

        aStream<<space<<"</w:compat>\r\n";
    }
}

void WordXMLCompatibility::reset()
{
    WordXMLBase::reset();

    isBreakWrappedTables=false;
    isSnapToGridInCell=false;
    isWrapTextWithPunct=false;
    isUseAsianBreakRules=false;
    isDontGrowAutofit=false;
    isUseFELayout=false;
}

bool WordXMLCompatibility::isModified()
{
    return isBreakWrappedTables
           ||
           isSnapToGridInCell
           ||
           isWrapTextWithPunct
           ||
           isUseAsianBreakRules
           ||
           isDontGrowAutofit
           ||
           isUseFELayout;
}

WordXMLCompatibility& WordXMLCompatibility::operator=(const WordXMLCompatibility &another)
{
    isBreakWrappedTables=another.isBreakWrappedTables;
    isSnapToGridInCell=another.isSnapToGridInCell;
    isWrapTextWithPunct=another.isWrapTextWithPunct;
    isUseAsianBreakRules=another.isUseAsianBreakRules;
    isDontGrowAutofit=another.isDontGrowAutofit;
    isUseFELayout=another.isUseFELayout;

    return *this;
}
