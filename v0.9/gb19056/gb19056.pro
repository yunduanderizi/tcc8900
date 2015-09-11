include(../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = gb19056

INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables

HEADERS += GB19056_2012.h GB19056Base.h GB19056Proxy.h GB19056CallBackInterface.h
                
SOURCES += GB19056_2012.cpp
SOURCES += GB19056Base.cpp

QMAKE_POST_LINK =make install

install_include.files = GB19056_2012.h GB19056Base.h GB19056Proxy.h GB19056CallBackInterface.h
install_include.path  = $$INNOVBOX_GB19056_INSTALL_DIR/include

target.path  = $$INNOVBOX_GB19056_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
