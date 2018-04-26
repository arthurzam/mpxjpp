include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    common/tst_object.h

SOURCES += \
        main.cpp \
    tst_daterange.cpp \
    tst_listwithcallbacks.cpp


INCLUDEPATH += $$PWD/../
unix:!macx: LIBS += -L$$OUT_PWD/../mpxjpp/ -lmpxjpp
QMAKE_RPATHDIR += ../mpxjpp/

QMAKE_CXXFLAGS_RELEASE += -flto -fuse-linker-plugin
QMAKE_LFLAGS_RELEASE += -flto -fuse-linker-plugin
