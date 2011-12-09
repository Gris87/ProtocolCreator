#ifndef ENCODER_H
#define ENCODER_H

#include <QString>

void Xoring(QByteArray &aStream, const QByteArray &aKeyArray);
void ExchangeBytes(QByteArray &aStream, int index1, int index2);
void RandomExchange(QByteArray &aStream, const QByteArray &aKeyArray);

void Encrypt(QByteArray &aStream, const QByteArray &aKeyArray);
void Decrypt(QByteArray &aStream, const QByteArray &aKeyArray);

bool EncryptFile(const QString aSourceFileName, const QString aDestFileName, const QString aKey);
bool DecryptFile(const QString aSourceFileName, const QString aDestFileName, const QString aKey);

bool EncryptStream(QByteArray &aStream, const QString aKey);
bool DecryptStream(QByteArray &aStream, const QString aKey);

QString EncryptString(const QString aText, const QString aKey);
QString DecryptString(const QString aText, const QString aKey);

#endif // ENCODER_H
