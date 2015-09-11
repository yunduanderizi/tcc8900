include(../product.conf)

system(chmod -x *.h ../common/message/*.h)

TEMPLATE = app
TARGET = sleep_mode_start


INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_SRC_DIR/jt808
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_GB19056_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_DEVICEMANAGER_SRC_DIR

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$INNOVBOX_GB19056_INSTALL_DIR/lib -lgb19056

# Input
HEADERS += $$INNOVBOX_DEVICEMANAGER_SRC_DIR/mcuprotocol.h \
		deviceManager_sleep_mode.h
HEADERS += $$INNOVBOX_COMMON_SRC_DIR/message/NaviDbusMessage.h
						
SOURCES += deviceManager_sleep_mode.cpp 
SOURCES += $$INNOVBOX_DEVICEMANAGER_SRC_DIR/mcuprotocol.cpp
SOURCES += $$INNOVBOX_COMMON_SRC_DIR/message/NaviDbusMessage.cpp

QMAKE_POST_LINK =make install

#install_include.files = $$INNOVBOX_DEVICEMANAGER_SRC_DIR/i2ccontrol.h

install_include.path  = $$INNOVBOX_DEVICEMANAGER_SLEEP_MODE_INSTALL_DIR/include

target.path  = $$INNOVBOX_DEVICEMANAGER_SLEEP_MODE_INSTALL_DIR

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$target.path/* $$install_include.path/*

