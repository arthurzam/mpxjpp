include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    filesystem/stringutils_testsuite.h

SOURCES += \
    main.cpp \


INCLUDEPATH += $$PWD/../
unix:!macx: LIBS += -L$$OUT_PWD/../poifs/ -lpoifs
QMAKE_RPATHDIR += ../poifs/

QMAKE_CXXFLAGS_RELEASE += -flto -fuse-linker-plugin
QMAKE_LFLAGS_RELEASE += -flto -fuse-linker-plugin
