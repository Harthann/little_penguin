obj-m = reverse.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

install:
	insmod reverse.ko

uninstall:
	rmmod reverse.ko

reinstall: uninstall install
