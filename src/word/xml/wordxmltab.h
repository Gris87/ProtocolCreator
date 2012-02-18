#ifndef WORDXMLTAB_H
#define WORDXMLTAB_H

#include "wordxmlbase.h"

enum ETabLocation
{
    tlLeft,
    tlCenter,
    tlRight
};

class WordXMLTab : public WordXMLBase
{
public:
    ETabLocation location;
    int position;

    explicit WordXMLTab(WordXMLBase* aParent=0);
    ~WordXMLTab();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTab& operator=(const WordXMLTab &another);
};

#endif // WORDXMLTAB_H
