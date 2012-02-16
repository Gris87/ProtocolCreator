#include "wordxmldocumentproperties.h"

WordXMLDocumentProperties::WordXMLDocumentProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    footnoteProperties.parent=this;
    endnoteProperties.parent=this;
    compatibility.parent=this;

    reset();
}

WordXMLDocumentProperties::~WordXMLDocumentProperties()
{
}

void WordXMLDocumentProperties::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:docPr>\r\n";

    aStream<<space<<" <w:view w:val=\"";

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
        aStream<<space<<" <w:zoom w:percent=\""+QString::number(zoom)+"\"/>\r\n";
    }

    if (isDoNotEmbedSystemFonts)
    {
        aStream<<space<<" <w:doNotEmbedSystemFonts/>\r\n";
    }

    if (proofStateSpelling!="" || proofStateGrammar!="")
    {
        aStream<<space<<" <w:proofState";

        if (proofStateSpelling!="")
        {
            aStream<<space<<"w:spelling=\""+proofStateSpelling+"\"";
        }

        if (proofStateGrammar!="")
        {
            aStream<<space<<"w:grammar=\""+proofStateGrammar+"\"";
        }

        aStream<<"/>\r\n";
    }

    aStream<<space<<" <w:attachedTemplate w:val=\""+attachedTemplate+"\"/>\r\n";

    if (defaultTabStop>=0)
    {
        aStream<<space<<" <w:defaultTabStop w:val=\""+QString::number(defaultTabStop)+"\"/>\r\n";
    }

    if (isPunctuationKerning)
    {
        aStream<<space<<" <w:punctuationKerning/>\r\n";
    }

    aStream<<space<<" <w:characterSpacingControl w:val=\"";

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
        aStream<<space<<" <w:optimizeForBrowser/>\r\n";
    }

    if (isValidateAgainstSchema)
    {
        aStream<<space<<" <w:validateAgainstSchema/>\r\n";
    }

    if (saveInvalidXML!=tsNone)
    {
        aStream<<space<<" <w:saveInvalidXML w:val=\"";

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
        aStream<<space<<" <w:ignoreMixedContent w:val=\"";

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
        aStream<<space<<" <w:alwaysShowPlaceholderText w:val=\"";

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
        aStream<<space<<" <wsp:rsids>\r\n";

        for (int i=0; i<rsidsList.length(); i++)
        {
            if (i==0)
            {
                aStream<<space<<"  <wsp:rsidRoot";
            }
            else
            {
                aStream<<space<<"  <wsp:rsid";
            }

            aStream<<space<<"wsp:val=\""+rsidsList.at(i)+"\"/>\r\n";
        }

        aStream<<space<<" </wsp:rsids>\r\n";
    }

    aStream<<space<<"</w:docPr>\r\n";
}

void WordXMLDocumentProperties::reset()
{
    WordXMLBase::reset();

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

bool WordXMLDocumentProperties::isModified()
{
    return view!=vtNormal
           ||
           zoom>=0
           ||
           isDoNotEmbedSystemFonts
           ||
           proofStateSpelling!=""
           ||
           proofStateGrammar!=""
           ||
           attachedTemplate!=""
           ||
           defaultTabStop>=0
           ||
           isPunctuationKerning
           ||
           characterSpacingControl!=cscDontCompress
           ||
           isOptimizeForBrowser
           ||
           isValidateAgainstSchema
           ||
           saveInvalidXML!=tsNone
           ||
           ignoreMixedContent!=tsNone
           ||
           alwaysShowPlaceholderText!=tsNone
           ||
           footnoteProperties.isModified()
           ||
           endnoteProperties.isModified()
           ||
           compatibility.isModified()
           ||
           rsidsList.length()>0;
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
    footnote->paragraph.addRun();

    footnote=footnoteProperties.add("continuation-separator");
    footnote->paragraph.addRun();

    WordXMLEndnote* endnote=endnoteProperties.add("separator");
    endnote->paragraph.addRun();

    endnote=endnoteProperties.add("continuation-separator");
    endnote->paragraph.addRun();

    compatibility.isBreakWrappedTables=true;
    compatibility.isSnapToGridInCell=true;
    compatibility.isWrapTextWithPunct=true;
    compatibility.isUseAsianBreakRules=true;
    compatibility.isDontGrowAutofit=true;
    compatibility.isUseFELayout=true;
}
