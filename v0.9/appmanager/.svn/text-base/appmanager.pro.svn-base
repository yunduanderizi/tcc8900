include(../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = appmanager
DEPENDPATH += 

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include \
               $$INNOVBOX_WM_INSTALL_DIR/include

LIBS += -L$$PACKAGE_DBUS_DIR/lib -lQtDBus -ldbus-1
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += procStatusMonitorInterface.h
HEADERS += $$INNOVBOX_WM_INSTALL_DIR/include/dbusClientProxy.h

SOURCES += procStatusMonitorInterface.cpp

QMAKE_POST_LINK =make install

install_include.files = procStatusMonitorInterface.h 
install_include.path  = $$INNOVBOX_APPMANAGER_INSTALL_DIR/include

target.path  = $$INNOVBOX_APPMANAGER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
