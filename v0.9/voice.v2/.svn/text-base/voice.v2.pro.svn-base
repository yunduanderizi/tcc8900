include(../product.conf)

system(chmod -x *.h)

TEMPLATE = app
TARGET = voicev2

QT += xml

DEPENDPATH += . $$PACKAGE_DBUS_DIR/include
DEPENDPATH +=  $$PACKAGE_ALSA_DIR/include/alsa
DEPENDPATH +=  $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/include

INCLUDEPATH += $$PACKAGE_AITTS_DIR/Inc
INCLUDEPATH += $$PACKAGE_AITALK_DIR/Inc interface
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_ALSA_DIR/include
INCLUDEPATH += $$PACKAGE_VOICERECOGNIZE_DIR/include
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += . ivTts pcmPlayer
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROL_INSTALL_DIR/include

LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L$$PACKAGE_AITALK_DIR/Lib -lAitalk #-static
LIBS += -L$$PACKAGE_AITTS_DIR/Lib -lAiSound5
LIBS += -L$$PACKAGE_ALSA_DIR/lib -lasound
!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}
LIBS += -L$$PACKAGE_VOICERECOGNIZE_DIR/lib -lmsc
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$INNOVBOX_AUDIOCONTROL_INSTALL_DIR/lib -laudioControlLib

SOURCES += AiTalk.cpp \
           AiPlay.cpp \
           voice_main.cpp \
           myDbus.cpp \
           AiBsTalk.cpp \
           Record.cpp \
           message/voiceMessage.cpp \
           KeyWordsMgr.cpp \
           ivTts/ivTts.cpp \
           pcmPlayer/pcmPlayer.cpp \
           pcmPlayer/pcmPlayerManager.cpp

HEADERS += AiTalk.h \
           AiPlay.h \
           myDbus.h \
           AiBsTalk.h \
           Record.h \
           message/voiceMessage.h \
           KeyWordsMgr.h \
           ivTts/ivTts.h \
           pcmPlayer/pcmPlayer.h \
           pcmPlayer/pcmPlayerManager.h



RESOURCES +=

QMAKE_POST_LINK =make install

target.path= $$INNOVBOX_VOICEV2_INSTALL_DIR/bin
INSTALLS += target

QMAKE_DISTCLEAN += $$target.path/*
