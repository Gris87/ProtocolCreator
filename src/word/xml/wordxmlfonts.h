#ifndef WORDXMLFONTS_H
#define WORDXMLFONTS_H

#include <QTextStream>

#include "wordxmlfont.h"

class WordXMLFonts : public QList<WordXMLFont>
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
