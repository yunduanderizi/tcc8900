include(../product.conf)

system(chmod -x *.h)

QT  += xml 

TEMPLATE = lib 
TARGET = 

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1


!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += dbusManager.h
				
SOURCES += dbusManager.cpp


	

QMAKE_POST_LINK =make install


install_include.files = dbusManager.h

install_include.path = $$INNOVBOX_DBUSMANAGER_INSTALL_DIR/include

target.path  = $$INNOVBOX_DBUSMANAGER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*

