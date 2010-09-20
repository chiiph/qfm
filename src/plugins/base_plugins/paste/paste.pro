TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = pastecommand.h
SOURCES       = pastecommand.cpp
TARGET        = $$qtLibraryTarget(plugin_paste)
DESTDIR       = ../../../../bin/plugins
