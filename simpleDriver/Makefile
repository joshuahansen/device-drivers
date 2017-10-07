obj-m := s3589185Device.o
KDIR := /lib/modules/4.10.0-35-generic/build
PWD := $(shell pwd)
DEV := /dev/s3589185Device
MAJOR_NUM := 246

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order

install:
	sudo insmod s3589185Device.ko
	sudo mknod $(DEV) c $(MAJOR_NUM) 0
	sudo chmod 777 $(DEV)
