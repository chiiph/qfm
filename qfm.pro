TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src
CONFIG += qt debug

# Input
HEADERS += src/commandbuffer.h \
	src/qfmcore.h \
	src/qfm.h \
	src/listitem.h \
	src/pastecommand.h \
	src/movecommand.h \
	src/deletecommand.h
SOURCES += src/commandbuffer.cpp \
	src/main.cpp \
	src/qfmcore.cpp \
	src/qfm.cpp \
	src/listitem.cpp \
	src/pastecommand.cpp \
	src/movecommand.cpp \
	src/deletecommand.cpp

MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src/ui
FORMS += src/ui/mainwin.ui
