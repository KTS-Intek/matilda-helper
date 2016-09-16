#include "consoleclass.h"

#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include <QCryptographicHash>
//#include <QDebug>

ConsoleClass::ConsoleClass(QObject *parent) : QObject(parent)
{

}

int ConsoleClass::checkArgs( QStringList list)
{


    QTextStream stream(stdout);

    bool ok = false;
    QByteArray msg;
    if(list.size() > 2){

        QStringList lk = QString("-l -u -hs").split(" ", QString::SkipEmptyParts);


        list.removeFirst();
        QString key = list.takeFirst();



        switch(lk.indexOf(key)){
        case 0: msg = convert2local8bit(list, ok); stream.setCodec(QTextCodec::codecForLocale());  break;
        case 1: msg = convert2utf8(list, ok); stream.setCodec("UTF-8"); break;
        case 2: msg = getHashSumm(list, ok); break;

        default: msg = "unknown key " + key.toLocal8Bit(); break;
        }

        if(!ok){
            stream << endl << msg << endl << endl;

            stream << "failed " << key << " " << list.join(" ") << " " << lk.indexOf(key) << " " << list.size() << endl;
        }
    }

    if(ok)
        stream << msg;
    else
        stream << showHelp() << endl;
    return ok ? 0 : 23;
}

QByteArray ConsoleClass::showHelp()
{
    QByteArray arr;
    arr.append("-l Convert UTF-8 to local  [in data format] [out format] [in data....]\r\n");
    arr.append("-u Convert local to UTF-8  [in data format] [out format] [in data....]\r\n");
    arr.append("-hs Get hash summ, arguments: [in data format] [hash algorithm] [out format] [in data....]\r\n"
               "\r\n\t [in data format]: -t text, -b base64, -h hex\r\n"
               "\t [hash algorithm]: Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512\r\n"
               "\t [out format]: -t text, -b base64, -h hex\r\n"
               "\t [in data ...]: input data");
    return arr;
}

QByteArray ConsoleClass::convert2local8bit(const QStringList &list, bool &ok)
{
    ok = true;
    if(list.size() < 3){
        ok = false;
        return QByteArray("size < 3, size = ") + QByteArray::number(list.size());
    }

    QByteArray inData = convertData(list.at(0), QString(list.mid(2).join(" ")).toLocal8Bit(), ok);

//    qDebug() << inData.toHex();

    inData = QString(inData).toLocal8Bit();

//    qDebug() << inData.toHex();

    return convertOutData(list.at(1), inData, ok);// list.join(" ").toLocal8Bit().toHex();
}

QByteArray ConsoleClass::convert2utf8(const QStringList &list, bool &ok)
{
    ok = true;
    if(list.size() < 3){
        ok = false;
        return QByteArray("size < 3, size = ") + QByteArray::number(list.size());
    }

//    return QVariant(QString(list.join(" "))).toByteArray().toHex();

    QByteArray inData = convertData(list.at(0), QVariant(list.mid(2).join(" ")).toByteArray(), ok);

    return convertOutData(list.at(1), inData, ok);
}

QByteArray ConsoleClass::getHashSumm(const QStringList &list, bool &ok)
{
//index
    // 0 - in data format - t text, -b base64, -h hex
    // 1 - hashAlg Md5 Sha1 Sha....
    // 2 - out format -h (hex), -b (base64), -t text
    // 3+ - in data

    ok = true;
    if(list.size() < 4){
        ok = false;
        return QByteArray("size < 4, size = ") + QByteArray::number(list.size());
    }

    QByteArray inData = convertData(list.at(0), list.mid(3).join(" ").toLocal8Bit(), ok);

    if(!ok)
        return inData;


    int alg = hshSummVal(list.at(1));
    if(alg < 0 || alg > getHshNames().size()){
        ok = false;
        inData = "unknown algorithm " + list.at(1).toLocal8Bit();
        return inData;
    }

    inData = convertOutData(list.at(2), QCryptographicHash::hash(inData, static_cast<QCryptographicHash::Algorithm>(alg)), ok );

    if(ok)
        return inData;
    else
        return inData;
}

int ConsoleClass::hshSummVal(const QString &arg)
{
    int alg;

       QStringList hashList = getHshNames();

       switch(hashList.indexOf(arg)){
       case 0: alg = QCryptographicHash::Md4; break;

       case 2: alg = QCryptographicHash::Sha1; break;

       case 3: alg = QCryptographicHash::Sha224; break;
       case 4: alg = QCryptographicHash::Sha256; break;
       case 5: alg = QCryptographicHash::Sha384; break;
       case 6: alg = QCryptographicHash::Sha512; break;

       case 7: alg = QCryptographicHash::Sha3_224; break;
       case 8: alg = QCryptographicHash::Sha3_256; break;
       case 9: alg = QCryptographicHash::Sha3_256; break;
       case 10: alg = QCryptographicHash::Sha3_512; break;


       default: alg = QCryptographicHash::Md5; break;

       }
       return alg;
}

QByteArray ConsoleClass::convertData(const QString &key, const QByteArray &inData, bool &ok)
{
    ok = true;
    QStringList lk = QString("-h -b -t").split(" ", QString::SkipEmptyParts);
    QByteArray outData;
    switch(lk.indexOf(key)){
    case 0: outData = QByteArray::fromHex(inData); break;
    case 1: outData = QByteArray::fromBase64(inData); break;
    case 2: outData = inData; break;
    default: ok = false; outData = "unknown key " + key.toLocal8Bit(); break;
    }
    return outData;
}

QByteArray ConsoleClass::convertOutData(const QString &key, const QByteArray &inData, bool &ok)
{
    ok = true;
    QStringList lk = QString("-h -b -t").split(" ", QString::SkipEmptyParts);
    QByteArray outData;
    switch(lk.indexOf(key)){
    case 0: outData = inData.toHex(); break;
    case 1: outData = inData.toBase64(QByteArray::OmitTrailingEquals); break;
    case 2: outData =  inData; break;
    default: ok = false; outData = "unknown key " + key.toLocal8Bit(); break;
    }
    return outData;
}

QStringList ConsoleClass::getHshNames()
{
    return QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");

}
