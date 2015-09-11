include(../product.conf)

system(chmod -x *.h)


TEMPLATE = lib 
TARGET = 

INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 

LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale

# Input
HEADERS += Utility.h \
           LogUtility.h \
           GeneralTimer.h
                
SOURCES += LogUtility.cpp \
           GeneralTimer.cpp

QMAKE_POST_LINK =make install

install_include.files = Utility.h \
                        GeneralTimer.h \
                        LogUtility.h

install_include.path  = $$INNOVBOX_UTILITY_INSTALL_DIR/include

target.path  = $$INNOVBOX_UTILITY_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
