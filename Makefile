obj-m += lkmguard.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

install:
	sudo ln -f -s `pwd`/build/lkmguard.ko /lib/modules/`uname -r`
	sudo depmod -a
	sudo modprobe lkmguard

uninstall:
	sudo modprobe -r lkmguard

info:
	modinfo ./build/lkmguard.ko
