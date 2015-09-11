PACKAGE_DIR=/home/xingqq/svndir/tcc890x_common/trunk/packages/target
PACKAGE_EXPAT_DIR=$(PACKAGE_DIR)/expat
PACKAGE_BOOST_DIR=$(PACKAGE_DIR)/libboost_1_49_0
PACKAGE_SQLITE_DIR=$(PACKAGE_DIR)/sqlite3
PACKAGE_TSLIB_DIR=$(PACKAGE_DIR)/tslib1.0
PACKAGE_QT_DIR=$(PACKAGE_DIR)/qt4.6.2/release
INNOVBOX_INSTALL_DIR=/home/xingqq/build/host/innovbox
INNOVBOX_TABLES_INSTALL_DIR=$(INNOVBOX_INSTALL_DIR)/table
INNOVBOX_UTILITY_INSTALL_DIR=$(INNOVBOX_INSTALL_DIR)/utility

CPPFLAGS=-I$(PACKAGE_EXPAT_DIR)/include -I$(INNOVBOX_TABLES_INSTALL_DIR)/include -I$(PACKAGE_BOOST_DIR)/include -I$(PACKAGE_SQLITE_DIR)/include -I$(INNOVBOX_UTILITY_INSTALL_DIR)/include
LDFLAGS=-L$(INNOVBOX_TABLES_INSTALL_DIR)/lib -L$(PACKAGE_EXPAT_DIR)/lib \
	    -L$(PACKAGE_TSLIB_DIR)/lib -L$(PACKAGE_BOOST_DIR)/lib -L$(PACKAGE_QT_DIR)/lib -L$(INNOVBOX_UTILITY_INSTALL_DIR)/lib -lexpat -ltables -lutility \
	    -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale -lts -lQtCore -lQtNetwork -lQtGui

all: importsetting readtuid generatemac select_start


importsetting: importsetting.cpp
	arm-none-linux-gnueabi-g++ $^ -o $@  $(LDFLAGS) $(CPPFLAGS)

readtuid: readtuid-new.cpp
	arm-none-linux-gnueabi-g++ $^ -o $@  $(LDFLAGS) $(CPPFLAGS)

generatemac: generatemac.cpp
	arm-none-linux-gnueabi-g++ $^ -o $@  $(LDFLAGS) $(CPPFLAGS)

select_start:select_start.c
	arm-none-linux-gnueabi-gcc $^ -o $@  $(LDFLAGS) $(CPPFLAGS)
    
clean:
	rm -fr importsetting readtuid generatemac select_start
