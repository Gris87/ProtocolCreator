#ifndef WORDXMLCUSTOMPROPERTIES_H
#define WORDXMLCUSTOMPROPERTIES_H

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

struct SCustomProperty
{
    QString name;
    ECustomPropertyType type;
    QString textValue;
    QDateTime dateValue;
    double floatValue;
    bool booleanValue;
};

class WordXMLCustomProperties
{
public:
    explicit WordXMLCustomProperties();
    ~WordXMLCustomProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    QVariant getValue(SCustomProperty* aProperty);
    void setValue(SCustomProperty* aProperty, QVariant aValue);

    SCustomProperty* add(QString aName, QVariant aValue);

    SCustomProperty* copy(QString aName, QString aSecondName);
    SCustomProperty* copy(int index, QString aSecondName);
    SCustomProperty* copy(SCustomProperty* aProperty, QString aSecondName);

    SCustomProperty* get(QString aName);
    SCustomProperty* get(int index);

    void remove(QString aName);
    void remove(int index);
    void remove(SCustomProperty* aProperty);

    int count();

private:
    QList<SCustomProperty*> propList;

    void clear();
};

#endif // WORDXMLCUSTOMPROPERTIES_H
