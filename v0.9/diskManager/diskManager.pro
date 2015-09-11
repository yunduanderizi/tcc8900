include(../product.conf)

system(chmod -x *.h)

TEMPLATE = app
TARGET = diskmanager
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include \
               $$INNOVBOX_DISKMANAGERCENTER_DIR/include

LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml \
        -L$$INNOVBOX_DISKMANAGERCENTER_DIR/lib -ldiskManager
        
!CONFIG(PC){
    LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}
# Input
SOURCES += diskManagerServerMain.cpp

QMAKE_POST_LINK =make install
target.path  = $$INNOVBOX_DISKMANAGER_DIR
INSTALLS += target
QMAKE_DISTCLEAN += $$target.path/*


