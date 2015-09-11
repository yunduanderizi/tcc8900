include(../product.conf)

system(chmod -x *.h tcc/*.h ../common/message/*.h)

TEMPLATE = app
TARGET = capapp
DEPENDPATH += . tcc cam

INCLUDEPATH += $$PACKAGE_OPENCV_DIR/include
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include

INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_JT808_INSTALL_DIR/include 

INCLUDEPATH += .tcc
INCLUDEPATH += .cam
INCLUDEPATH += $$PACKAGE_OPENCV_DIR/include

DEFINES +=  TCC_89XX_INCLUDE
LIBS += -L$$PACKAGE_OPENCV_DIR/lib -ljpeg -lcv -lhighgui -lcvaux -lml -lz -lcxcore  -lpthread -lm -lrt
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm


# Input
HEADERS += rsc.h \
	   main.h \
           v4l2.h \
           cam/queue.h \
           cam/yuv2jpg.h \
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
           tcc/vpu_clk_ctrl.h \
           tcc/viqe_cmd.h \
           tcc/viqe_lib.h \
           capapp.h

         
         
SOURCES += v4l2.c \
           rsc.c \
           cam/saclejpeg.c \
           cam/yuv2jpg.c \
           cam/queue.c \
           tcc/Virtual.c \
           tcc/M2Mscaler_Video.c \
           tcc/test_util.c \
           tcc/sys_memory.c \
           tcc/matrix_inv.c \
           tcc/triangle.c \
           tcc/viqe_lib.c \
           tcc/viqe_cmd.c \
           tcc/gamut_main.c \
           tcc/tcc_viqe_interface.c \
           main.c \
           capapp.cpp 


QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_CAPAPP_INSTALL_DIR

install_include.files = $$INNOVBOX_COMMON_SRC_DIR/message/CapappDBusMessage.h

install_include.path  = $$INNOVBOX_CAPAPP_INSTALL_DIR/include

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$target.path/* $$install_include.path/*

