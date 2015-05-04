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

unix {
    LIBS += -L/usr/local/lib \
	    -lboost_system \
	    -lboost_filesystem \
	    -lboost_test_exec_monitor \
	    -lboost_regex \
	    -lboost_thread
}

win32 {
    INCLUDEPATH += C:/Projects/Qt/boost_1_56_0
    LIBS += -LC:/Projects/Qt/boost_1_56_0/stage/lib/ \
	-lws2_32 -lMswsock
    LIBS += -LC:/Projects/Qt/boost_1_56_0/stage/lib/ \
	    -lboost_system-mgw49-mt-1_56 \
	    -lboost_filesystem-mgw49-mt-1_56 \
	    -lboost_test_exec_monitor-mgw49-mt-1_56 \
	    -lboost_regex-mgw49-mt-1_56 \
	    -lboost_thread-mgw49-mt-1_56
}
