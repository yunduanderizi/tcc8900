include(../product.conf)

system(chmod -x *.h)

QT       += core 

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

TARGET = recoveryDefaultData
TEMPLATE = app

SOURCES += main.cpp


QMAKE_POST_LINK =make install

depend_sh.files = file_list* \
                  *.sh*

depend_sh.path = $$INNOVBOX_RECOVERDEFAULTDATA_INSTALL_DIR

target.path= $$INNOVBOX_RECOVERDEFAULTDATA_INSTALL_DIR

INSTALLS += target depend_sh

QMAKE_DISTCLEAN += $$target.path/*
