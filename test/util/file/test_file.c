#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "test_file.h"
#include "../out/out.h"

void create_test_file(const char *path) {
    remove(path);
    int fd = open(path, O_CREAT | O_EXCL | O_WRONLY, S_IRWXU | S_IROTH);
    PEASSERT(fd > 0, "create file failed")
    char *buffer = "Hello I bims 1 Test-File\n";
    size_t len = strlen(buffer);
    for (int i = 0; i < 100; ++i) {
        PEASSERT(write(fd, buffer, len) != -1, "write failed");
    }
    close(fd);
}
