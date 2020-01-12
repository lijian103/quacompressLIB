#include "JlCompress.h"
#include <QDebug>
#include <iostream>
#include "mjlcompress.h"

MJlCompress::MJlCompress()
{

}

static bool copyData(QIODevice &inFile, QIODevice &outFile)
{
    while (!inFile.atEnd()) {
        char buf[4096];
        qint64 readLen = inFile.read(buf, 4096);
        if (readLen <= 0)
            return false;
        if (outFile.write(buf, readLen) != readLen)
            return false;
    }
    return true;
}

bool MJlCompress::compressFileEncrypt(QuaZip* zip, QString fileName, QString fileDest,const QString &passWord)
{

    char*  password;
    QByteArray ba = passWord.toLatin1();
    if(passWord==NULL)
    {
        password=0;//0表示空指针char *  null
    }
    else
    {
        password=ba.data();
    }

    // zip: oggetto dove aggiungere il file
    // fileName: nome del file reale
    // fileDest: nome del file all'interno del file compresso

    // Controllo l'apertura dello zip
    if (!zip) return false;
    if (zip->getMode()!=QuaZip::mdCreate &&
        zip->getMode()!=QuaZip::mdAppend &&
        zip->getMode()!=QuaZip::mdAdd) return false;

    // Apro il file originale
    QFile inFile;
    inFile.setFileName(fileName);
    if(!inFile.open(QIODevice::ReadOnly)) return false;

    // Apro il file risulato
    QuaZipFile outFile(zip);
    if(!outFile.open(QIODevice::WriteOnly,
                     QuaZipNewInfo(fileDest, inFile.fileName()),
                     password, // 密码
                     0,        // CRC值（默认值是0）
                     8  // 写入方法（0为文件夹，8为普通文件）
                     )) return false;


    // Copio i dati
    if (!copyData(inFile, outFile) || outFile.getZipError()!=UNZ_OK) {
        return false;
    }

    // Chiudo i file
    outFile.close();
    if (outFile.getZipError()!=UNZ_OK) return false;
    inFile.close();

    return true;
}


bool MJlCompress::compressFileEncrypt(QString fileCompressed, QString file,const QString &passWord)
{
    // Creo lo zip
    QuaZip zip(fileCompressed);
    QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
    if(!zip.open(QuaZip::mdCreate)) {
        QFile::remove(fileCompressed);
        return false;
    }

    // Aggiungo il file
    if (!compressFileEncrypt(&zip,file,QFileInfo(file).fileName(),passWord)) {
        QFile::remove(fileCompressed);
        return false;
    }

    // Chiudo il file zip
    zip.close();
    if(zip.getZipError()!=0) {
        QFile::remove(fileCompressed);
        return false;
    }

    return true;
}


bool MJlCompress::compressSubDirEncrypt(QuaZip* zip, QString dir, QString origDir, bool recursive, QDir::Filters filters,const QString &passWord)
{
    char*  password;
    QByteArray ba = passWord.toLatin1();
    if(passWord==NULL)
    {
        password=0;
    }
    else
    {
        password=ba.data();
    }
    if(password==0)
    {
        //空语句，去除告警
    }
    // zip: oggetto dove aggiungere il file
    // dir: cartella reale corrente
    // origDir: cartella reale originale
    // (path(dir)-path(origDir)) = path interno all'oggetto zip

    // Controllo l'apertura dello zip
    if (!zip) return false;
    if (zip->getMode()!=QuaZip::mdCreate &&
        zip->getMode()!=QuaZip::mdAppend &&
        zip->getMode()!=QuaZip::mdAdd) return false;

    // Controllo la cartella
    QDir directory(dir);
    if (!directory.exists()) return false;

    QDir origDirectory(origDir);
    if (dir != origDir) {
        QuaZipFile dirZipFile(zip);

        if (!dirZipFile.open(QIODevice::WriteOnly,
                             QuaZipNewInfo(origDirectory.relativeFilePath(dir) + QLatin1String("/"), dir),
                             0, 0, 0))
//                             password, // 密码
//                             0,        // CRC值（默认值是0）
//                             8))  // 写入方法（0为文件夹，8为普通文件）
        {
                return false;
        }
        dirZipFile.close();
    }


    // Se comprimo anche le sotto cartelle
    if (recursive) {
        // Per ogni sotto cartella
        QFileInfoList files = directory.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot|filters);
        for (int index = 0; index < files.size(); ++index ) {
            const QFileInfo & file( files.at( index ) );
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 4)
            if (!file.isDir())
                continue;
#endif
            // Comprimo la sotto cartella
            if(!compressSubDirEncrypt(zip,file.absoluteFilePath(),origDir,recursive,filters,passWord)) return false;
        }
    }

    // Per ogni file nella cartella
    QFileInfoList files = directory.entryInfoList(QDir::Files|filters);
    for (int index = 0; index < files.size(); ++index ) {
        const QFileInfo & file( files.at( index ) );
        // Se non e un file o e il file compresso che sto creando
        if(!file.isFile()||file.absoluteFilePath()==zip->getZipName()) continue;

        // Creo il nome relativo da usare all'interno del file compresso
        QString filename = origDirectory.relativeFilePath(file.absoluteFilePath());

        // Comprimo il file
        if (!compressFileEncrypt(zip,file.absoluteFilePath(),filename,passWord)) return false;
    }

    return true;
}


bool MJlCompress::compressDirEncrypt(QString fileCompressed, QString dir, const QString &passWord,bool recursive)
{
    return compressDirEncrypt(fileCompressed, dir, recursive, 0,passWord);
}

bool MJlCompress::compressDirEncrypt(QString fileCompressed, QString dir,bool recursive, QDir::Filters filters,const QString &passWord)
{
    // Creo lo zip
    QuaZip zip(fileCompressed);
    QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
    if(!zip.open(QuaZip::mdCreate)) {
        QFile::remove(fileCompressed);
        return false;
    }

    // Aggiungo i file e le sotto cartelle
    if (!compressSubDirEncrypt(&zip,dir,dir,recursive, filters,passWord)) {
        QFile::remove(fileCompressed);
        return false;
    }

    // Chiudo il file zip
    zip.close();
    if(zip.getZipError()!=0) {
        QFile::remove(fileCompressed);
        return false;
    }

    return true;
}



bool MJlCompress::removeFile(QStringList listFile) {
    bool ret = true;
    // Per ogni file
    for (int i=0; i<listFile.count(); i++) {
        // Lo elimino
        ret = ret && QFile::remove(listFile.at(i));
    }
    return ret;
}

bool MJlCompress::extractFiledeEncrypt(QuaZip* zip, QString fileName, QString fileDest,const QString &passWord)
{

    char*  password;
    QByteArray ba = passWord.toLatin1();
    if(passWord==NULL)
    {
        password=0;
    }
    else
    {

        password=ba.data();
    }
    // zip: oggetto dove aggiungere il file
    // filename: nome del file reale
    // fileincompress: nome del file all'interno del file compresso

    // Controllo l'apertura dello zip
    if (!zip) return false;
    if (zip->getMode()!=QuaZip::mdUnzip) return false;

    // Apro il file compresso
    if (!fileName.isEmpty())
        zip->setCurrentFile(fileName);
    QuaZipFile inFile(zip);
    if(!inFile.open(QIODevice::ReadOnly,password) || inFile.getZipError()!=UNZ_OK) return false;

    // Controllo esistenza cartella file risultato
    QDir curDir;
    if (fileDest.endsWith(QLatin1String("/"))) {
        if (!curDir.mkpath(fileDest)) {
            return false;
        }
    } else {
        if (!curDir.mkpath(QFileInfo(fileDest).absolutePath())) {
            return false;
        }
    }

    QuaZipFileInfo64 info;
    if (!zip->getCurrentFileInfo(&info))
        return false;

    QFile::Permissions srcPerm = info.getPermissions();
    if (fileDest.endsWith(QLatin1String("/")) && QFileInfo(fileDest).isDir()) {
        if (srcPerm != 0) {
            QFile(fileDest).setPermissions(srcPerm);
        }
        return true;
    }

    // Apro il file risultato
    QFile outFile;
    outFile.setFileName(fileDest);
    if(!outFile.open(QIODevice::WriteOnly)) return false;

    // Copio i dati
    if (!copyData(inFile, outFile) || inFile.getZipError()!=UNZ_OK) {
        outFile.close();
        removeFile(QStringList(fileDest));
        return false;
    }
    outFile.close();

    // Chiudo i file
    inFile.close();
    if (inFile.getZipError()!=UNZ_OK) {
        removeFile(QStringList(fileDest));
        return false;
    }

    if (srcPerm != 0) {
        outFile.setPermissions(srcPerm);
    }
    return true;
}

QStringList MJlCompress::extractDirdeEncrypt(QString fileCompressed, QString dir,const QString &passWord) {
    // Apro lo zip
    QuaZip zip(fileCompressed);
    return extractDirdeEncrypt(zip, dir,passWord);
}

QStringList MJlCompress::extractDirdeEncrypt(QuaZip &zip, const QString &dir,const QString &passWord)
{
    if(!zip.open(QuaZip::mdUnzip)) {
        return QStringList();
    }
    QString cleanDir = QDir::cleanPath(dir);
    QDir directory(cleanDir);
    QString absCleanDir = directory.absolutePath();
    QStringList extracted;
    if (!zip.goToFirstFile()) {
        return QStringList();
    }
    do {
        QString name = zip.getCurrentFileName();
        QString absFilePath = directory.absoluteFilePath(name);
        QString absCleanPath = QDir::cleanPath(absFilePath);
        if (!absCleanPath.startsWith(absCleanDir + QLatin1String("/")))
            continue;
        if (!extractFiledeEncrypt(&zip, QLatin1String(""), absFilePath,passWord)) {
            removeFile(extracted);
            return QStringList();
        }
        extracted.append(absFilePath);
    } while (zip.goToNextFile());

    // Chiudo il file zip
    zip.close();
    if(zip.getZipError()!=0) {
        removeFile(extracted);
        return QStringList();
    }

    return extracted;
}


/************加密压缩文件************/
bool MJlCompress::compressDirOrFileEncrypt(const QString &zipDest,const QString &zipSrc,const QString &passWord )
{
    QFileInfo fileinfo(zipSrc);
    if(fileinfo.isFile())
    {
        if(false==MJlCompress::compressFileEncrypt(zipDest,zipSrc,passWord))
        {    qDebug()<<"compress failed";
               return false;
        }
        else
            qDebug()<<"compress successed";
    }
    else if (fileinfo.isDir())
    {
        if(false==MJlCompress::compressDirEncrypt(zipDest,zipSrc,passWord))
        {    qDebug()<<"compress failed";
               return false;
        }
           else
               qDebug()<<"compress successed";
    }
    return true;
}


/************压缩文件************/
bool MJlCompress::compressDirOrFile(const QString &zipDest,const QString &zipSrc)
{
    QFileInfo fileinfo(zipSrc);
    if(fileinfo.isFile())
    {
        if(false==MJlCompress::compressFile(zipDest,zipSrc))
        {    qDebug()<<"compress failed";
               return false;
        }
        else
            qDebug()<<"compress successed";
    }
    else if (fileinfo.isDir())
    {
        if(false==MJlCompress::compressDir(zipDest,zipSrc))
        {    qDebug()<<"compress failed";
               return false;
        }
           else
               qDebug()<<"compress successed";
    }
    return true;
}

/************加密压缩文件************/
bool MJlCompress::encryptZip(const QString &zipDest,const QString &zipSrc , const QString &passWord)
{
    char*  password;
    QByteArray ba = passWord.toLatin1();
    if(passWord==NULL)
    {
        password=0;
    }
    else
    {

        password=ba.data();
    }

    MJlCompress::compressDirOrFile(zipDest, zipSrc);
    QFileInfo fileinfo(zipDest);

    QString zipDestMiddle = zipDest + fileinfo.suffix();//中间文件，添加同类型的后缀
    QuaZip zip(zipDestMiddle);
    zip.open(QuaZip::mdCreate);
    QuaZipFile zipFile(&zip);
    bool ret = zipFile.open(QIODevice::WriteOnly,
                            fileinfo.fileName(),   // QuaZipNewInfo结构体引用
                            password, // 密码
                            0,        // CRC值（默认值是0）
                            8);       // 写
    if(ret)
    {
        QFile file(zipDest);
        bool isopen=file.open(QIODevice::ReadOnly);
        if (isopen)
        {
            // 开始写入文件的数据了
            zipFile.write(file.readAll());
        }
        file.close();
        QFile::remove(zipDest);
        zipFile.close();
    }
    else
    {
        return false;
    }
    zip.close();
    QFile::rename (zipDestMiddle, zipDest);
    qDebug()<<"EncryptZip successed";
    return  true;
}


/************解密压缩文件************/
bool MJlCompress::deEncryptZip(const QString &zipDest,const QString &zipSrc , const QString &passWord)
{
    char*  password;
    QByteArray ba = passWord.toLatin1();
    if(passWord==NULL)
    {
        password=0;
    }
    else
    {
        password=ba.data();
    }

    QDir dir;
    dir.mkpath(zipSrc);

    QFileInfo fileinfo(zipDest);
    QuaZip zip(zipDest);
    zip.open(QuaZip::mdUnzip);
    QuaZipFile zipFile(&zip);
    zip.goToFirstFile();
    bool ret = zipFile.open(QIODevice::ReadOnly,password);
    if(ret)
    {
        QFile file(zipSrc+"/middle"+fileinfo.suffix());
        bool isopen=file.open(QIODevice::WriteOnly);
        if (isopen)
        {
            //开始写入文件的数据了
            file.write(zipFile.readAll());
            file.close();
        }
        zipFile.close();
    }
    else
    {
        return false;
    }
    zip.close();

    JlCompress::extractDir(zipSrc+"/middle"+fileinfo.suffix(),zipSrc);
    QFile::remove(zipSrc+"/middle"+fileinfo.suffix());
    qDebug()<<"deEncryptZip successed";
    return true;
}
