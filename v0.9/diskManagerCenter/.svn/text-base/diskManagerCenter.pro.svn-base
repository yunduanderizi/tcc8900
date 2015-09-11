include(../product.conf)

system(chmod -x *.h)

QT += core
TEMPLATE = lib 
TARGET = diskManager 

DEPENDPATH += . $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += . $$PACKAGE_DBUS_DIR/include $$INNOVBOX_WM_INSTALL_DIR/include
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += diskManager.h \
           diskManagerClient.h \
           diskManagerMessage.h \
           diskManagerServer.h

SOURCES += diskManagerServer.cpp \
           diskManagerClient.cpp \
           diskManagerMessage.cpp \
           diskManager.cpp 

QMAKE_POST_LINK =make install


install_include.files = diskManagerMessage.h diskManagerServer.h diskManagerClient.h diskManager.h
install_include.path  =$$INNOVBOX_DISKMANAGERCENTER_DIR/include

target.path  = $$INNOVBOX_DISKMANAGERCENTER_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
