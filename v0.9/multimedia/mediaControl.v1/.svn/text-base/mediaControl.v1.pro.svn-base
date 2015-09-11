include(../../product.conf)

system(chmod -x *.h)

TARGET = mediaControlv1

QT       += core
TEMPLATE = lib

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

SOURCES += myprocess.cpp \
           ibmediacontrolc.cpp

HEADERS  +=  myprocess.h \
             trace.h \
             tccfb_ioctl.h \
             ibmediacontrolc.h

QMAKE_POST_LINK = make install
install_include.files = ibmediacontrolc.h\
                        myprocess.h

install_include.path  =$$INNOVBOX_MEDIACONTROLV1_INSTALL_DIR/include

target.path  = $$INNOVBOX_MEDIACONTROLV1_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
