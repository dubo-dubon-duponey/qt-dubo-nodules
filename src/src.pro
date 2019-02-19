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
            $$PWD/lib$${TARGET}/handler.h

SOURCES +=  $$PWD/root.cpp \
            $$PWD/filesystem.cpp \
            $$PWD/walker.cpp \
            $$PWD/networkcache.cpp \
            $$PWD/handler.cpp
