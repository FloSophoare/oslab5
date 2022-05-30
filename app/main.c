#include "types.h"
#include "utils.h"
#include "lib.h"


#define DIRENTRY_SIZE 128
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

union DirEntry {
	uint8_t byte[128];
	struct {
		uint32_t inode;
		char name[64];
	};
};

typedef union DirEntry DirEntry;


int ls(char *destFilePath) {
	 int fd;
    int ret;
    int size;
    uint8_t buffer[MAX_BUFFER_SIZE];
    int i;
    DirEntry* dirEntry;
    printf("ls %s\n",destFilePath);
    fd = open(destFilePath,O_READ | O_DIRECTORY);
    if(fd == -1){
        printf("ls failed\n");
        return -1;
    }
    size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    for(i=0;i<size/DIRENTRY_SIZE;++i){
        ret = read(fd,buffer,DIRENTRY_SIZE);
        if(ret == -1){
            printf("ls failed\n");
            return -1;
        }
        dirEntry = (DirEntry*)buffer;
        if(dirEntry->inode != 0){
            printf("%s ",dirEntry->name);
        }

    }
    printf("\n");
    ret = close(fd);
    printf("ls success\n");
    return 0;
}

int cat(char *destFilePath){
    int fd;
    int ret;
    uint8_t buffer[MAX_BUFFER_SIZE];
    printf("cat %s\n",destFilePath);
    fd = open(destFilePath,O_READ);
    if(fd == -1){
        printf("cat failed\n");
        return -1;
    }
    while(1){
        ret = read(fd,buffer,MAX_BUFFER_SIZE/2);
        if(ret == -1){
            printf("cat faild\n");
            return -1;
        }
        if(ret == 0){
            break;
        }
        buffer[ret] = 0;
        printf("%s",buffer);
    }
    close(fd);
    printf("\ncat success\n");
    return 0;
}
	
int uEntry(void) {
	int fd = 0;
	int i = 0;
	char tmp = 0;
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat("/usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove("/usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr");
	ls("/");
	printf("create /usr/\n");
	fd = open("/usr/", O_CREATE | O_DIRECTORY);
	close(fd);
	ls("/");
	
	exit();
	return 0;
}
