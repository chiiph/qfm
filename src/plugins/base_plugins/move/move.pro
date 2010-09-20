TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = movecommand.h
SOURCES       = movecommand.cpp
TARGET        = $$qtLibraryTarget(plugin_move)
DESTDIR       = ../../../../bin/plugins
