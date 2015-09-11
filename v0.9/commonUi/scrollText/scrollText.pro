include(../../product.conf)

system(chmod -x *.h)

QT       += core gui

TARGET = scrollText
TEMPLATE = lib

LIBS += -L$$PACKAGE_TSLIB_DIR/lib  -lts


SOURCES += scrollbar.cpp \
           scrollarea.cpp \
           kineticscroll.cpp \
           scrolltext.cpp 
   


HEADERS += scrolltext.h \
           scrollbar.h \
           scrollarea.h \
           kineticscroll.h \
           scrolltextCdata.h
           
