TEMPLATE = app

# RESOURCES = battleship.qrc
QT += gui svg xml
DEPENDPATH += . src
INCLUDEPATH += . src

RCC_DIR = tmp
UI_DIR = tmp
MOC_DIR = tmp
OBJECTS_DIR = tmp
DESTDIR = tmp
RESOURCES = battleship.qrc


CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = battleship_debug
    QMAKE_CXXFLAGS += -O0
    DEFINES += DEBUG
} else {
    TARGET = battleship
}

INSTALLDIR= $$(INSTALLDIR)
isEmpty(INSTALLDIR) {
    INSTALLDIR = $$(PWD)/rootfs/
}
message ("The application will be installed in $$INSTALLDIR")

target.path = $$INSTALLDIR/usr/bin
INSTALLS += target

# Input
HEADERS += src/animation.h \
           src/animator.h \
           src/battlefield.h \
           src/battlefieldview.h \
           src/button.h \
           src/colorproxy_p.h \
           src/controller.h \
           src/coord.h \
           src/delegate.h \
           src/element.h \
           src/entity.h \
           src/grid.h \
           src/kbsrenderer.h \
           src/kimagecache.h \
           src/kgamecanvas.h \
           src/kgamerenderer.h \
           src/kgamerenderer_p.h \
           src/kgamerendererclient.h \
           src/kshareddatacache.h \
           src/kshareddatacache_p.h \
           ksharedptr.h \
           src/hitinfo.h \
           src/message.h \
           src/networkentity.h \
           src/playerlabel.h \
           src/playerentity.h \
           src/protocol.h \
           src/sea.h \
           src/seaview.h \
           src/ship.h \
           src/shot.h \
           src/sprite.h \
           src/spritefactory.h \
           src/stats.h \
           src/statswidget.h \
           src/uientity.h \
           src/welcomescreen.h
SOURCES += src/animation.cpp \
           src/animator.cpp \
           src/battlefield.cpp \
           src/battlefieldview.cpp \
           src/button.cpp \
           src/colorproxy_p.cpp \
           src/controller.cpp \
           src/coord.cpp \
           src/element.cpp \
           src/entity.cpp \
           src/kbsrenderer.cpp \
           src/kimagecache.cpp \
           src/kgamecanvas.cpp \
           src/kgamerenderer.cpp \
           src/kgamerendererclient.cpp \
           src/kshareddatacache.cpp \
           src/main.cpp \
           src/message.cpp \
           src/networkentity.cpp \
           src/playerentity.cpp \
           src/playerlabel.cpp \
           src/protocol.cpp \
           src/sea.cpp \
           src/seaview.cpp \
           src/ship.cpp \
           src/shot.cpp \
           src/sprite.cpp \
           src/spritefactory.cpp \
           src/stats.cpp \
           src/statswidget.cpp \
           src/uientity.cpp \
           src/welcomescreen.cpp
