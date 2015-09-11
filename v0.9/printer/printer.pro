include(../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = innov_printer

INCLUDEPATH += $$INNOVBOX_DEVICEMANAGER_SRC_DIR/

# Input
HEADERS += innov_printer_interface.h

HEADERS += $$INNOVBOX_DEVICEMANAGER_SRC_DIR/serialbase.h
                
SOURCES += innov_printer_interface.cpp
SOURCES += $$INNOVBOX_DEVICEMANAGER_SRC_DIR/serialbase.cpp

QMAKE_POST_LINK =make install

install_include.files = innov_printer_interface.h 



install_include.path  = $$INNOVBOX_PRINTER_INSTALL_DIR/include

target.path  = $$INNOVBOX_PRINTER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
