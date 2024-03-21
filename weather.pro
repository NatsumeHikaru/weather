QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hook.cpp \
    main.cpp \
    weatherdate.cpp \
    widget.cpp

HEADERS += \
    hook.h \
    weatherTool.h \
    weatherdate.h \
    widget.h

FORMS += \
    widget.ui

TRANSLATIONS += \
    weather_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

LIBS += -lUser32
LIBS += -lkernel32

DISTFILES += \
    fonts/851CHIKARA-DZUYOKU_kanaA.ttf \
    fonts/song.ttf \
    fonts/кн.ttf
