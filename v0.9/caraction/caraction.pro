include(../product.conf)

system(chmod -x *.h)

CONFIG -= qt

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ./common
LIBS +=-lpthread

# Input
HEADERS += DevCtrl.h ./common/i2ctl.h ./common/NetApi.h \
           ./common/tinystr.h    \
           ./common/tinyxml.h
SOURCES += DevCtrl.cpp caraction.cpp ./common/i2ctl.cpp ./common/NetApi.cpp \
           ./common/tinystr.cpp    \
           ./common/tinyxmlparser.cpp    \
           ./common/tinyxmlerror.cpp    \
           ./common/tinyxml.cpp

QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_CARACTION_INSTALL_DIR

INSTALLS += target

QMAKE_DISTCLEAN += $$target.path/*


