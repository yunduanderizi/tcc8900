include(../../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = voice

CONFIG += staticlib

QT += xml

DEPENDPATH += . $$PACKAGE_DBUS_DIR/include

INCLUDEPATH += $$INNOVBOX_VOICEV2_SRC_DIR
INCLUDEPATH += $$INNOVBOX_VOICEV2_SRC_DIR/message

INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include

LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

SOURCES += VoiceFunControl.cpp \
           $$INNOVBOX_VOICEV2_SRC_DIR/message/voiceMessage.cpp

HEADERS += VoiceFunControl.h
HEADERS += $$INNOVBOX_VOICEV2_SRC_DIR/voiceDefine.h

RESOURCES +=

QMAKE_POST_LINK =make install

install_include.files = VoiceFunControl.h $$INNOVBOX_VOICEV2_SRC_DIR/voiceDefine.h

install_include.path = $$INNOVBOX_VOICEV2_INSTALL_DIR/include

target.path = $$INNOVBOX_VOICEV2_INSTALL_DIR/lib

INSTALLS += install_include target 

QMAKE_DISTCLEAN += $$target.path/*
