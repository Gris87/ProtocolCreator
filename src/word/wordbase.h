#ifndef WORDBASE_H
#define WORDBASE_H

#include <QObject>

#include <QAxObject>
#include <QStringList>
#include <QFile>

class WordWorkBase;

class WordBase : public QObject
{
    Q_OBJECT
public:
    explicit WordBase(QObject *parent = 0);
    ~WordBase();

    QVariant asVariant();
    QVariant dynamicCall(const char * function, const QVariant & var1 = QVariant(), const QVariant & var2 = QVariant(), const QVariant & var3 = QVariant(), const QVariant & var4 = QVariant(), const QVariant & var5 = QVariant(), const QVariant & var6 = QVariant(), const QVariant & var7 = QVariant(), const QVariant & var8 = QVariant());
    QVariant dynamicCall(const char * function, QList<QVariant> & vars);
    QString generateDocumentation();
    void saveDocumentation(QString aFileName);
    bool isNull() const;
    QAxObject::PropertyBag propertyBag() const;
    bool propertyWritable(const char * prop) const;
    WordWorkBase* querySubObject(const char * name, const QVariant & var1 = QVariant(), const QVariant & var2 = QVariant(), const QVariant & var3 = QVariant(), const QVariant & var4 = QVariant(), const QVariant & var5 = QVariant(), const QVariant & var6 = QVariant(), const QVariant & var7 = QVariant(), const QVariant & var8 = QVariant());
    WordWorkBase* querySubObject(const char * name, QList<QVariant> & vars);
    void setPropertyBag(const QAxObject::PropertyBag & bag);
    void setPropertyWritable(const char * prop, bool ok);
    QStringList verbs() const;
    bool doVerb(const QString & verb);

protected:
    QAxObject *base_ax_object;
};

#endif // WORDBASE_H
