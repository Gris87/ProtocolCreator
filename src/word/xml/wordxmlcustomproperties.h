#ifndef WORDXMLCUSTOMPROPERTIES_H
#define WORDXMLCUSTOMPROPERTIES_H

#include <QDateTime>
#include <QVariant>

#include "wordxmlbase.h"

#include "wordxmlcustomproperty.h"

class WordXMLCustomProperties : public WordXMLBase
{
public:
    explicit WordXMLCustomProperties(WordXMLBase* aParent=0);
    ~WordXMLCustomProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLCustomProperty* add(QString aName, QVariant aValue);

    WordXMLCustomProperty* copy(QString aName, QString aSecondName);
    WordXMLCustomProperty* copy(int index, QString aSecondName);
    WordXMLCustomProperty* copy(WordXMLCustomProperty* aProperty, QString aSecondName);

    WordXMLCustomProperty* get(QString aName);
    WordXMLCustomProperty* get(int index);

    void remove(QString aName);
    void remove(int index);
    void remove(WordXMLCustomProperty* aProperty);

    int count();

    void clear();

private:
    QList<WordXMLCustomProperty*> mList;
};

#endif // WORDXMLCUSTOMPROPERTIES_H
