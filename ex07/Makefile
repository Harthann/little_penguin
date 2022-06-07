obj-m = debug.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

install:
	insmod misc.ko

uninstall:
	rmmod misc.ko

reinstall: uninstall install
