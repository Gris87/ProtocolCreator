#ifndef WORDXMLCUSTOMPROPERTIES_H
#define WORDXMLCUSTOMPROPERTIES_H

#include <QTextStream>
#include <QDateTime>

enum ECustomPropertyType
{
    cptText,
    cptDate,
    cptNumber,
    cptYesNo
};

struct SCustomProperty
{
    QString name;
    ECustomPropertyType type;
    QString textValue;
    QDateTime dateValue;
    double floatValue;
    bool booleanValue;
};

class WordXMLCustomProperties : public QList<SCustomProperty>
{
public:
    explicit WordXMLCustomProperties();
    ~WordXMLCustomProperties();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLCUSTOMPROPERTIES_H
