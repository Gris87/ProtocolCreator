#ifndef WORDXMLCUSTOMPROPERTY_H
#define WORDXMLCUSTOMPROPERTY_H

#include <QTextStream>
#include <QDateTime>
#include <QVariant>

enum ECustomPropertyType
{
    cptText,
    cptDate,
    cptNumber,
    cptYesNo
};

class WordXMLCustomProperty
{
public:
    QString name;
    ECustomPropertyType type;
    QString textValue;
    QDateTime dateValue;
    double floatValue;
    bool booleanValue;

    explicit WordXMLCustomProperty();
    ~WordXMLCustomProperty();

    void writeToStream(QTextStream &aStream);

    void reset();

    QVariant getValue();
    void setValue(QVariant aValue);
};

#endif // WORDXMLCUSTOMPROPERTY_H
