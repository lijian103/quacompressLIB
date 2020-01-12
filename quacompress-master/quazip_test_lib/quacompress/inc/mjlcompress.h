#ifndef MJLCOMPRESS_H
#define MJLCOMPRESS_H
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QString>
#include "JlCompress.h"


class MJlCompress : public JlCompress
{
public:
    MJlCompress();
    ~MJlCompress();

    /************压缩文件************/
    static bool compressDirOrFile(const QString &zipDest,const QString &zipSrc);

    static bool compressDirOrFileEncrypt(const QString &zipDest,const QString &zipSrc,const QString &passWord=NULL );

    /************加密压缩文件************/
    static bool encryptZip(const QString &zipDest,const QString &zipSrc , const QString &passWord=NULL);

    /************解密压缩文件************/
    static bool deEncryptZip(const QString &zipDest,const QString &zipSrc ,  const QString &passWord=NULL);

    static bool compressFileEncrypt(QuaZip* zip, QString fileName, QString fileDest,const QString &passWord=NULL);

    static bool compressFileEncrypt(QString fileCompressed, QString file,const QString &passWord=NULL);


    static bool removeFile(QStringList listFile);

    static bool extractFiledeEncrypt(QuaZip* zip, QString fileName, QString fileDest,const QString &passWord=NULL) ;

    static QStringList extractDirdeEncrypt(QuaZip &zip, const QString &dir,const QString &passWord=NULL);

    static QStringList extractDirdeEncrypt(QString fileCompressed, QString dir,const QString &passWord=NULL);


    static bool compressSubDirEncrypt(QuaZip* zip, QString dir, QString origDir, bool recursive, QDir::Filters filters,const QString &passWord=NULL);

    static bool compressDirEncrypt(QString fileCompressed, QString dir,bool recursive, QDir::Filters filters,const QString &passWord=NULL);

    static bool compressDirEncrypt(QString fileCompressed, QString dir,const QString &passWord=NULL, bool recursive = true) ;

};



#endif // MJLCOMPRESS_H
