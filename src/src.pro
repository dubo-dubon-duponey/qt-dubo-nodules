TEMPLATE = lib
QT = core widgets network

PROJECT_ROOT = $$PWD/..
include($$PROJECT_ROOT/config/qmakeitup.pri)

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/lib$${TARGET}/global.h \
            $$PWD/lib$${TARGET}/root.h \
            $$PWD/lib$${TARGET}/filesystem.h \
            $$PWD/lib$${TARGET}/walker.h \
            $$PWD/lib$${TARGET}/network.h \
            $$PWD/lib$${TARGET}/networkjar.h \
            $$PWD/lib$${TARGET}/networkcache.h \
            $$PWD/lib$${TARGET}/networkreply.h \
            $$PWD/lib$${TARGET}/networkserver.h \
            $$PWD/lib$${TARGET}/handler.h \
            $$PWD/lib$${TARGET}/icon.h \
            $$PWD/lib$${TARGET}/tray.h \
            $$PWD/lib$${TARGET}/menu.h \
            $$PWD/lib$${TARGET}/menubar.h \
            $$PWD/lib$${TARGET}/action.h \


SOURCES +=  $$PWD/root.cpp \
            $$PWD/filesystem.cpp \
            $$PWD/walker.cpp \
            $$PWD/networkcache.cpp \
            $$PWD/handler.cpp \
            $$PWD/icon.cpp \
            $$PWD/tray.cpp \
            $$PWD/menu.cpp \
            $$PWD/menubar.cpp \
            $$PWD/action.cpp
