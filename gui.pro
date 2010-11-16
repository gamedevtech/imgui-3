TEMPLATE = lib
CONFIG = release qt shared embed_manifest_dll

DESTDIR = lib
OBJECTS_DIR = tmp
MOC_DIR = tmp

HEADERS = include/gui.h src/widgets.h
SOURCES = src/gui.cpp
INCLUDEPATH += include

DEFINES += GUI_EXPORTS

QT += opengl

LIBS += $$QMAKE_LIBS_CORE
LIBS += $$QMAKE_LIBS_GUI
LIBS += $$QMAKE_LIBS_NETWORK
LIBS += $$QMAKE_LIBS_OPENGL