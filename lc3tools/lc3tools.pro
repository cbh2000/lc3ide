TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lex.lc3convert.c \
    lex.lc3.c \
    lc3sim.c \
    symbol.c

OTHER_FILES += \
    COPYING \
    lc3.def \
    CHANGE_LOG \
    lc3sim-tk \
    lc3.f \
    lc3os.sym \
    lc3os.obj \
    lc3os.asm \
    lc3convert.f \
    lc3sim-tk.def \
    README \
    NO_WARRANTY

HEADERS += \
    lc3sim.h \
    symbol.h

