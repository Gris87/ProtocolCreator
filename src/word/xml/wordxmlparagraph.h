#ifndef     WORDXMLPARAGRAPH_H
#define     WORDXMLPARAGRAPH_H

#include "wordxmlbase.h"

#include <QTextCharFormat>

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

    explicit WordXMLParagraph(WordXMLBase* aParent=0);
    ~WordXMLParagraph();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setFont(QTextCharFormat aFormat);

    WordXMLRun* addRun();

    WordXMLRun* copyRun(int index);
    WordXMLRun* copyRun(WordXMLRun* aRun);

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

    WordXMLParagraph& operator=(const WordXMLParagraph &another);

private:
    QList<WordXMLBase*> mList;
};

#endif //     WORDXMLPARAGRAPH_H
