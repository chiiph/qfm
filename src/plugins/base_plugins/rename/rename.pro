TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../../../interfaces/
HEADERS       = renamecommand.h
SOURCES       = renamecommand.cpp
TARGET        = $$qtLibraryTarget(plugin_rename)
DESTDIR       = ../../../../bin/plugins

FORMS += ui/renameprompt.ui
UI_DIR = ui
