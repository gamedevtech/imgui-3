TEMPLATE = lib
CONFIG = release qt dll embed_manifest_dll

DESTDIR = lib
OBJECTS_DIR = tmp
MOC_DIR = tmp

HEADERS = include/gui.h src/widgets.h
SOURCES = src/gui.cpp
INCLUDEPATH += include

DEFINES += GUI_EXPORTS

QT += opengl