include(../../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = audioControlLib

QT += xml

INCLUDEPATH += $$INNOVBOX_AUDIOCONTROL_SRC_DIR
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROL_SRC_DIR/message

INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include

LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

SOURCES += audioControlLib.cpp

HEADERS += audioControlLib.h

RESOURCES +=

QMAKE_POST_LINK =make install

install_include.files = audioControlLib.h
install_include.path = $$INNOVBOX_AUDIOCONTROL_DIR/include

target.path = $$INNOVBOX_AUDIOCONTROL_DIR/lib

INSTALLS += install_include target 

QMAKE_DISTCLEAN += $$target.path/*
