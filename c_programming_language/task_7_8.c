#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define REPORT(code, desc)                                                     \
    if (code < 0) {                                                            \
        fprintf(stderr, "%s, %s: %s", program_name, file_name, desc);          \
        exit(-1);                                                              \
    }

int main(int argc, char **argv) {
    char *program_name = *argv++, *file_name, buf[BUFSIZ];
    int fd, out_fd = fileno(stdout);
    long code;
    int page = 1;
    argc--;
    while (argc--) {
        file_name = *argv++;
        fd = (int)syscall(SYS_open, file_name, O_RDONLY);
        REPORT(fd, "failed to open");

        sprintf(buf, "\n--- FILE: %s --- [ %d ] \n", file_name, page);
        code = syscall(SYS_write, out_fd, buf, strlen(buf));
        REPORT(code, "failed to write page header");

        for (;;) {
            code = syscall(SYS_read, fd, buf, BUFSIZ);
            REPORT(code, "failed to read");

            if (code == 0)
                // that means eof
                break;

            code = syscall(SYS_write, out_fd, buf, code);
            REPORT(code, "failed to read");
        }
        sprintf(buf, "\n--- [ %d ]\n", page);
        code = syscall(SYS_write, out_fd, buf, strlen(buf));
        REPORT(code, "failed to write page footer");

        code = syscall(SYS_close, fd);
        REPORT(code, "failed to close");

        ++page;
    }
}
