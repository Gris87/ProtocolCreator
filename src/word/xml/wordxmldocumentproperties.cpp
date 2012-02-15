#include "wordxmldocumentproperties.h"

WordXMLDocumentProperties::WordXMLDocumentProperties() : WordXMLBase()
{
    reset();
}

WordXMLDocumentProperties::~WordXMLDocumentProperties()
{
}

void WordXMLDocumentProperties::writeToStream(QTextStream &aStream)
{
    aStream<<" <w:docPr>\r\n";

    aStream<<"  <w:view w:val=\"";

    switch(view)
    {
        case vtPrint:
        {
            aStream<<"print";
        }
        break;
        case vtOutline:
        {
            aStream<<"outline";
        }
        break;
        case vtNormal:
        {
            aStream<<"normal";
        }
        break;
        case vtWeb:
        {
            aStream<<"web";
        }
        break;
        case vtMasterPages:
        {
            aStream<<"master-pages";
        }
        break;
    }

    aStream<<"\"/>\r\n";

    if (zoom>=0)
    {
        aStream<<"  <w:zoom w:percent=\""+QString::number(zoom)+"\"/>\r\n";
    }

    if (isDoNotEmbedSystemFonts)
    {
        aStream<<"  <w:doNotEmbedSystemFonts/>\r\n";
    }

    if (proofStateSpelling!="" || proofStateGrammar!="")
    {
        aStream<<"  <w:proofState";

        if (proofStateSpelling!="")
        {
            aStream<<" w:spelling=\""+proofStateSpelling+"\"";
        }

        if (proofStateGrammar!="")
        {
            aStream<<" w:grammar=\""+proofStateGrammar+"\"";
        }

        aStream<<"/>\r\n";
    }

    aStream<<"  <w:attachedTemplate w:val=\""+attachedTemplate+"\"/>\r\n";

    if (defaultTabStop>=0)
    {
        aStream<<"  <w:defaultTabStop w:val=\""+QString::number(defaultTabStop)+"\"/>\r\n";
    }

    if (isPunctuationKerning)
    {
        aStream<<"  <w:punctuationKerning/>\r\n";
    }

    aStream<<"  <w:characterSpacingControl w:val=\"";

    switch(characterSpacingControl)
    {
        case cscDontCompress:
        {
            aStream<<"DontCompress";
        }
        break;
        case cscCompressPunctuation:
        {
            aStream<<"CompressPunctuation";
        }
        break;
        case cscCompressPunctuationAndJapaneseKana:
        {
            aStream<<"CompressPunctuationAndJapaneseKana";
        }
        break;
    }

    aStream<<"\"/>\r\n";

    if (isOptimizeForBrowser)
    {
        aStream<<"  <w:optimizeForBrowser/>\r\n";
    }

    if (isValidateAgainstSchema)
    {
        aStream<<"  <w:validateAgainstSchema/>\r\n";
    }

    if (saveInvalidXML!=tsNone)
    {
        aStream<<"  <w:saveInvalidXML w:val=\"";

        if (saveInvalidXML==tsOn)
        {
            aStream<<"on";
        }
        else
        {
            aStream<<"off";
        }

        aStream<<"\"/>\r\n";
    }

    if (ignoreMixedContent!=tsNone)
    {
        aStream<<"  <w:ignoreMixedContent w:val=\"";

        if (ignoreMixedContent==tsOn)
        {
            aStream<<"on";
        }
        else
        {
            aStream<<"off";
        }

        aStream<<"\"/>\r\n";
    }

    if (alwaysShowPlaceholderText!=tsNone)
    {
        aStream<<"  <w:alwaysShowPlaceholderText w:val=\"";

        if (alwaysShowPlaceholderText==tsOn)
        {
            aStream<<"on";
        }
        else
        {
            aStream<<"off";
        }

        aStream<<"\"/>\r\n";
    }

    if (footnoteProperties.isModified())
    {
        aStream<<"\r\n";
        footnoteProperties.writeToStream(aStream);
    }

    if (endnoteProperties.isModified())
    {
        aStream<<"\r\n";
        endnoteProperties.writeToStream(aStream);
    }

    if (compatibility.isModified())
    {
        aStream<<"\r\n";
        compatibility.writeToStream(aStream);
    }

    if (rsidsList.length()>0)
    {
        aStream<<"  <wsp:rsids>\r\n";

        for (int i=0; i<rsidsList.length(); i++)
        {
            if (i==0)
            {
                aStream<<"   <wsp:rsidRoot";
            }
            else
            {
                aStream<<"   <wsp:rsid";
            }

            aStream<<" wsp:val=\""+rsidsList.at(i)+"\"/>\r\n";
        }

        aStream<<"  </wsp:rsids>\r\n";
    }

    aStream<<" </w:docPr>\r\n";
}

void WordXMLDocumentProperties::reset()
{
    view=vtNormal;
    zoom=-1;
    isDoNotEmbedSystemFonts=false;
    proofStateSpelling="";
    proofStateGrammar="";
    attachedTemplate="";
    defaultTabStop=-1;
    isPunctuationKerning=false;
    characterSpacingControl=cscDontCompress;
    isOptimizeForBrowser=false;
    isValidateAgainstSchema=false;
    saveInvalidXML=tsNone;
    ignoreMixedContent=tsNone;
    alwaysShowPlaceholderText=tsNone;

    footnoteProperties.reset();
    endnoteProperties.reset();
    compatibility.reset();

    rsidsList.clear();
}

void WordXMLDocumentProperties::setDefaultProperties2003()
{
    reset();

    view=vtPrint;
    zoom=100;
    isDoNotEmbedSystemFonts=true;
    proofStateSpelling="clean";
    proofStateGrammar="clean";
    attachedTemplate="";
    defaultTabStop=708;
    isPunctuationKerning=true;
    characterSpacingControl=cscDontCompress;
    isOptimizeForBrowser=true;
    isValidateAgainstSchema=true;
    saveInvalidXML=tsOff;
    ignoreMixedContent=tsOff;
    alwaysShowPlaceholderText=tsOff;

    WordXMLFootnote* footnote=footnoteProperties.add("separator");
    footnote=footnoteProperties.add("continuation-separator");

    WordXMLEndnote* endnote=endnoteProperties.add("separator");
    endnote=endnoteProperties.add("continuation-separator");

    compatibility.isBreakWrappedTables=true;
    compatibility.isSnapToGridInCell=true;
    compatibility.isWrapTextWithPunct=true;
    compatibility.isUseAsianBreakRules=true;
    compatibility.isDontGrowAutofit=true;
    compatibility.isUseFELayout=true;
}
