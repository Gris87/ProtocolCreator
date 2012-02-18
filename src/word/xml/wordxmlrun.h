#ifndef     WORDXMLRUN_H
#define     WORDXMLRUN_H

#include "wordxmlbase.h"

#include "wordxmlrunproperties.h"

#include "wordxmltext.h"
#include "wordxmlseparator.h"
#include "wordxmlcontinuationseparator.h"
#include "wordxmlbreak.h"
#include "wordxmltabchar.h"
#include "wordxmlpagenumber.h"
#include "wordxmlfieldchar.h"

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

    WordXMLText* addText(QString aText);
    WordXMLSeparator* addSeparator();
    WordXMLContinuationSeparator* addContinuationSeparator();
    WordXMLBreak* addBreak();
    WordXMLTabChar* addTabChar();
    WordXMLPageNumber* addPageNumber();
    WordXMLFieldChar* addFieldChar(EFieldCharType aType);

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

    WordXMLRun& operator=(const WordXMLRun &another);

private:
    QList<WordXMLBase*> mList;
};

#endif //     WORDXMLRUN_H
