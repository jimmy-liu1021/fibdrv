#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


#define FIB_DEV "/dev/fibonacci"


int main()
{
    char buf[100];
    // char write_buf[] = "testing writing";
    int offset = 100;
    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        long long sz = read(fd, buf, 100);
        if (sz)
            printf("returned message was truncated!\n");
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence %s.\n",
               i, buf);
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        long long sz = read(fd, buf, 1);
        if (sz)
            printf("returned message was truncated!\n");
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence %s.\n",
               i, buf);
    }
    close(fd);
    return 0;
}
