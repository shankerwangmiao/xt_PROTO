KDIR ?= /lib/modules/`uname -r`/build
CC := gcc
CCLD := $(CC)

LDLIBS += `pkg-config --libs libiptc`
CFLAGS += `pkg-config --cflags libiptc`

all: kmod lib

kmod: xt_PROTO.ko

xt_PROTO.ko: xt_PROTO.c
	$(MAKE) -C $(KDIR) M=$$PWD

lib: libxt_PROTO.so 

lib%.so: lib%.oo
	$(CCLD) $(CFLAGS) -shared $(LDFLAGS) -o $@ $< $(LDLIBS)

%.oo: %.c
	$(CC) -c -DPIC -fPIC $(CFLAGS) -o $@ $<   

clean: clean-kmod clean-lib 

clean-kmod: 
	-$(MAKE) -C $(KDIR) M=$$PWD clean

clean-lib:
	-rm *.so *.oo

install: install-kmod install-lib

install-kmod: kmod
	mkdir -p /lib/modules/`uname -r`/kernel/misc
	cp xt_PROTO.ko /lib/modules/`uname -r`/kernel/misc/
	depmod -A `uname -r`

install-lib: lib
	cp libxt_PROTO.so `pkg-config --variable=xtlibdir xtables`/

.PHONY: all kmod lib clean clean-kmod clean-lib install install-kmod install-lib
