#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    shm_unlink("/TransparentBank");
}
