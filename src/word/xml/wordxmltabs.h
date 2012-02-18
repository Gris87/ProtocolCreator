#ifndef WORDXMLTABS_H
#define WORDXMLTABS_H

#include <QDateTime>
#include <QVariant>

#include "wordxmlbase.h"

#include "wordxmltab.h"

class WordXMLTabs : public WordXMLBase
{
public:
    explicit WordXMLTabs(WordXMLBase* aParent=0);
    ~WordXMLTabs();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTab* add(ETabLocation aLocation, int aPosition);

    WordXMLTab* copy(int index, int aPosition);
    WordXMLTab* copy(WordXMLTab* aTab, int aPosition);

    WordXMLTab* get(int index);

    void remove(int index);
    void remove(WordXMLTab* aTab);

    int count();

    void clear();

    WordXMLTabs& operator=(const WordXMLTabs &another);

private:
    QList<WordXMLTab*> mList;
};

#endif // WORDXMLTABS_H
