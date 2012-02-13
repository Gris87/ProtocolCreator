#ifndef WORDXMLFONTS_H
#define WORDXMLFONTS_H

#include <QTextStream>

class WordXMLFonts
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
