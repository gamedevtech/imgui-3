TEMPLATE = lib
CONFIG = release shared qt embed_manifest_dll

DESTDIR = lib
OBJECTS_DIR = tmp
MOC_DIR = tmp

HEADERS = include/gui.h src/widgets.h
SOURCES = src/gui.cpp
INCLUDEPATH += include

DEFINES += GUI_EXPORTS
DEFINES += QT_NO_DEBUG_OUTPUT

QT += opengl

win32 {
  
  win32-mingw {
    TEMP = $$[QT_INSTALL_LIBS] libQtGui.prl    
  }
  else {
    TEMP = $$[QT_INSTALL_LIBS] QtGui.prl        
  }

  include($$join(TEMP, "/"))
  
  contains(QMAKE_PRL_CONFIG, shared) {
    # message(Shared Qt)
  } 
  else {
    # message(Static Qt)
  
    DEFINES += QT_NODLL
    
    LIBS += $$QMAKE_LIBS_CORE
    LIBS += $$QMAKE_LIBS_GUI
    LIBS += $$QMAKE_LIBS_NETWORK
    LIBS += $$QMAKE_LIBS_OPENGL
  }

}