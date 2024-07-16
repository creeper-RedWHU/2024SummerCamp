QT       += core gui
QT       += widgets
QT       += charts
QT       += sql
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    api.cpp \
    database.cpp \
    datavisualization.cpp \
    forecast.cpp \
    historyweather.cpp \
    lifeindexwidget.cpp \
    login_register.cpp \
    main.cpp \
    mainwindow.cpp \
    regis.cpp \
    sectionwidget.cpp \
    simulation.cpp \
    todayweather.cpp

HEADERS += \
    admin.h \
    api.h \
    database.h \
    datavisualization.h \
    forecast.h \
    historyweather.h \
    lifeindexwidget.h \
    login_register.h \
    mainwindow.h \
    regis.h \
    sectionwidget.h \
    simulation.h \
    todayweather.h

FORMS += \
    admin.ui \
    datavisualization.ui \
    forecast.ui \
    historyweather.ui \
    lifeindexwidget.ui \
    login_register.ui \
    mainwindow.ui \
    regis.ui \
    sectionwidget.ui \
    simulation.ui \
    todayweather.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
