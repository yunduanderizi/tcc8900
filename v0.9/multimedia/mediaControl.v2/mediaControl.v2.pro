include(../../product.conf)

system(chmod -x *.h)

TARGET = mediaControlv2

QT       += core
TEMPLATE = lib

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

#---------------------------------------------------------------
SOURCES += myprocess.cpp \
           ibMediaControl.cpp \
           ibMediaListControl.cpp \
           ibMovieControl.cpp \
           ibMusicControl.cpp \
           ibStoryControl.cpp \
           ibNetMusicControl.cpp \
           FileListUtil.cpp 

HEADERS  +=  myprocess.h \
             trace.h \
             tccfb_ioctl.h \
             ibMediaControl.h \
             ibMediaListControl.h \
             ibMovieControl.h \
             ibMediaDefine.h \
             ibMusicControl.h \
             ibStoryControl.h \
             ibNetMusicControl.h \
             FileListUtil.h

QMAKE_POST_LINK = make install

install_include.files = ibMediaDefine.h \
                        ibMediaListControl.h \
                        ibMovieControl.h \
                        ibMusicControl.h \
                        ibStoryControl.h \
                        ibNetMusicControl.h

install_include.path  = $$INNOVBOX_MEDIACONTROLV2_INSTALL_DIR/include

target.path  = $$INNOVBOX_MEDIACONTROLV2_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
