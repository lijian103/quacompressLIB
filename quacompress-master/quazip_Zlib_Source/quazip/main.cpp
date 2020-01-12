#include <QCoreApplication>
#include "mjlcompress.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //先压缩成zip，再加密（压缩包里面是个zip文件）,无密码passWorlD=NULL或者使用默认参数
//    MJlCompress::encryptZip("./my.zip","./test" , NULL);
//    MJlCompress::deEncryptZip("./my.zip","./test2" , NULL);
//    MJlCompress::encryptZip("./my1.zip","./test.txt" , NULL);
//    MJlCompress::deEncryptZip("./my1.zip","./test3" , NULL);
//    MJlCompress::compressDirOrFile("./my2.zip","./test" );

    //直接压缩加密，递归遍历每一个文件。无密码passWorlD=NULL使用默认参数
    MJlCompress::compressDirOrFileEncrypt("./my3.zip","./test" ,"NULL");
    MJlCompress::compressDirOrFileEncrypt("./my4.zip","./test.txt" ,NULL);
//    MJlCompress::compressDirOrFileEncrypt("./my3.zip","./test" );
//    MJlCompress::compressDirOrFileEncrypt("./my4.zip","./test.txt" );//默认参数passWorlD=NULL
    MJlCompress::extractDirdeEncrypt("./my3.zip", "./my3","NULL");
    MJlCompress::extractDirdeEncrypt("./my4.zip", "./my4",NULL);


//      MJlCompress::compressFileEncrypt ("./my2.zip","./test1.txt","12345" );
//      MJlCompress::compressFile("./my3.zip","./test1.txt" );
//      MJlCompress::compressDirEncrypt("./my2.zip","./test" ,"NULL");
//      MJlCompress::compressDirEncrypt("./my3.zip","./test" ,NULL);
//      MJlCompress::extractDirdeEncrypt("./my2.zip", "./my2","NULL");
//      MJlCompress::extractDirdeEncrypt("./my3.zip", "./my3",NULL);
//      MJlCompress::extractDirdeEncrypt("./my2.zip", "./my2","12");
//    MJlCompress::extractDirdeEncrypt("./my3.zip", "./my3","");
    return 0;
}
