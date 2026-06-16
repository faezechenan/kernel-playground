obj-m := m9_icmp_monitor.o

KMOD_NAME := $(basename $(obj-m))
KMOD := $(KMOD_NAME).ko
SHARED_FOLDER := shared

.PHONY: all build install clean

all: build install

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	rm -r -f *.mod.c .*.cmd *.symvers *.o

install:
	cp -av $(KMOD) shared

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
