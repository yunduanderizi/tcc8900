include(../product.conf)

system(chmod -x *.h ./*.h)
DEFINES +=  TCC_89XX_INCLUDE
QT +=xml

TEMPLATE = lib
TARGET = writecard
DEPENDPATH +=  

INCLUDEPATH += . 
INCLUDEPATH += $$PACKAGE_EXPAT_DIR/include
PACKAGE_EXPAT_DIR=$$PACKAGE_DIR/expat

LIBS += -L$$PACKAGE_EXPAT_DIR/lib -lexpat

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += writecard.h
         
SOURCES += writecard.cpp 

QMAKE_POST_LINK =make install

install_include.files = writecard.h
install_include.path  =$$INNOVBOX_IC_CARD_INSTALL_DIR/include

target.path  = $$INNOVBOX_IC_CARD_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
