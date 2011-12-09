#include "src/other/global.h"

void Xoring(QByteArray &aStream, const QByteArray &aKeyArray)
{

}

void ExchangeBytes(QByteArray &aStream, int index1, int index2)
{
    index1=index1 % aStream.size();
    index2=index2 % aStream.size();

    char temp=aStream.at(index1);
    aStream[index1]=aStream.at(index2);
    aStream[index1]=temp;
}

void RandomExchange(QByteArray &aStream, const QByteArray &aKeyArray)
{

}

void Encrypt(QByteArray &aStream, const QByteArray &aKeyArray)
{

}

void Decrypt(QByteArray &aStream, const QByteArray &aKeyArray)
{

}

bool EncryptFile(const QString aSourceFileName, const QString aDestFileName, const QString aKey)
{
    if (aKey=="")
    {
        return false;
    }

    QFile aFile(aSourceFileName);

    if (!aFile.exists())
    {
        return false;
    }

    QByteArray aStream;

    aFile.open(QIODevice::ReadOnly);
    aStream=aFile.readAll();
    aFile.close();

    if (EncryptStream(aStream, aKey))
    {
        QFile aFile2(aDestFileName);

        aFile2.open(QIODevice::WriteOnly);
        aFile2.write(aStream);
        aFile2.close();

        return true;
    }

    return false;
}

bool DecryptFile(const QString aSourceFileName, const QString aDestFileName, const QString aKey)
{
    if (aKey=="")
    {
        return false;
    }

    QFile aFile(aSourceFileName);

    if (!aFile.exists())
    {
        return false;
    }

    QByteArray aStream;

    aFile.open(QIODevice::ReadOnly);
    aStream=aFile.readAll();
    aFile.close();

    if (DecryptStream(aStream, aKey))
    {
        QFile aFile2(aDestFileName);

        aFile2.open(QIODevice::WriteOnly);
        aFile2.write(aStream);
        aFile2.close();

        return true;
    }

    return false;
}

bool EncryptStream(QByteArray &aStream, const QString aKey)
{
    if (aKey=="")
    {
        return false;
    }

    aStream.insert(0, QString("GRIS").toAscii());

    QByteArray aKeyArray;
    QDataStream aKeyDataStream(&aKeyArray, QIODevice::WriteOnly);
    aKeyDataStream.setByteOrder(QDataStream::BigEndian);

    for (int i=0; i<aKey.length(); i++)
    {
        quint8 aUnicode=aKey.at(i).unicode() & 255;
        aKeyDataStream << aUnicode;
    }

    Encrypt(aStream, aKeyArray);

    return true;
}

bool DecryptStream(QByteArray &aStream, const QString aKey)
{
    if (aKey=="")
    {
        return false;
    }

    QByteArray aKeyArray;
    QDataStream aKeyDataStream(&aKeyArray, QIODevice::WriteOnly);
    aKeyDataStream.setByteOrder(QDataStream::BigEndian);

    for (int i=0; i<aKey.length(); i++)
    {
        quint8 aUnicode=aKey.at(i).unicode() & 255;
        aKeyDataStream << aUnicode;
    }

    Decrypt(aStream, aKeyArray);

    if (QString::fromAscii(aStream.left(4))=="GRIS")
    {
        aStream.remove(0, 4);
    }
    else
    {
        return false;
    }

    return true;
}

QString EncryptString(const QString aText, const QString aKey)
{
    QByteArray aStream;
    QDataStream aDataStream(&aStream, QIODevice::ReadWrite);
    aDataStream.setByteOrder(QDataStream::BigEndian);

    for (int i=0; i<aText.length(); i++)
    {
        ushort aUnicode=aText.at(i).unicode();
        aDataStream << aUnicode;
    }

    QString aResult="";

    if (EncryptStream(aStream, aKey))
    {
        aDataStream.device()->seek(0);

        while (!aDataStream.atEnd())
        {
            ushort aUnicode;
            aDataStream >> aUnicode;
            aResult.append(QChar(aUnicode));
        }
    }

    return aResult;
}

QString DecryptString(const QString aText, const QString aKey)
{
    if (aText.length()<4)
    {
        return "";
    }

    QByteArray aStream;
    QDataStream aDataStream(&aStream, QIODevice::ReadWrite);
    aDataStream.setByteOrder(QDataStream::BigEndian);

    for (int i=0; i<aText.length(); i++)
    {
        ushort aUnicode=aText.at(i).unicode();
        aDataStream << aUnicode;
    }

    QString aResult="";

    if (DecryptStream(aStream, aKey))
    {
        aDataStream.device()->seek(0);

        while (!aDataStream.atEnd())
        {
            ushort aUnicode;
            aDataStream >> aUnicode;
            aResult.append(QChar(aUnicode));
        }
    }

    return aResult;
}
