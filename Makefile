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

clean: clean_kmod clean_lib 

clean_kmod: 
	-$(MAKE) -C $(KDIR) M=$$PWD clean

clean_lib:
	-rm *.so *.oo

.PHONY: all kmod lib clean clean_kmod clean_lib
