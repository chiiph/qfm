TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = gotocommand.h
SOURCES       = gotocommand.cpp
TARGET        = $$qtLibraryTarget(plugin_goto)
DESTDIR       = ../../../../bin/plugins

FORMS += ui/gotoprompt.ui
UI_DIR = ui
