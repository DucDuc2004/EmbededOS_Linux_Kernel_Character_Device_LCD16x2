#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/lcd"

int main(int argc, char *argv[]) {
    int fd;
    const char *msg;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"Message to display\"\n", argv[0]);
        return 1;
    }

    msg = argv[1];

    fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open /dev/lcd");
        return 1;
    }

    if (write(fd, msg, strlen(msg)) < 0) {
        perror("Failed to write to LCD");
        close(fd);
        return 1;
    }

    printf("Message written to LCD: %s\n", msg);
    close(fd);
    return 0;
}
