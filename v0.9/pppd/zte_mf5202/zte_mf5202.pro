include(../../product.conf)

system(mkdir -p $$INNOVBOX_PPPD_DIR)

QT =

TEMPLATE = lib 
TARGET = 

# Input
				
SOURCES += pppd.cpp






QMAKE_POST_LINK = install -m 755 -p pppd ppp-on chat ppp-on-dialer $$INNOVBOX_PPPD_DIR/

QMAKE_DISTCLEAN += $$INNOVBOX_PPPD_DIR/*


