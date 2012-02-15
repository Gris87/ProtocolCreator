#ifndef WORDXMLTABS_H
#define WORDXMLTABS_H

#include <QTextStream>
#include <QDateTime>
#include <QVariant>

#include "wordxmltab.h"

class WordXMLTabs
{
public:
    explicit WordXMLTabs();
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

private:
    QList<WordXMLTab*> tabsList;

    void clear();
};

#endif // WORDXMLTABS_H
