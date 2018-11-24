#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/msdos_fs.h>

int main(int argc, char* argv[]) {
	int ret = 0;
	int eu20_fd = 0;
	int label_length = 0;	
	
	// Handle args
	if (argc != 3) {
		printf ("Usage: %s <mountpoint of fatfs> <new volume label>", argv[0]);
		ret = -1;
		goto exit;
	}
	
	label_length = strnlen(argv[2], MSDOS_NAME + 1); 
	
	if (label_length == MSDOS_NAME + 1) {
		printf("The max length for volume label is %d", MSDOS_NAME);
		ret = -1;
		goto exit;
	}	
	
	eu20_fd = open(argv[1], 0);
	if (eu20_fd < 0) {
		printf("Failed opening filesystem with error %d\n", errno);
		ret = -1;
		goto exit;
	}
	
	if (ioctl(eu20_fd, FAT_IOCTL_SET_VOLUME_LABEL, argv[2])) {
		printf("Call to eu20 ioctl failed with error %d\n", errno);
		ret = -1;
		goto close_fd;
	}
	
	printf("ioctl called successfully\n");

close_fd:
	close(eu20_fd);
exit:
	return ret;
}
