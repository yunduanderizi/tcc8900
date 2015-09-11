include(../product.conf)

system(chmod -x *.h */*.h)

QT  += xml 

TEMPLATE = lib 
TARGET = 

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$PACKAGE_CURL_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_SRC_DIR
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_PROJECTCONFIG_SRC_DIR
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/login
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/news
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/peccancyHint
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/stock
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/trafficNotice
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/weatherForecast
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/locationUpload
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/authenticationReport
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/ucsInfoGather
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/getTelephone
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/carInfoMonitor
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/client
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/server
INCLUDEPATH += $$INNOVBOX_BSMANAGER_SRC_DIR/message

LIBS += -L$$PACKAGE_CURL_DIR/lib -lcurl
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_CARES_DIR/lib -lcares


!CONFIG(PC){
LIBS += -L$$PACKAGE_TSLIB_DIR/lib -lts
}

# Input
HEADERS += bsManagerCommon.h \
           bsManagerErrCode.h \
           trafficNotice/bsTrafficNotice.h \
           locationUpload/bsLocationUpload.h \
           weatherForecast/bsWeatherForecast.h \
           stock/bsStock.h \
           authenticationReport/bsAuthenticationReport.h \
           peccancyHint/bsPeccancyHint.h \
           login/bsLogin.h \
           news/bsNews.h \
           json/json.h \
           json/config.h \
           carInfoMonitor/bsCarInfoMonitor.h \
           carInfoMonitor/carDataGather.h \
           carInfoMonitor/carDataUpload.h \
           carInfoMonitor/OBDDataGather.h \
           carInfoMonitor/OBDDataUpload.h \
           getTelephone/bsGetTelephone.h \
           ucsInfoGather/bsUcsInfoGather.h \
           client/bsClient.h \
           server/bsServer.h \
           message/bsManagerMessage.h \
           bsCommunalLock.h

SOURCES += bsManagerCommon.cpp \
           trafficNotice/bsTrafficNotice.cpp \
           locationUpload/bsLocationUpload.cpp \
           weatherForecast/bsWeatherForecast.cpp \
           stock/bsStock.cpp \
           authenticationReport/bsAuthenticationReport.cpp \
           peccancyHint/bsPeccancyHint.cpp \
           login/bsLogin.cpp \
           news/bsNews.cpp \
           json/json_reader.cpp \
           json/json_value.cpp \
           json/json_writer.cpp \
           carInfoMonitor/bsCarInfoMonitor.cpp \
           carInfoMonitor/carDataGather.cpp \
           carInfoMonitor/carDataUpload.cpp \
           carInfoMonitor/OBDDataGather.cpp \
           carInfoMonitor/OBDDataUpload.cpp \
           getTelephone/bsGetTelephone.cpp \
           ucsInfoGather/bsUcsInfoGather.cpp \
           client/bsClient.cpp \
           server/bsServer.cpp \
           message/bsManagerMessage.cpp


SOURCES += $$INNOVBOX_COMMON_SRC_DIR/message/NaviDbusMessage.cpp
        

QMAKE_POST_LINK =make install


install_include.files = bsManagerCommon.h \
                        bsManagerErrCode.h \
                        trafficNotice/bsTrafficNotice.h \
                        locationUpload/bsLocationUpload.h \
                        weatherForecast/bsWeatherForecast.h \
                        stock/bsStock.h \
                        authenticationReport/bsAuthenticationReport.h \
                        peccancyHint/bsPeccancyHint.h \
                        login/bsLogin.h \
                        news/bsNews.h \
                        json/json.h \
                        json/config.h \
                        carInfoMonitor/bsCarInfoMonitor.h \
                        getTelephone/bsGetTelephone.h \
                        ucsInfoGather/bsUcsInfoGather.h \
                        client/bsClient.h \
                        server/bsServer.h \
                        message/bsManagerMessage.h

install_include.path  =$$INNOVBOX_BSMANAGER_INSTALL_DIR/include

target.path  = $$INNOVBOX_BSMANAGER_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*

