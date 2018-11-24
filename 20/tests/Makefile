C      = gcc
CFLAGS  = -g -I "../kmod"
RM      = rm -f


default: all

all: build

build: call_ioctl.c
	$(CC) $(CFLAGS) -o call_ioctl call_ioctl.c

clean veryclean:
	$(RM) call_ioctl

