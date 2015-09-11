include(../product.conf)

system(chmod -x *.h)


TEMPLATE = lib 
TARGET = 

INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility

# Input
HEADERS += ConcreteTableBase.h \
           DriversLoginTable.h \
           ElectronicFenceTable.h \
           MultiRowWithUniqueIndexTable.h \
           SingleRowTable.h \
           SQLiteTable.h \
           TableNameDef.h \
           VDRDeployParaTable.h \
           VDRBaseConfigTable.h \
           VDRMonitorTable.h \
           VDRRunParaTable.h \
           VDRTrackInfoTable.h \
	   VDRPreTiredTable.h \
           VDRContactsTable.h \
           VDREventsTable.h \
	   VDRAccelerateTable.h \
	   LargeTableBase.h	\
	   VDRPhoneTable.h \
           VDRUpdateTable.h \
	   VDRStudentIdTable.h  \
           VDRJT8080TextRecordTable.h
                
SOURCES += ConcreteTableBase.cpp \
           DriversLoginTable.cpp \
           ElectronicFenceTable.cpp \
           MultiRowWithUniqueIndexTable.cpp \
           SingleRowTable.cpp \
           SQLiteTable.cpp \
           VDRDeployParaTable.cpp \
           VDRBaseConfigTable.cpp \
           VDRMonitorTable.cpp \
           VDRRunParaTable.cpp \
           VDRTrackInfoTable.cpp \
	   VDRPreTiredTable.cpp \
           VDRContactsTable.cpp \
           VDREventsTable.cpp \
	   VDRAccelerateTable.cpp \
           LargeTableBase.cpp	\
	   VDRPhoneTable.cpp \
	   VDRUpdateTable.cpp \
	   VDRStudentIdTable.cpp \
           VDRJT8080TextRecordTable.cpp

QMAKE_POST_LINK =make install

install_include.files = ConcreteTableBase.h \
                        DriversLoginTable.h \
                        ElectronicFenceTable.h \
                        MultiRowWithUniqueIndexTable.h \
                        SingleRowTable.h \
                        SQLiteTable.h \
                        TableNameDef.h \
                        VDRDeployParaTable.h \
                        VDRBaseConfigTable.h \
                        VDRMonitorTable.h \
                        VDRRunParaTable.h \
                        VDRTrackInfoTable.h \
			VDRPreTiredTable.h \
                        VDRContactsTable.h \
                        VDREventsTable.h \
			VDRAccelerateTable.h \
                        LargeTableBase.h \
			VDRPhoneTable.h \ 
			VDRUpdateTable.h \ 
			VDRTrackInfoTable.h \
		        VDRStudentIdTable.h \
                        VDRJT8080TextRecordTable.h

install_include.path  = $$INNOVBOX_TABLES_INSTALL_DIR/include

target.path  = $$INNOVBOX_TABLES_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*
