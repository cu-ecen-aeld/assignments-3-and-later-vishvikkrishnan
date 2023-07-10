#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char **argv) {
    int fd;
    int str_length;
    int num_written;

    openlog(NULL, 0, LOG_USER);

    // Arguments insufficient to perform write
    if (argc-1 < 2) {
        syslog(LOG_ERR, "Writer called with less than 2 arguments!");
        printf("Enter both the file path and the string to write\n");
        exit(1);
    }

    fd = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);

    // Failed file open/create
    if (fd == -1) {
        syslog(LOG_ERR, "File open error! Errno: %d (%s).", errno, strerror(errno));
        printf("Could not open file %s; Error %d: %s\n", argv[1], errno, strerror(errno));
        exit(1);
    }
    str_length = strlen(argv[2]);
    num_written = write(fd, argv[2], str_length);

    // Failed write 
    if (num_written == -1) {
        syslog(LOG_ERR, "Write error! Errno: %d (%s)", errno, strerror(errno));
        printf("Write failed! Error %d (%s)\n", errno, strerror(errno));
        exit(1);
    }
    // Partial write scenario -- could be due to interruption during write()
    else if (num_written != str_length) {
        syslog(LOG_ERR, "Partial write! Wrote only %d/%d characters. Errorno: %d (%s)", num_written, str_length, errno, strerror(errno));
        printf("Partial write! Wrote only %d/%d characters. Error %d: %s\n", num_written, str_length, errno, strerror(errno));
        exit(1);
    }

    // Write was successful
    syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);
    printf("Write successful!\n");

    closelog();
}