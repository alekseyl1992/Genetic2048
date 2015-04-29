#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T12:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Genetic2048
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++14

SOURCES += main.cpp\
        MainWindow.cpp \
    Game2048.cpp \
    WorkerThread.cpp \
    Chromosome.cpp \
    Genetic.cpp \
    util.cpp

HEADERS  += MainWindow.h \
    Game2048.h \
    WorkerThread.h \
    Chromosome.h \
    Genetic.h \
    util.h

FORMS    += MainWindow.ui
