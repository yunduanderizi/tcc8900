include(../product.conf)

system(chmod -x *.h)

TEMPLATE = lib
TARGET = ./camapp
DEPENDPATH += . tcc test
INCLUDEPATH += . tcc

DEFINES +=  TCC_89XX_INCLUDE
QT +=xml

!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}


# Input
HEADERS += Camapp.h \
           rsc.h \
           v4l2.h \
           tcc/common.h \
           tcc/dev_viqe.h \
           tcc/gamut_map_internal.h \
           tcc/globals.h \
           tcc/ioctl_ckc.h \
           tcc/ioctl_ckcstr.h \
           tcc/M2Mscaler_Video.h \
           tcc/sys_memory.h \
           tcc/TCC89x_Physical.h \
           tcc/TCC89x_Structures.h \
           tcc/TCC_VPU_CODEC.h \
           tcc/tcc_vpu_interface.h \
           tcc/tccfb_ioctl.h \
           tcc/tccfb_ioctl_code.h \
           tcc/TCCUtil.h \
           tcc/test_util.h \
           tcc/Virtual.h \
           tcc/vpu_clk_ctrl.h
         
         
SOURCES += v4l2.c Camapp.c \
           rsc.c \
           tcc/Virtual.c \
           tcc/M2Mscaler_Video.c \
           tcc/test_util.c \
           tcc/sys_memory.c \
           tcc/matrix_inv.c \
           tcc/triangle.c


QMAKE_POST_LINK =make install

install_include.files = Camapp.h rsc.h v4l2.h  
install_include.path  =$$INNOVBOX_CAMERA_INSTALL_DIR/include

target.path  = $$INNOVBOX_CAMERA_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
