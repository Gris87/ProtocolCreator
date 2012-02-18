#ifndef     WORDXMLBASE_H
#define     WORDXMLBASE_H

#include <QTextStream>

enum EWordXMLType
{
    wxtUnknown,
    wxtMain,
    wxtBreak,
    wxtCompatibility,
    wxtContinuationSeparator,
    wxtCustomProperties,
    wxtCustomProperty,
    wxtDocumentProperties,
    wxtEndnote,
    wxtEndnoteProperties,
    wxtFieldChar,
    wxtFonts,
    wxtFootnote,
    wxtFootnoteProperties,
    wxtHeaderFooter,
    wxtMultiPart,
    wxtPageNumber,
    wxtParagraph,
    wxtParagraphProperties,
    wxtProperties,
    wxtRun,
    wxtRunProperties,
    wxtSection,
    wxtSectionProperties,
    wxtSections,
    wxtSeparator,
    wxtStyle,
    wxtStyles,
    wxtTab,
    wxtTabChar,
    wxtTable,
    wxtTableBorders,
    wxtTableCellmargin,
    wxtTableProperties,
    wxtTabs,
    wxtText
};

class WordXMLBase
{
public:
    WordXMLBase *parent;

    explicit WordXMLBase(WordXMLBase* aParent=0);
    ~WordXMLBase();

    virtual void writeToStream(QTextStream &aStream);

    virtual void reset();

    virtual bool isModified();

    EWordXMLType getComponentType();

protected:
    EWordXMLType componentType;
    QString space;
};

#endif //     WORDXMLBASE_H
