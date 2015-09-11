include(../../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
#CONFIG += staticlib
#DEFINES += ADD_WIFI_TEST
TARGET = ./PhoneManager

QT +=xml

INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$PACKAGE_CURL_DIR/include
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include

INCLUDEPATH += $$INNOVBOX_BSMANAGER_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
#INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR

INCLUDEPATH +=$$PACKAGE_ZTEVIDEOPHONE_DIR/include \
              $$PACKAGE_ALSA_DIR/include/sys \
              $$PACKAGE_ALSA_DIR/include/alsa \
              $$PACKAGE_ALSA_DIR/include

LIBS += -L$$PACKAGE_ZTEVIDEOPHONE_DIR/lib -lopencore-amrnb -lmediadevapi -lh32x -lcoreapi -lcommapi \
        -L$$PACKAGE_ALSA_DIR/lib -lasound \
        -L$$INNOVBOX_CAMERA_INSTALL_DIR/lib -lcamapp
LIBS += -L$$INNOVBOX_BSMANAGER_INSTALL_DIR/lib -lbsManager
LIBS += -L$$PACKAGE_CURL_DIR/lib -lcurl
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus 
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_CARES_DIR/lib -lcares
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm

contains(DEFINES,WITH_NEW_AUDIO_CONTROL_SYSTEM){
}else{
INCLUDEPATH += $$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/include
LIBS += -L$$INNOVBOX_AUDIOCONTROLCENTER_INSTALL_DIR/lib -laudioControl
}
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables

INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility



!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += MobileInterFace.h \
           videoPhoneMonitor.h \
           phoneMonitorVoiceReportBase.h \
           voicePhoneInitThread.h \
           voicePhoneWorkThread.h \
           phoneConnection.h \
           sms.h \
           MobileInterFace1.h \
           audioControl.h \
           play_sign_sound.h \
           sendDTMF.h \
           videoPhoneAudioPlayThread.h \
           $$PACKAGE_ZTEVIDEOPHONE_DIR/include/interf_dec.h \
           $$PACKAGE_ZTEVIDEOPHONE_DIR/include/interf_enc.h \
           $$PACKAGE_ZTEVIDEOPHONE_DIR/include/tccfb_ioctl.h \
           md5.h \
           $$PACKAGE_ZTEVIDEOPHONE_DIR/include/vtlib.h \
    smscmdprotocol.h

contains(DEFINES,MODEM_5310){
SOURCES += MobileInterFace-5310.cpp \
           voicePhoneWorkThread-5310.cpp
}else{
SOURCES += MobileInterFace.cpp \
           voicePhoneWorkThread.cpp
}

SOURCES += videoPhoneMonitor.cpp \
           phoneMonitorVoiceReportBase.cpp \
           phoneConnection.cpp \
           voicePhoneInitThread.cpp \
           sms.cpp \
           MobileInterFace1.cpp \
           audioControl.cpp \
           play_sign_sound.cpp \
           videoPhoneAudioPlayThread.cpp \
           md5.cpp \
           sendDTMF.cpp \
    smscmdprotocol.cpp
           
QMAKE_POST_LINK =make install

install_include.files = MobileInterFace.h phoneMonitorVoiceReportBase.h MobileInterFace1.h smscmdprotocol.h
install_include.path  =$$INNOVBOX_MODEM_INSTALL_DIR/include

target.path  = $$INNOVBOX_MODEM_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*

