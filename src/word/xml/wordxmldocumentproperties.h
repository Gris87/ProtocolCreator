#ifndef WORDXMLDOCUMENTPROPERTIES_H
#define WORDXMLDOCUMENTPROPERTIES_H

#include <QStringList>

#include "wordxmlbase.h"

#include "wordxmltypes.h"
#include "wordxmlfootnoteproperties.h"
#include "wordxmlendnoteproperties.h"
#include "wordxmlcompatibility.h"

enum EViewType
{
    vtPrint,
    vtOutline,
    vtNormal,
    vtWeb,
    vtMasterPages
};

enum ECharacterSpacingControl
{
    cscDontCompress,
    cscCompressPunctuation,
    cscCompressPunctuationAndJapaneseKana
};

class WordXMLDocumentProperties : public WordXMLBase
{
public:
    EViewType view;
    int zoom;
    bool isDoNotEmbedSystemFonts;
    QString proofStateSpelling;
    QString proofStateGrammar;
    QString attachedTemplate;
    int defaultTabStop;
    bool isPunctuationKerning;
    ECharacterSpacingControl characterSpacingControl;
    bool isOptimizeForBrowser;
    bool isValidateAgainstSchema;
    ETripleState saveInvalidXML;
    ETripleState ignoreMixedContent;
    ETripleState alwaysShowPlaceholderText;
    WordXMLFootnoteProperties footnoteProperties;
    WordXMLEndnoteProperties endnoteProperties;
    WordXMLCompatibility compatibility;
    QStringList rsidsList;

    explicit WordXMLDocumentProperties(WordXMLBase* aParent=0);
    ~WordXMLDocumentProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setDefaultProperties2003();
};

#endif // WORDXMLDOCUMENTPROPERTIES_H
