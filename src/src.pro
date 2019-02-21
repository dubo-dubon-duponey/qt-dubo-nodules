TEMPLATE = lib
QT = core widgets network
QT += webenginewidgets webengine

PROJECT_ROOT = $$PWD/..
include($$PROJECT_ROOT/config/qmakeitup.pri)
system(mkdir -p $$quote($$DESTDIR/../include/lib$${TARGET}/web))
system(mkdir -p $$quote($$DESTDIR/../include/lib$${TARGET}/network))
system(cp -rf $$quote($$PWD/lib$${TARGET}/web/*.h) $$quote($$DESTDIR/../include/lib$${TARGET}/web))
system(cp -rf $$quote($$PWD/lib$${TARGET}/network/*.h) $$quote($$DESTDIR/../include/lib$${TARGET}/network))

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/lib$${TARGET}/global.h \
            $$PWD/lib$${TARGET}/root.h \
            $$PWD/lib$${TARGET}/filesystem.h \
            $$PWD/lib$${TARGET}/walker.h \
            $$PWD/lib$${TARGET}/handler.h \
            $$PWD/lib$${TARGET}/icon.h \
            $$PWD/lib$${TARGET}/tray.h \
            $$PWD/lib$${TARGET}/menu.h \
            $$PWD/lib$${TARGET}/menubar.h \
            $$PWD/lib$${TARGET}/action.h \
            $$PWD/lib$${TARGET}/network/controller.h \
            $$PWD/lib$${TARGET}/network/jar.h \
            $$PWD/lib$${TARGET}/network/cache.h \
            $$PWD/lib$${TARGET}/network/reply.h \
            $$PWD/lib$${TARGET}/network/server.h \
            $$PWD/lib$${TARGET}/web/view.h \
            $$PWD/lib$${TARGET}/web/kit.h \


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
