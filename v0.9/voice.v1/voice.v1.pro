include(../product.conf)

system(chmod -x *.h)

TEMPLATE = app
TARGET = voicev1

QT += xml

DEPENDPATH += . $$PACKAGE_DBUS_DIR/include
DEPENDPATH += ./voiceDbus
DEPENDPATH +=  $$PACKAGE_ALSA_DIR/include/alsa
DEPENDPATH +=  $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/include

INCLUDEPATH += . ivTTS $$PACKAGE_AITTS_DIR/Inc aiTalk \
               $$PACKAGE_AITALK_DIR/Inc interface

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += ./voiceDbus
INCLUDEPATH += $$PACKAGE_ALSA_DIR/include
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/include $$INNOVBOX_WM_INSTALL_DIR/include

LIBS += -L$$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/lib -laudioControl
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L$$PACKAGE_AITALK_DIR/Lib -lAitalk #-static 
LIBS += -L$$PACKAGE_AITTS_DIR/Lib -lAiSound5 
LIBS += -L$$PACKAGE_ALSA_DIR/lib -lasound
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts

HEADERS += interface/voiceInterface.h \
           error_types.h \
           parseConfig.h \
           asoundlib.h \
           ivTTS/iv_TTS.h \
           ivTTS/AisoundTTS.h \
           voice_setting.h \
           aiTalk/aiTalk.h \
           aiTalk/aiTalk_setting.h \
           VoiceDbusApp.h \
           VoiceDbusClientApp.h \
           dbusServer.h \
           dbusMessage.h \
           dbusApplication.h \
           dbusClient.h

SOURCES +=  parseConfig.cpp \
            ivTTS/iv_TTS.cpp \
            ivTTS/AisoundTTS.cpp \
            aiTalk/aiTalk.cpp \
            aiTalk/aiTalk_setting.cpp \
            voice_main.cpp

RESOURCES +=

QMAKE_POST_LINK =make install

target.path= $$INNOVBOX_VOICEV1_INSTALL_DIR
INSTALLS += target 

QMAKE_DISTCLEAN += $$target.path/*
