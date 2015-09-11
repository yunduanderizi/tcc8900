include(../product.conf)

system(chmod -x *.h)

QT  += xml 

TEMPLATE = lib 
TARGET = 

INCLUDEPATH += message
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$PACKAGE_CURL_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR

LIBS += -L$$PACKAGE_CURL_DIR/lib -lcurl
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_CARES_DIR/lib -lcares


!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += dataManager.h \
           dataManagerInterface.h \
           dataManagerServer.h \
           dataManagerClient.h \
           pluginManager.h \
           message/dataManagerMessage.h
                
SOURCES += dataManager.cpp \
           dataManagerInterface.cpp \
           dataManagerServer.cpp \
           dataManagerClient.cpp \
           pluginManager.cpp \
           message/dataManagerMessage.cpp

QMAKE_POST_LINK =make install

install_include.files = dataManagerInterface.h \
                        dataManagerServer.h \
                        dataManagerClient.h \
                        dataManagerDefine.h \
                        message/dataManagerMessage.h

install_include.path  = $$INNOVBOX_DATAMANAGER_INSTALL_DIR/include

target.path  = $$INNOVBOX_DATAMANAGER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
