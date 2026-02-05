#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

int main() {
    const char* filename = "mmap_posix.txt";
    const size_t FILE_SIZE = 1024;
    
    // 1. Create or open the file
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. Resize the file to the desired size
    if (ftruncate(fd, FILE_SIZE) == -1) {
        std::cerr << "Error resizing file: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // 3. Memory map the file
    char* map = static_cast<char*>(mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (map == MAP_FAILED) {
        std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // File descriptor is no longer needed after mapping
    close(fd);

    // 4. Access and modify the memory as a pointer/array
    for (size_t i = 0; i < FILE_SIZE; ++i) {
        map[i] = 'A' + (i % 26);
    }
    std::cout << "Data written to memory-mapped region." << std::endl;

    // 5. Unmap the memory and clean up
    if (munmap(map, FILE_SIZE) == -1) {
        std::cerr << "Error un-mapping file: " << strerror(errno) << std::endl;
    }

    // Optional: remove the file
    //std::remove(filename);

    return 0;
}