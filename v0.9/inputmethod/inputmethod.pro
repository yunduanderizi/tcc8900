include(../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = 

CONFIG(HandInput_800){

DEPENDPATH += . ./screen800
INCLUDEPATH += . ./screen800

}else{

DEPENDPATH += . ./screen400
INCLUDEPATH += . ./screen400

}

DEPENDPATH += . $$PACKAGE_FCITXIMENGINE_DIR/include
INCLUDEPATH += . $$PACKAGE_FCITXIMENGINE_DIR/include

LIBS += -L$$PACKAGE_FCITXIMENGINE_DIR/lib -lChshw -lfcitxIMEngine


!CONFIG(PC){
 LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

LIBS +=-L. -linputkernel

MOC_DIR=./tmp
OBJECTS_DIR=./tmp

# Input  

CONFIG(HandInput_800){

CONFIG(VOICE_VERSION_2){
HEADERS += InputInit.h\
           screen800/swtichslectewidget.h\
           screen800/pinyininputmethod.h\
           screen800/fcitxMethod.h\
           screen800/handinput.h\
           screen800/letterwidget.h\
           screen800/digitwidget.h\
           screen800/characterwidget.h\
           screen800/editwidget.h\
           screen800/voiceinput.h
}else{

HEADERS += InputInit.h\
           screen800/swtichslectewidget.h\
           screen800/pinyininputmethod.h\
           screen800/fcitxMethod.h\
           screen800/handinput.h\
           screen800/letterwidget.h\
           screen800/digitwidget.h\
           screen800/characterwidget.h\
           screen800/editwidget.h
}
            
            

#FORMS += myinputpanelform.ui
CONFIG(VOICE_VERSION_2){
SOURCES += InputInit.cpp\
           screen800/swtichslectewidget.cpp\
           screen800/pinyininputmethod.cpp\
           screen800/fcitxMethod.cpp\
           screen800/handinput.cpp\
           screen800/letterwidget.cpp\
           screen800/digitwidget.cpp\
           screen800/characterwidget.cpp\
           screen800/editwidget.cpp\
           screen800/voiceinput.cpp

}else{
SOURCES += InputInit.cpp\
           screen800/swtichslectewidget.cpp\
           screen800/pinyininputmethod.cpp\
           screen800/fcitxMethod.cpp\
           screen800/handinput.cpp\
           screen800/letterwidget.cpp\
           screen800/digitwidget.cpp\
           screen800/characterwidget.cpp\
           screen800/editwidget.cpp
}

RESOURCES += screen800/inputmethod.qrc

}else{
CONFIG(VOICE_VERSION_2){
HEADERS += InputInit.h\
           screen400/swtichslectewidget.h\
           screen400/pinyininputmethod.h\
           screen400/fcitxMethod.h\
           screen400/handinput.h\
           screen400/letterwidget.h\
           screen400/digitwidget.h\
           screen400/characterwidget.h\
           screen400/editwidget.h\
           screen400/voiceinput.h
}else{
HEADERS += InputInit.h\
           screen400/swtichslectewidget.h\
           screen400/pinyininputmethod.h\
           screen400/fcitxMethod.h\
           screen400/handinput.h\
           screen400/letterwidget.h\
           screen400/digitwidget.h\
           screen400/characterwidget.h\
           screen400/editwidget.h

}
CONFIG(VOICE_VERSION_2){
SOURCES += InputInit.cpp\
           screen400/swtichslectewidget.cpp\
           screen400/pinyininputmethod.cpp\
           screen400/fcitxMethod.cpp\
           screen400/handinput.cpp\
           screen400/letterwidget.cpp\
           screen400/digitwidget.cpp\
           screen400/characterwidget.cpp\
           screen400/editwidget.cpp\
           screen400/voiceinput.cpp
}else{
SOURCES += InputInit.cpp\
           screen400/swtichslectewidget.cpp\
           screen400/pinyininputmethod.cpp\
           screen400/fcitxMethod.cpp\
           screen400/handinput.cpp\
           screen400/letterwidget.cpp\
           screen400/digitwidget.cpp\
           screen400/characterwidget.cpp\
           screen400/editwidget.cpp

}
RESOURCES += screen400/inputmethod.qrc

}


PRE_TARGETDEPS += libinputkernel.a

QMAKE_POST_LINK =make install

install_include.files =InputInit.h InputMethod.h

install_include.path  =$$INNOVBOX_INPUTMETHOD_INSTALL_DIR/include

target.path  = $$INNOVBOX_INPUTMETHOD_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
