#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "test_file.h"

void create_test_file(const char *path) {
    remove(path);
    int fd = open(path, O_CREAT | O_EXCL | O_WRONLY, S_IRWXU | S_IROTH);
    assert(fd > 0);
    char *buffer = "Hello I bims 1 Test-File\n";
    size_t len = strlen(buffer);
    for (int i = 0; i < 100; ++i) {
        assert(write(fd, buffer, len) != -1);
    }
    close(fd);
}
