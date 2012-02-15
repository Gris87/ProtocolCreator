#ifndef WORDXMLDOCUMENTPROPERTIES_H
#define WORDXMLDOCUMENTPROPERTIES_H

#include <QTextStream>
#include <QStringList>

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

enum ETripleState
{
    tsNone,
    tsOn,
    tsOff
};

class WordXMLDocumentProperties
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
    QStringList rsidsList;

    explicit WordXMLDocumentProperties();
    ~WordXMLDocumentProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    void setDefaultProperties2003();
};

#endif // WORDXMLDOCUMENTPROPERTIES_H
