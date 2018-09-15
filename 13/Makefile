obj-m += hello.o

all:
	make -C /lib/modules/4.15.0-33-generic/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
