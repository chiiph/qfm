TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src
CONFIG += qt debug
DESTDIR = bin

# Input
HEADERS += src/interfaces/commandbuffer.h \
	src/core/qfmcore.h \
	src/qfm.h \
	src/listitem.h
SOURCES += src/core/qfmcore.cpp \
	src/main.cpp \
	src/qfm.cpp \
	src/listitem.cpp

MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src/ui
FORMS += src/ui/mainwin2.ui
