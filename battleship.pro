TEMPLATE = app

QT += dbus gui svg xml network
DEPENDPATH += . src src/dnssd
INCLUDEPATH += . src src/dnssd

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

images.path = $$INSTALLDIR/usr/share/matrix/images
images.files = data/battleship-icon.png

scripts.path = $$INSTALLDIR/usr/bin
scripts.files = scripts/*

INSTALLS += target images scripts

FORMS += ui/mainwindow.ui \
         ui/clientnetworkdialog.ui
# Input
HEADERS += src/animation.h \
           src/animator.h \
           src/battlefield.h \
           src/battlefieldview.h \
           src/button.h \
           src/clientnetworkdialog.h \
           src/colorproxy_p.h \
           src/controller.h \
           src/coord.h \
           src/delegate.h \
           src/dnssd/avahi-domainbrowser_p.h \
           src/dnssd/avahi-publicservice_p.h \
           src/dnssd/avahi-remoteservice_p.h \
           src/dnssd/avahi-servicebrowser_p.h \
           src/dnssd/avahi-servicetypebrowser_p.h \
           src/dnssd/avahi_domainbrowser_interface.h \
           src/dnssd/avahi_entrygroup_interface.h \
           src/dnssd/avahi_server_interface.h \
           src/dnssd/avahi_servicebrowser_interface.h \
           src/dnssd/avahi_serviceresolver_interface.h \
           src/dnssd/avahi_servicetypebrowser_interface.h \
           src/dnssd/dnssd_export.h \
           src/dnssd/domainbrowser.h \
           src/dnssd/domainmodel.h \
           src/dnssd/kdemacros.h \
           src/dnssd/publicservice.h \
           src/dnssd/remoteservice.h \
           src/dnssd/servicebase.h \
           src/dnssd/servicebase_p.h \
           src/dnssd/servicebrowser.h \
           src/dnssd/servicemodel.h \
           src/dnssd/servicetypebrowser.h \
           src/element.h \
           src/entity.h \
           src/grid.h \
           src/hitinfo.h \
           src/kbsrenderer.h \
           src/kgamecanvas.h \
           src/kgamerenderer.h \
           src/kgamerenderer_p.h \
           src/kgamerendererclient.h \
           src/kimagecache.h \
           src/kshareddatacache.h \
           src/kshareddatacache_p.h \
           src/ksharedptr.h \
           src/mainwindow.h \
           src/message.h \
           src/networkentity.h \
           src/playerentity.h \
           src/playerlabel.h \
           src/playfield.h \
           src/protocol.h \
           src/sea.h \
           src/seaview.h \
           src/ship.h \
           src/shot.h \
           src/simplemenu.h \
           src/sprite.h \
           src/spritefactory.h \
           src/stats.h \
           src/statswidget.h \
           src/uientity.h \
           src/welcomescreen.h \
           src/wpa/device.h \
           src/wpa/p2pservicemodel.h \
           src/wpa/group.h              \
           src/wpa/interface.h          \
           src/wpa/interfaces.h         \
           src/wpa/p2pdevice.h          \
           src/wpa/peer.h               \
           src/wpa/types.h              \
           src/wpa/wpa.h                \
           src/wpa/wps.h

SOURCES += src/animation.cpp \
           src/animator.cpp \
           src/battlefield.cpp \
           src/battlefieldview.cpp \
           src/button.cpp \
           src/clientnetworkdialog.cpp \
           src/colorproxy_p.cpp \
           src/controller.cpp \
           src/coord.cpp \
           src/dnssd/avahi-domainbrowser.cpp \
           src/dnssd/avahi-publicservice.cpp \
           src/dnssd/avahi-remoteservice.cpp \
           src/dnssd/avahi-servicebrowser.cpp \
           src/dnssd/avahi-servicetypebrowser.cpp \
           src/dnssd/avahi_domainbrowser_interface.cpp \
           src/dnssd/avahi_entrygroup_interface.cpp \
           src/dnssd/avahi_server_interface.cpp \
           src/dnssd/avahi_servicebrowser_interface.cpp \
           src/dnssd/avahi_serviceresolver_interface.cpp \
           src/dnssd/avahi_servicetypebrowser_interface.cpp \
           src/dnssd/domainmodel.cpp \
           src/dnssd/servicebase.cpp \
           src/dnssd/servicemodel.cpp \
           src/element.cpp \
           src/entity.cpp \
           src/kbsrenderer.cpp \
           src/kgamecanvas.cpp \
           src/kgamerenderer.cpp \
           src/kgamerendererclient.cpp \
           src/kimagecache.cpp \
           src/kshareddatacache.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/message.cpp \
           src/networkentity.cpp \
           src/playerentity.cpp \
           src/playerlabel.cpp \
           src/playfield.cpp \
           src/protocol.cpp \
           src/sea.cpp \
           src/seaview.cpp \
           src/ship.cpp \
           src/shot.cpp \
           src/simplemenu.cpp \
           src/sprite.cpp \
           src/spritefactory.cpp \
           src/stats.cpp \
           src/statswidget.cpp \
           src/uientity.cpp \
           src/welcomescreen.cpp \
           src/wpa/device.cpp \
           src/wpa/group.cpp              \
           src/wpa/interface.cpp          \
           src/wpa/interfaces.cpp         \
           src/wpa/p2pservicemodel.cpp \
           src/wpa/p2pdevice.cpp          \
           src/wpa/peer.cpp               \
           src/wpa/wpa.cpp                \
           src/wpa/wps.cpp
