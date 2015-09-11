include(../product.conf)

system(chmod -x *.h)

QT       += core gui network

TEMPLATE = lib 
TARGET = 

DEPENDPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus  -lQtXml 

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += windowManager.h WmMessage.h \
           windowManagerApp.h dbusClientProxy.h \
           debugMonitor.h debugMonitorMessage.h 

SOURCES += windowManager.cpp WmMessage.cpp \
        windowManagerApp.cpp dbusClientProxy.cpp \
        debugMonitor.cpp debugMonitorMessage.cpp \
        i2ccontrol.cpp 

QMAKE_POST_LINK = make install

install_include.files = dbusClientProxy.h  windowManagerApp.h  windowManager.h debugMonitor.h \
                        debugMonitorMessage.h i2ccontrol.h 
install_include.path  = $$INNOVBOX_WM_INSTALL_DIR/include

target.path  = $$INNOVBOX_WM_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
