TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = deletecommand.h
SOURCES       = deletecommand.cpp
TARGET        = $$qtLibraryTarget(plugin_delete)
DESTDIR       = ../../../../bin/plugins
