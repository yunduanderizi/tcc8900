include(../product.conf)

system(chmod -x *.h ../common/message/*.h)

TEMPLATE = app
TARGET = devicemanager


INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_SRC_DIR/jt808
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_GB19056_INSTALL_DIR/include

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$INNOVBOX_GB19056_INSTALL_DIR/lib -lgb19056

# Input
HEADERS +=  mcuprotocol.h \
	    serialbase.h \
	    serialprotocol2012.h \
	    devicemanager.h\
	    workq.h \
	    errors.h
HEADERS += $$INNOVBOX_COMMON_SRC_DIR/message/NaviDbusMessage.h \
           $$INNOVBOX_COMMON_SRC_DIR/message/phoneMessage.h
						
SOURCES += mcuprotocol.cpp \
	   serialbase.cpp \
	   serialprotocol2012.cpp \
	   workq.cpp
t{
	SOURCES += devicemanager-J.cpp 
}
else{
	SOURCES += devicemanager.cpp 
}

SOURCES += $$INNOVBOX_COMMON_SRC_DIR/message/NaviDbusMessage.cpp
SOURCES += $$INNOVBOX_COMMON_SRC_DIR/message/phoneMessage.cpp 
QMAKE_POST_LINK =make install

install_include.files = $$INNOVBOX_COMMON_SRC_DIR/message/DeviceManagerDbusMessage.h

install_include.path  = $$INNOVBOX_DEVICEMANAGER_INSTALL_DIR/include

target.path  = $$INNOVBOX_DEVICEMANAGER_INSTALL_DIR

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$target.path/* $$install_include.path/*

