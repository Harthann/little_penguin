obj-m = misc.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

install: uninstall
	insmod ./misc.ko

uninstall:
	rmmod ./misc.ko
