#include "src/other/global.h"

void Xoring(QByteArray &aStream, const QByteArray &aKeyArray)
{
    for (int i=0; i<aStream.length(); i++)
    {
        aStream[i]=aStream.at(i) ^ aKeyArray.at(i % aKeyArray.length());
    }
}

void ExchangeBytes(QByteArray &aStream, int index1, int index2)
{
    index1=index1 % aStream.length();
    index2=index2 % aStream.length();

    if (index1<0)
    {
        index1=-index1 % 8;
    }

    if (index2<0)
    {
        index2=-index2 % 8;
    }

    char temp=aStream.at(index1);
    aStream[index1]=aStream.at(index2);
    aStream[index2]=temp;
}

void RandomExchange(QByteArray &aStream, const QByteArray &aKeyArray, const bool isKey, const bool isForwardWay, const int encryptStep)
{
    quint8 choice;

    if (isKey)
    {
        choice=(qChecksum(aKeyArray.data(), aKeyArray.length())*315 - (encryptStep*37)) % 8;
    }
    else
    {
        choice=(qChecksum(aKeyArray.data(), aKeyArray.length())*217 + (encryptStep*23)) % 8;
    }

    if (choice<0)
    {
        choice=-choice % 8;
    }

    int i;

    if (isForwardWay)
    {
        i=0;
    }
    else
    {
        i=aStream.length()-1;
    }

    switch (choice)
    {
        case 0:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, (i*3)+1);

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 1:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, (i*3)-1);

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 2:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, (i*7)+1);

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 3:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, (i*7)-1);

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 4:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, i*i);

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 5:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, floor(exp(i)));

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 6:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, floor((aStream.length()-1)*(0.5+sin(i)*0.5)));

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
        case 7:
        {
            while ((i>=0) && (i<=aStream.length()-1))
            {
                ExchangeBytes(aStream, i, floor((aStream.length()-1)*(0.5+cos(i)*0.5)));

                i=i+(isForwardWay? 1 : -1);
            }
        }
        break;
    }
}

void Encrypt(QByteArray &aStream, const QByteArray &aKeyArray1)
{
    QByteArray aKeyArray2=aKeyArray1;
    QByteArray aKeyArray3=aKeyArray1;
    QByteArray aKeyArray4=aKeyArray1;

    // Prepare keys
    RandomExchange(aKeyArray2, aKeyArray1, true, true, 1);
    RandomExchange(aKeyArray3, aKeyArray2, true, true, 2);
    RandomExchange(aKeyArray4, aKeyArray3, true, true, 3);

    Xoring(aStream, aKeyArray1);
    RandomExchange(aStream, aKeyArray1, false, true, 1);

    Xoring(aStream, aKeyArray2);
    RandomExchange(aStream, aKeyArray2, false, true, 2);

    Xoring(aStream, aKeyArray3);
    RandomExchange(aStream, aKeyArray3, false, true, 3);

    Xoring(aStream, aKeyArray4);
    RandomExchange(aStream, aKeyArray4, false, true, 4);
}

void Decrypt(QByteArray &aStream, const QByteArray &aKeyArray1)
{
    QByteArray aKeyArray2=aKeyArray1;
    QByteArray aKeyArray3=aKeyArray1;
    QByteArray aKeyArray4=aKeyArray1;

    // Prepare keys
    RandomExchange(aKeyArray2, aKeyArray1, true, true, 1);
    RandomExchange(aKeyArray3, aKeyArray2, true, true, 2);
    RandomExchange(aKeyArray4, aKeyArray3, true, true, 3);

    RandomExchange(aStream, aKeyArray4, false, false, 4);
    Xoring(aStream, aKeyArray4);

    RandomExchange(aStream, aKeyArray3, false, false, 3);
    Xoring(aStream, aKeyArray3);

    RandomExchange(aStream, aKeyArray2, false, false, 2);
    Xoring(aStream, aKeyArray2);

    RandomExchange(aStream, aKeyArray1, false, false, 1);
    Xoring(aStream, aKeyArray1);
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

    for (int i=0; i<aKey.length(); i++)
    {
        quint8 aOneByte=aKey.at(i).unicode() & 255;
        aKeyDataStream << aOneByte;
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

    for (int i=0; i<aKey.length(); i++)
    {
        quint8 aOneByte=aKey.at(i).unicode() & 255;
        aKeyDataStream << aOneByte;
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
        ushort aUnicode;

        aDataStream.device()->seek(0);

        while (!aDataStream.atEnd())
        {
            aDataStream >> aUnicode;
            aResult.append(QChar(aUnicode));
        }
    }

    return aResult;
}

QString DecryptString(const QString aText, const QString aKey)
{
    if (aText.length()<=2)
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
        ushort aUnicode;

        aDataStream.device()->seek(0);

        while (!aDataStream.atEnd())
        {
            aDataStream >> aUnicode;
            aResult.append(QChar(aUnicode));
        }
    }

    return aResult;
}
