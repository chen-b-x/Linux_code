obj-m += virtual_gpio_dev.o
KDIR:=/lib/modules/$(shell uname -r)/build
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order