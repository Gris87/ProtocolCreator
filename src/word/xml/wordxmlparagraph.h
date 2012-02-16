#ifndef     WORDXMLPARAGRAPH_H
#define     WORDXMLPARAGRAPH_H

#include "wordxmlbase.h"

#include "wordxmlparagraphproperties.h"
#include "wordxmlrun.h"

class WordXMLParagraph : public WordXMLBase
{
public:
    QString rsidR;
    QString rsidRPr;
    QString rsidRDefault;
    QString rsidP;
    WordXMLParagraphProperties properties;

    explicit WordXMLParagraph();
    ~WordXMLParagraph();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLRun* addRun();

    WordXMLRun* copyRun(int index);
    WordXMLRun* copyRun(WordXMLRun* aRun);

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

private:
    QList<WordXMLBase*> mList;

    void clear();
};

#endif //     WORDXMLPARAGRAPH_H
