#ifndef WORDXMLSECTIONS_H
#define WORDXMLSECTIONS_H

#include "wordxmlbase.h"

#include "wordxmlsection.h"

class WordXMLSections : public WordXMLBase
{
public:
    explicit WordXMLSections(WordXMLBase* aParent=0);
    ~WordXMLSections();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLSection* add();

    WordXMLSection* copy(int index);
    WordXMLSection* copy(WordXMLSection* aSection);

    WordXMLSection* get(int index);

    void remove(int index);
    void remove(WordXMLSection* aSection);

    int count();

    void clear();

    WordXMLSections& operator=(const WordXMLSections &another);

private:
    QList<WordXMLSection*> mList;
};

#endif // WORDXMLSECTIONS_H
