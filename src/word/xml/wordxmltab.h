#ifndef WORDXMLTAB_H
#define WORDXMLTAB_H

#include <QTextStream>

enum ETabLocation
{
    tlLeft,
    tlCenter,
    tlRight
};

class WordXMLTab
{
public:
    ETabLocation location;
    int position;

    explicit WordXMLTab();
    ~WordXMLTab();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLTAB_H
