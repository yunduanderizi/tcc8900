include(../product.conf)

system(chmod -x *.h)

TEMPLATE = app
TARGET = recordvoice

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$PACKAGE_ALSA_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$PACKAGE_AMR_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include

INCLUDEPATH += $$INNOVBOX_SRC_DIR/jt808

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$PACKAGE_ALSA_DIR/lib -lasound
LIBS += -L$$PACKAGE_AMR_DIR/lib -lopencore-amrnb
# Input
HEADERS += record.h \
	   clientMain.h \
	   wav_parser.h 
                
SOURCES += record.cpp \
	   clientMain.cpp \
	   wav_parser.cpp 

QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_AUDIORECORD_INSTALL_DIR

INSTALLS += target

QMAKE_DISTCLEAN += $$target.path/*

