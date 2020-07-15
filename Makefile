# KERNELDIR要与uname -r 中的kernel版本匹配，当前采用手动加载，后续看如何
# 直接从uname -r中提取出结果来
# KERNELDIR := /lib/modules/4.15.0-101-generic/build
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := chrdev.o
#chrdev-objs += 
.PHONY: all clean

all: build

.PHONY:build
# 有关下面核心命令的原理，可以参考KERNELDIR中的Makefile
build:
	echo $(MAKE)
	echo $(KERNELDIR)
	echo $(PWD)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	echo $(RM)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


