QT       += core gui
QT       += widgets
QT       += charts
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    database.cpp \
    datavisualization.cpp \
    forecast.cpp \
    historyweather.cpp \
    login_register.cpp \
    main.cpp \
    mainwindow.cpp \
    sectionwidget.cpp \
    todayweather.cpp

HEADERS += \
    admin.h \
    database.h \
    datavisualization.h \
    forecast.h \
    historyweather.h \
    login_register.h \
    mainwindow.h \
    sectionwidget.h \
    todayweather.h

FORMS += \
    admin.ui \
    datavisualization.ui \
    forecast.ui \
    historyweather.ui \
    login_register.ui \
    mainwindow.ui \
    sectionwidget.ui \
    todayweather.ui

<<<<<<< HEAD

=======
>>>>>>> ed63de640a875decadeaa3afd237c0701f11f945

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
