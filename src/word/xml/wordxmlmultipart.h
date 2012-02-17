#ifndef WORDXMLMULTIPART_H
#define WORDXMLMULTIPART_H

#include "wordxmlbase.h"

#include <QTextEdit>

class WordXMLParagraph;
class WordXMLTable;

class WordXMLMultiPart : public WordXMLBase
{
public:
    explicit WordXMLMultiPart(WordXMLBase* aParent=0);
    ~WordXMLMultiPart();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void insertFromText(QTextEdit *aTextEdit);

    WordXMLParagraph* addParagraph();
    WordXMLTable* addTable();

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

protected:
    QList<WordXMLBase*> mList;
};

#endif // WORDXMLMULTIPART_H
