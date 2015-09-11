include(../product.conf)

system(chmod -x *.h)

QT  += xml 

TEMPLATE = lib 
TARGET = 

INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR


!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += systemSetting.h

SOURCES += systemSetting.cpp

QMAKE_POST_LINK =make install

install_include.files = systemSetting.h

install_include.path  = $$INNOVBOX_SYSTEMSETTING_INSTALL_DIR/include

target.path  = $$INNOVBOX_SYSTEMSETTING_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*

