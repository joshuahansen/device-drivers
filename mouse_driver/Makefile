obj-m := s3589185mouse.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
NAME := s3589185mouse
USER_PROG := userProg

MAJOR_NUM := 247

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)
	gcc -o userProg userProg.c

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order .*.cmd .tmp* $(USER_PROG)

install:
	sudo insmod s3589185mouse.ko
	sudo chmod 666 /sys/devices/platform/mouse/coordinates
	dmesg | tail
	cat /proc/bus/input/devices | grep $(NAME) -A 10
	sudo gpm -m /dev/input/event6 -t evdev
	sudo /etc/init.d/gpm start

uninstall:
	sudo rmmod s3589185mouse
	dmesg | tail
