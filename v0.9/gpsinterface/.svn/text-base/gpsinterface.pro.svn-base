include(../product.conf)

system(chmod -x *.h)

CONFIG -= qt

TEMPLATE = lib
TARGET = 
DEPENDPATH += .

INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables

# Input
HEADERS += gpsinterface.h
SOURCES += readtuid.cpp
SOURCES += readfence.cpp

QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_GPSINTERFACE_INSTALL_DIR/lib

install_include.files = gpsinterface.h

install_include.path  = $$INNOVBOX_GPSINTERFACE_INSTALL_DIR/include

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$target.path/*


