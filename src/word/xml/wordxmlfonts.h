#ifndef WORDXMLFONTS_H
#define WORDXMLFONTS_H

#include "wordxmlbase.h"

class WordXMLFonts : public WordXMLBase
{
public:
    QString defaultAsciiFont;
    QString defaultFarEastFont;
    QString defaultHAsciiFont;
    QString defaultCSFont;

    explicit WordXMLFonts(WordXMLBase* aParent=0);
    ~WordXMLFonts();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLFONTS_H
