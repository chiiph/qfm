TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = mkdircommand.h
SOURCES       = mkdircommand.cpp
TARGET        = $$qtLibraryTarget(plugin_mkdir)
DESTDIR       = ../../../../bin/plugins

FORMS += ui/mkdirprompt.ui
UI_DIR = ui
