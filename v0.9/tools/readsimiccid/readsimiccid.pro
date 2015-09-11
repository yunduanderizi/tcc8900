include(../../product.conf)

system(chmod -x ../../common/message/*.h)

TEMPLATE = app
TARGET = readsimiccid


INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include


LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables


# INCLUDEPATH += .
INCLUDEPATH += $$PACKAGE_EXPAT_DIR/include
PACKAGE_EXPAT_DIR=$$PACKAGE_DIR/expat

LIBS += -L$$PACKAGE_EXPAT_DIR/lib -lexpat

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}


# Input
HEADERS += 
SOURCES += readsimiccid.cpp
			
QMAKE_POST_LINK =make install


target.path  = $$INNOVBOX_INSTALL_DIR/tools/readsimiccid

INSTALLS += target 

QMAKE_DISTCLEAN += $$target.path/* 

