#ifndef WORDXMLCUSTOMPROPERTIES_H
#define WORDXMLCUSTOMPROPERTIES_H

#include <QTextStream>
#include <QDateTime>
#include <QVariant>

#include "wordxmlcustomproperty.h"

class WordXMLCustomProperties
{
public:
    explicit WordXMLCustomProperties();
    ~WordXMLCustomProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

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

private:
    QList<WordXMLCustomProperty*> propList;

    void clear();
};

#endif // WORDXMLCUSTOMPROPERTIES_H
