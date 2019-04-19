obj-m += lkmguard.o
lkmguard-objs += lkmguard_main.o
lkmguard-objs += integrity/integrity.o
lkmguard-objs += integrity/hooks.o
lkmguard-objs += keylogger/keylogger.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

load:
	sudo insmod lkmguard.ko
	modinfo lkmguard.ko
	
unload:
	sudo rmmod lkmguard
