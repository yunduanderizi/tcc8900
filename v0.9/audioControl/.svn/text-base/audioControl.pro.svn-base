include(../product.conf)

system(chmod -x *.h */*.h)

TEMPLATE = app
TARGET = audioControl

QT +=  core xml
 
DEPENDPATH +=  $$PACKAGE_ALSA_DIR/include/alsa
DEPENDPATH += . $$PACKAGE_DBUS_DIR/include

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_ALSA_DIR/include
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROL_SRC_DIR
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROL_SRC_DIR/message

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L$$PACKAGE_ALSA_DIR/lib -lasound
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

SOURCES +=  audioControlMain.cpp \
            myDbus.cpp \
            audioControl.cpp \
            audioDevice.cpp \
            parseConfig.cpp \
            message/audioControlMessage.cpp

HEADERS +=  myDbus.h \
            audioControl.h \
            audioDevice.h \
            audioData.h \
            parseConfig.h \
            audioControlErrCode.h \
            message/audioControlMessage.h

RESOURCES +=

QMAKE_POST_LINK =make install

install_include.files = audioData.h audioControlErrCode.h

install_include.path = $$INNOVBOX_AUDIOCONTROL_DIR/include

target.path = $$INNOVBOX_AUDIOCONTROL_DIR/bin

INSTALLS += install_include target 

QMAKE_DISTCLEAN += $$install_include.path/* $$target.path/*
