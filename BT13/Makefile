obj-m := cdev_gpio.o

KDIR := /home/kien/yocto/my-build/tmp/work/raspberrypi3_64-poky-linux/linux-raspberrypi/1_5.15.92+gitAUTOINC+509f4b9d68_14b35093ca-r0/linux-raspberrypi3_64-standard-build

PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=aarch64-poky-linux-gnu- modules

clean:
	make -C $(KDIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=aarch64-poky-linux-gnu- clean






