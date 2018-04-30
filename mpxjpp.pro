TEMPLATE = subdirs

SUBDIRS += \
    mpxjpp \
    tests \
    mpxjpp-tests \
    poifs \
    poifs-tests

poifs-tests.depends = poifs
mpxjpp-tests.depends = mpxjpp
mpxjpp.depends = poifs
