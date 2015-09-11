include(../product.conf)

system(chmod -x *.h ../common/message/*.h)

TEMPLATE = app
TARGET = remotecontrol



INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_SRC_DIR/jt808
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$PACKAGE_CURL_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_GB19056_INSTALL_DIR/include

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_CURL_DIR/lib -lcurl
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$INNOVBOX_GB19056_INSTALL_DIR/lib -lgb19056
LIBS += -L$$PACKAGE_CARES_DIR/lib -lcares


# INCLUDEPATH += .
INCLUDEPATH += $$PACKAGE_EXPAT_DIR/include
PACKAGE_EXPAT_DIR=$$PACKAGE_DIR/expat

LIBS += -L$$PACKAGE_EXPAT_DIR/lib -lexpat

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}


# Input
HEADERS += remotecontrol.h \
			xml_analy.h			
SOURCES += remotecontrol.cpp \
			xml_analy.cpp
			
QMAKE_POST_LINK =make install



target.path  = $$INNOVBOX_REMOTECONTROL_INSTALL_DIR

INSTALLS += target 

QMAKE_DISTCLEAN += $$target.path/* 

