obj-m = mntpoint.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

install:
	insmod mntpoint.ko

uninstall:
	rmmod mntpoint.ko

reinstall: uninstall install
