QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addremovecustomer.cpp \
    addremoverooms.cpp \
    customerstatuspage.cpp \
    directorypage.cpp \
    facilityinfo.cpp \
    facilityinfo2.cpp \
    loginpage.cpp \
    main.cpp \
    permissionpage.cpp \
    roominfobox.cpp \
    statusreportpage.cpp

HEADERS += \
    addremovecustomer.h \
    addremoverooms.h \
    customer.h \
    customerstatuspage.h \
    databaseManager.h \
    directorypage.h \
    facilityinfo.h \
    facilityinfo2.h \
    loginpage.h \
    permissionpage.h \
    room.h \
    roominfobox.h \
    statusreportpage.h

FORMS += \
    addremovecustomer.ui \
    addremoverooms.ui \
    customerstatuspage.ui \
    directorypage.ui \
    facilityinfo.ui \
    facilityinfo2.ui \
    loginpage.ui \
    permissionpage.ui \
    roominfobox.ui \
    statusreportpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    SSDatabase.sqlite
