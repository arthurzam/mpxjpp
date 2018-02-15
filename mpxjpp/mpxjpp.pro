TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

QT       += core

INCLUDEPATH += ../

SOURCES += main.cpp \
    mpp/criteriareader.cpp \
    group.cpp \
    projectconfig.cpp \
    enums.cpp \
    recurringdata.cpp \
    fieldtype.cpp \
    projectcalendar.cpp \
    resourceassignment.cpp \
    criteria.cpp \
    duration.cpp \
    projectproperties.cpp \
    customfield.cpp \
    projectfile.cpp \
    task.cpp \
    mpp/mpputility.cpp

HEADERS += \
    daterange.h \
    priority.h \
    mpp/criteriareader.h \
    projectentity.h \
    group.h \
    mpxjpp-gens.h \
    projectconfig.h \
    timeunit.h \
    rate.h \
    view.h \
    viewstate.h \
    enums.h \
    listwithcallbacks.h \
    table.h \
    duration.h \
    timephased.h \
    fieldtype.h \
    recurringdata.h \
    task.h \
    subproject.h \
    costeatetable.h \
    availability.h \
    relation.h \
    common/object.h \
    assignmentfield.h \
    constraintfield.h \
    projectfield.h \
    projectcalendar.h \
    common/calendar.h \
    common/font.h \
    resourcefield.h \
    resourceassignment.h \
    common/strutils.h \
    criteria.h \
    filter.h \
    taskfield.h \
    projectproperties.h \
    customfield.h \
    common/uuid.h \
    projectfile.h \
    mpp/mpputility.h

QMAKE_CXXFLAGS_RELEASE += -Wpedantic -Wextra
#QMAKE_CXXFLAGS_RELEASE += -flto -fuse-linker-plugin
#QMAKE_LFLAGS_RELEASE += -flto -fuse-linker-plugin
