#ifndef     WORDXMLRUN_H
#define     WORDXMLRUN_H

#include "wordxmlbase.h"

#include "wordxmlrunproperties.h"

#include "wordxmlseparator.h"
#include "wordxmlcontinuationseparator.h"
#include "wordxmlbreak.h"
#include "wordxmltabchar.h"
#include "wordxmlpagenumber.h"

class WordXMLRun : public WordXMLBase
{
public:
    QString rsidR;
    WordXMLRunProperties properties;

    explicit WordXMLRun(WordXMLBase* aParent=0);
    ~WordXMLRun();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLSeparator* addSeparator();
    WordXMLContinuationSeparator* addContinuationSeparator();
    WordXMLBreak* addBreak();
    WordXMLTabChar* addTabChar();
    WordXMLPageNumber* addPageNumber();

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

private:
    QList<WordXMLBase*> mList;

    void clear();
};

#endif //     WORDXMLRUN_H
