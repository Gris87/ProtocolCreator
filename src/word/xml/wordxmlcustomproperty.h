#ifndef WORDXMLCUSTOMPROPERTY_H
#define WORDXMLCUSTOMPROPERTY_H

#include <QDateTime>
#include <QVariant>

#include "wordxmlbase.h"

enum ECustomPropertyType
{
    cptText,
    cptDate,
    cptNumber,
    cptYesNo
};

class WordXMLCustomProperty : public WordXMLBase
{
public:
    QString name;
    ECustomPropertyType type;
    QString textValue;
    QDateTime dateValue;
    double floatValue;
    bool booleanValue;

    explicit WordXMLCustomProperty(WordXMLBase* aParent=0);
    ~WordXMLCustomProperty();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    QVariant getValue();
    void setValue(QVariant aValue);
};

#endif // WORDXMLCUSTOMPROPERTY_H
