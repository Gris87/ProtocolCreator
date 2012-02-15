#include "wordxmlcompatibility.h"

WordXMLCompatibility::WordXMLCompatibility()
{
    reset();
}

WordXMLCompatibility::~WordXMLCompatibility()
{
}

void WordXMLCompatibility::writeToStream(QTextStream &aStream)
{
    if (
        isBreakWrappedTables
        ||
        isSnapToGridInCell
        ||
        isWrapTextWithPunct
        ||
        isUseAsianBreakRules
        ||
        isDontGrowAutofit
        ||
        isUseFELayout
       )
    {
        aStream<<"   <w:compat>\r\n";

        if (isBreakWrappedTables)
        {
            aStream<<"    <w:breakWrappedTables/>\r\n";
        }

        if (isSnapToGridInCell)
        {
            aStream<<"    <w:snapToGridInCell/>\r\n";
        }

        if (isWrapTextWithPunct)
        {
            aStream<<"    <w:wrapTextWithPunct/>\r\n";
        }

        if (isUseAsianBreakRules)
        {
            aStream<<"    <w:useAsianBreakRules/>\r\n";
        }

        if (isDontGrowAutofit)
        {
            aStream<<"    <w:dontGrowAutofit/>\r\n";
        }

        if (isUseFELayout)
        {
            aStream<<"    <w:useFELayout/>\r\n";
        }

        aStream<<"   </w:compat>\r\n";
    }
}

void WordXMLCompatibility::reset()
{
    isBreakWrappedTables=false;
    isSnapToGridInCell=false;
    isWrapTextWithPunct=false;
    isUseAsianBreakRules=false;
    isDontGrowAutofit=false;
    isUseFELayout=false;
}
