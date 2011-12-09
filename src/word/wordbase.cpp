#include "word.h"

WordBase::WordBase(QObject *parent) :
    QObject(parent)
{
    base_ax_object=0;
}

WordBase::~WordBase()
{
    try
    {
        if (base_ax_object)
        {
            // TODO Do something
//            delete base_ax_object;
        }
    }
    catch (...)
    {
        qWarning()<<"WordBase::~WordBase(): Couldn't delete word object";
    }
}

QVariant WordBase::asVariant()
{
    try
    {
        return base_ax_object->asVariant();
    }
    catch (...)
    {
        qWarning()<<"WordBase::asVariant(): Couldn't convert asVariant";
    }

    return QVariant();
}

QVariant WordBase::dynamicCall(const char * function, const QVariant & var1, const QVariant & var2, const QVariant & var3, const QVariant & var4, const QVariant & var5, const QVariant & var6, const QVariant & var7, const QVariant & var8)
{
    try
    {
        return base_ax_object->dynamicCall(function, var1, var2, var3, var4, var5, var6, var7, var8);
    }
    catch (...)
    {
        qWarning()<<"WordBase::dynamicCall(): Couldn't perform function: "<<function;
    }

    return QVariant();
}

QVariant WordBase::dynamicCall(const char * function, QList<QVariant> & vars)
{
    try
    {
        return base_ax_object->dynamicCall(function, vars);
    }
    catch (...)
    {
        qWarning()<<"WordBase::dynamicCall(): Couldn't perform function: "<<function;
    }

    return QVariant();
}

QString WordBase::generateDocumentation()
{
    try
    {
        return base_ax_object->generateDocumentation();
    }
    catch (...)
    {
        qWarning()<<"WordBase::generateDocumentation(): Couldn't receive documentation";
    }

    return "";
}

void WordBase::saveDocumentation(QString aFileName)
{
    try
    {
        QFile aFile(aFileName);
        aFile.open(QIODevice::WriteOnly);

        QTextStream aStream(&aFile);

        aStream<<generateDocumentation();
    }
    catch (...)
    {
        qWarning()<<"WordBase::saveDocumentation(): Couldn't save documentation";
    }
}

bool WordBase::isNull() const
{
    try
    {
        return base_ax_object->isNull();
    }
    catch (...)
    {
        qWarning()<<"WordBase::isNull(): Couldn't check for null";
    }

    return true;
}

QAxObject::PropertyBag WordBase::propertyBag() const
{
    try
    {
        return base_ax_object->propertyBag();
    }
    catch (...)
    {
        qWarning()<<"WordBase::propertyBag(): Couldn't receive propertyBag";
    }

    return QAxObject::PropertyBag();
}

bool WordBase::propertyWritable(const char * prop) const
{
    try
    {
        return base_ax_object->propertyWritable(prop);
    }
    catch (...)
    {
        qWarning()<<"WordBase::propertyWritable(): Couldn't check for propertyWritable: "<<prop;
    }

    return true;
}

WordWorkBase* WordBase::querySubObject(const char * name, const QVariant & var1, const QVariant & var2, const QVariant & var3, const QVariant & var4, const QVariant & var5, const QVariant & var6, const QVariant & var7, const QVariant & var8)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject(name, var1, var2, var3, var4, var5, var6, var7, var8);

        if (aObject==0)
        {
            qWarning()<<"WordBase::querySubObject(): Retry #"+QString::number(retry+1)+" Couldn't perform querySubObject: "<<name;
            continue;
        }

        return new WordWorkBase(aObject, this);
    }

    throw "WordBase::querySubObject(): Couldn't perform querySubObject: "+QString(name);
}

WordWorkBase* WordBase::querySubObject(const char * name, QList<QVariant> & vars)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject(name, vars);

        if (aObject==0)
        {
            qWarning()<<"WordBase::querySubObject(): Retry #"+QString::number(retry+1)+" Couldn't perform querySubObject: "<<name;
            continue;
        }

        return new WordWorkBase(aObject, this);
    }

    throw "WordBase::querySubObject(): Couldn't perform querySubObject: "+QString(name);
}

void WordBase::setPropertyBag(const QAxObject::PropertyBag & bag)
{
    try
    {
        return base_ax_object->setPropertyBag(bag);
    }
    catch (...)
    {
        qWarning()<<"WordBase::setPropertyBag(): Couldn't setPropertyBag";
    }
}

void WordBase::setPropertyWritable(const char * prop, bool ok)
{
    try
    {
        return base_ax_object->setPropertyWritable(prop, ok);
    }
    catch (...)
    {
        qWarning()<<"WordBase::setPropertyWritable(): Couldn't setPropertyWritable: "<<prop;
    }
}

QStringList WordBase::verbs() const
{
    try
    {
        return base_ax_object->verbs();
    }
    catch (...)
    {
        qWarning()<<"WordBase::verbs(): Couldn't receive verbs";
    }

    return QStringList();
}

bool WordBase::doVerb(const QString & verb)
{
    try
    {
        return base_ax_object->doVerb(verb);
    }
    catch (...)
    {
        qWarning()<<"WordBase::doVerb(): Couldn't doVerb: "+verb;
    }

    return false;
}
