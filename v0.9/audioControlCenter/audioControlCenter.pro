include(../product.conf)

system(chmod -x *.h)

QT       += core gui network

TEMPLATE = lib
#CONFIG += staticlib
CONFIG += debug
TARGET = audioControl
DEPENDPATH += . $$PACKAGE_DBUS_DIR/include $$PACKAGE_ALSA_DIR/include
INCLUDEPATH += ./ $$PACKAGE_DBUS_DIR/include $$PACKAGE_ALSA_DIR/include $$INNOVBOX_WM_INSTALL_DIR/include

contains(DEFINES,WITH_NEW_AUDIO_CONTROL_SYSTEM){
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
}else{
}


LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus  -lQtXml  -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm -L$$PACKAGE_ALSA_DIR/lib -lasound
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += audio_control.h audio_control_message.h \
           audio_control_client.h audio_control_server.h audio_main_volume.h 

SOURCES += audio_control.cpp audio_control_server.cpp \
           audio_control_client.cpp audio_control_message.cpp audio_main_volume.cpp

QMAKE_POST_LINK =make install

install_include.files = audio_control_client.h  audio_control.h audio_control_server.h audio_main_volume.h
install_include.path  = $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/include

target.path  = $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*

