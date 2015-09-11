include(../../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = 

INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

DEPENDPATH += . $$INNOVBOX_DATAMANAGER_SRC_DIR
INCLUDEPATH += . $$INNOVBOX_DATAMANAGER_SRC_DIR

# Input
HEADERS += dataBasePlugin.h
SOURCES += dataBasePlugin.cpp

QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_DATAMANAGER_INSTALL_DIR/plugin
INSTALLS += target
QMAKE_DISTCLEAN += $$target.path/*
