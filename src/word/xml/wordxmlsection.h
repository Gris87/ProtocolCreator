#ifndef WORDXMLSECTION_H
#define WORDXMLSECTION_H

#include "wordxmlbase.h"

#include "wordxmlsectionproperties.h"

#include "wordxmlparagraph.h"
#include "wordxmltable.h"

class WordXMLSection : public WordXMLBase
{
public:
    WordXMLSectionProperties properties;

    explicit WordXMLSection(WordXMLBase* aParent=0);
    ~WordXMLSection();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLParagraph* addParagraph();
    WordXMLTable* addTable();

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

private:
    QList<WordXMLBase*> mList;
};

#endif // WORDXMLSECTION_H
