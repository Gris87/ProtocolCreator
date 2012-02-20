#ifndef WORDXMLMULTIPART_H
#define WORDXMLMULTIPART_H

#include "wordxmlbase.h"

#include <QTextFrame>
#include <QTextTable>
#include <QTextCursor>

class WordXMLParagraph;
class WordXMLRun;
class WordXMLTable;

class WordXMLMultiPart : public WordXMLBase
{
public:
    explicit WordXMLMultiPart(WordXMLBase* aParent=0);
    ~WordXMLMultiPart();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void insertFromText(QTextFrame *aFrame);

    WordXMLParagraph* addParagraph();
    WordXMLTable* addTable();

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

    WordXMLMultiPart& operator=(const WordXMLMultiPart &another);

protected:
    QList<WordXMLBase*> mList;

    void insertByTextCursor(QTextCursor cursor, const int start, const int end);
    void putTextWithFormat(WordXMLParagraph *paragraph, WordXMLRun *run, QString aText, QTextCharFormat aFormat);
};

#endif // WORDXMLMULTIPART_H
