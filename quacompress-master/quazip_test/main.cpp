#include <QCoreApplication>
#include "mjlcompress.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //��ѹ����zip���ټ��ܣ�ѹ���������Ǹ�zip�ļ���,������passWorlD=NULL����ʹ��Ĭ�ϲ���
//    MJlCompress::encryptZip("./my.zip","./test" , NULL);
//    MJlCompress::deEncryptZip("./my.zip","./test2" , NULL);
//    MJlCompress::encryptZip("./my1.zip","./test.txt" , NULL);
//    MJlCompress::deEncryptZip("./my1.zip","./test3" , NULL);
//    MJlCompress::compressDirOrFile("./my2.zip","./test" );

    //ֱ��ѹ�����ܣ��ݹ����ÿһ���ļ���������passWorlD=NULLʹ��Ĭ�ϲ���
    MJlCompress::compressDirOrFileEncrypt("./my3.zip","./test" ,"NULL");
    MJlCompress::compressDirOrFileEncrypt("./my4.zip","./test.txt" ,NULL);
//    MJlCompress::compressDirOrFileEncrypt("./my3.zip","./test" );
//    MJlCompress::compressDirOrFileEncrypt("./my4.zip","./test.txt" );//Ĭ�ϲ���passWorlD=NULL
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
