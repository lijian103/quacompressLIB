QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

#打开下面可以编译库
###########produce lib#############
TARGET = quacompress
TEMPLATE = lib
CONFIG += staticlib
CONFIG(debug, debug|release) {
     mac: TARGET = $$join(TARGET,,,_debug)
     win32: TARGET = $$join(TARGET,,,d)
}
win32 {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    INSTALLS += headers target
    CONFIG(staticlib){
        target.path=$$PREFIX/lib
        QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/lib/
    } else {
        target.path=$$PREFIX/bin
        QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/bin/
    }

    ## odd, this path seems to be relative to the
    ## target.path, so if we install the .dll into
    ## the 'bin' dir, the .pc will go there as well,
    ## unless have hack the needed path...
    ## TODO any nicer solution?
    QMAKE_PKGCONFIG_DESTDIR = ../lib/pkgconfig
    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX
#    include(./3rdparty/zlib.pri)

    win32:CONFIG(debug, debug|release): LIBS += -LD:/NavigationDependencies/lib/Debug/zlib/ -lzlibstaticd
    else:win32:CONFIG(release, debug|release): LIBS += -LD:/NavigationDependencies/lib/Release/zlib/ -lzlibstatic
    INCLUDEPATH += D:/NavigationDependencies/include/zlib-1.2.11
    DEPENDPATH += D:/NavigationDependencies/include/zlib-1.2.11

}
#####################

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QUAZIP_STATIC

win32:CONFIG(debug, debug|release): LIBS += -LD:/NavigationDependencies/lib/Debug/zlib/ -lzlibstaticd
else:win32:CONFIG(release, debug|release): LIBS += -LD:/NavigationDependencies/lib/Release/zlib/ -lzlibstatic
INCLUDEPATH += D:/NavigationDependencies/include/zlib-1.2.11
DEPENDPATH += D:/NavigationDependencies/include/zlib-1.2.11
#include(./quacompress/3rdparty/zlib.pri)



HEADERS += \
        $$PWD/quacompress/inc/minizip_crypt.h \
        $$PWD/quacompress/inc/ioapi.h \
        $$PWD/quacompress/inc/JlCompress.h \
        $$PWD/quacompress/inc/quaadler32.h \
        $$PWD/quacompress/inc/quachecksum32.h \
        $$PWD/quacompress/inc/quacrc32.h \
        $$PWD/quacompress/inc/quagzipfile.h \
        $$PWD/quacompress/inc/quaziodevice.h \
        $$PWD/quacompress/inc/quazipdir.h \
        $$PWD/quacompress/inc/quazipfile.h \
        $$PWD/quacompress/inc/quazipfileinfo.h \
        $$PWD/quacompress/inc/quazip_global.h \
        $$PWD/quacompress/inc/quazip.h \
        $$PWD/quacompress/inc/quazipnewinfo.h \
        $$PWD/quacompress/inc/unzip.h \
        $$PWD/quacompress/inc/zip.h \
        $$PWD/quacompress/inc/mjlcompress.h

INCLUDEPATH += $$PWD/quacompress/inc

SOURCES += $$PWD/quacompress/src/qioapi.cpp \
           $$PWD/quacompress/src/JlCompress.cpp \
           $$PWD/quacompress/src/quaadler32.cpp \
           $$PWD/quacompress/src/quacrc32.cpp \
           $$PWD/quacompress/src/quagzipfile.cpp \
           $$PWD/quacompress/src/quaziodevice.cpp \
           $$PWD/quacompress/src/quazip.cpp \
           $$PWD/quacompress/src/quazipdir.cpp \
           $$PWD/quacompress/src/quazipfile.cpp \
           $$PWD/quacompress/src/quazipfileinfo.cpp \
           $$PWD/quacompress/src/quazipnewinfo.cpp \
           $$PWD/quacompress/src/unzip.c \
           $$PWD/quacompress/src/zip.c \
           $$PWD/quacompress/src/mjlcompress.cpp\
#           $$PWD/quacompress/moc/moc_quagzipfile.cpp\
#           $$PWD/quacompress/moc/moc_quaziodevice.cpp\
#           $$PWD/quacompress/moc/moc_quazipfile.cpp

#SOURCES += \
#        main.cpp \

