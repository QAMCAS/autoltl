include padkit/compile.mk

all: bin/eventdivider

.PHONY: all clean

bin: ; mkdir bin

clean: ; rm -rf bin padkit

padkit: ; git clone https://github.com/yavuzkoroglu/padkit.git

padkit/compile.mk: padkit; $(make padkit/compile.mk)

padkit/include/padkit.h: padkit; make -C padkit include/padkit.h

padkit/lib/libpadkit.a: padkit; make -C padkit lib/libpadkit.a

bin/eventdivider:           \
    bin                     \
    padkit/compile.mk       \
    padkit/lib/libpadkit.a  \
    padkit/include/padkit.h \
    src/eventdivider.c      \
    ; ${COMPILE} -Ipadkit/include padkit/lib/libpadkit.a src/eventdivider.c -o bin/eventdivider
