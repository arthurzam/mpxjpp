CONFIG += c++14

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CLEAN += $(TARGET)

INCLUDEPATH += $$PWD/../

#LIBS += -L../src -lmylib

HEADERS += \
    object_testsuite.h \
    listwithcallbacks_testsuite.h \
    daterange_testsuite.h \
    sized_array_testsuite.h

TEST_HEADERS = $$find(HEADERS, .*testsuite.h)

generate_tests.name = "Generate CxxTests"
generate_tests.input = TEST_HEADERS
generate_tests.output =  $$PWD/runner.cc
generate_tests.commands = cxxtestgen --root --have-std --have-eh --error-printer -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
generate_tests.CONFIG += no_link target_predeps combine
generate_tests.clean += ${QMAKE_FILE_OUT}
generate_tests.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += generate_tests
QMAKE_CXXFLAGS_RELEASE += -flto -fuse-linker-plugin
QMAKE_LFLAGS_RELEASE += -flto -fuse-linker-plugin
