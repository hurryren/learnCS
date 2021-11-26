

KERN_DIR =  /lib/modules/5.11.0-40-generic/build 

all:
	make -C $(KERN_DIR) M=$(shell pwd) modules 

clean:
	make -C $(KERN_DIR) M=$(shell pwd) modules clean
	rm -rf modules.order *.ko

obj-m	+= hello_drv.o


