include(../product.conf)

system(chmod -x *.h)


TEMPLATE = app
TARGET = JT808Manager

QT +=  core xml

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_GB19056_INSTALL_DIR/include
INCLUDEPATH += $$PACKAGE_ZLIB_DIR/include

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_ZLIB_DIR/lib -lz
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L/home/wy/source/tcc890x_common/trunk/packages/target/tslib1.0/lib 
LIBS += -L$$INNOVBOX_GB19056_INSTALL_DIR/lib -lgb19056

# Input
HEADERS += ConfigManager.h ConnectionManager.h \
           JT808MessageHandler.h JT808MessageHelper.h \
           JT808DbusMessage.h JT808DbusMessageHandler.h \
           ../common/message/NaviDbusMessage.h\
	   ../common/message/phoneMessage.h\
    JT808GnssManager.h 

SOURCES += ConfigManager.cpp ConnectionManager.cpp \
           JT808MessageHandler.cpp JT808MessageHelper.cpp \
           JT808Manager.cpp JT808DbusMessageHandler.cpp \
           ../common/message/NaviDbusMessage.cpp \
	   ../common/message/phoneMessage.cpp\
    JT808GnssManager.cpp

QMAKE_POST_LINK =make install

install_include.files = JT808DbusMessage.h

install_include.path  = $$INNOVBOX_JT808_INSTALL_DIR/include

target.path  = $$INNOVBOX_JT808_INSTALL_DIR

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/* $$target.path/*

