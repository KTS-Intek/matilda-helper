#ifndef CONSOLECLASS_H
#define CONSOLECLASS_H

#include <QObject>
#include <QStringList>

class ConsoleClass : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleClass(QObject *parent = 0);

    int checkArgs(QStringList list);

    QByteArray showHelp();

    QByteArray convert2local8bit(const QStringList &list, bool &ok);

    QByteArray convert2utf8(const QStringList &list, bool &ok);

    QByteArray compressDecompressData(const bool &compress, const QStringList &list, bool &ok);


    QByteArray getHashSumm(const QStringList &list, bool &ok);


private:
    int hshSummVal(const QString &arg) ;

    QByteArray convertData(const QString &key, const QByteArray &inData, bool &ok);
    QByteArray convertOutData(const QString &key, const QByteArray &inData, bool &ok);

    QStringList getHshNames();

};

#endif // CONSOLECLASS_H
