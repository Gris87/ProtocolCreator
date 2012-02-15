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

    explicit WordXMLFonts();
    ~WordXMLFonts();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLFONTS_H