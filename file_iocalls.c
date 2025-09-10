#include <stdio.h>      // Standard I/O functions: printf, etc.
#include <stdlib.h>     // Standard library functions: exit, etc.
#include <sys/types.h>  // Data types used in system calls
#include <sys/stat.h>   // Data returned by the stat() function
#include <fcntl.h>      // File control options: open flags
#include <unistd.h>     // POSIX API: read, write, lseek, close, etc.

#define BUFFER_SIZE 30

int main() {
    // Open the file "myfile.txt" for reading and writing
    int fd = open("x.txt", O_RDWR);
    int n;
    char buffer[BUFFER_SIZE];

    printf("File discriptor is %d\n", fd);
    if (fd == -1) {
        printf("Failed to open file.\n");
        exit(0);
    }

    // Read first 10 characters from the file
    printf("Reading first 10 characters from file: \n");
    n = read(fd, buffer, 10);
    write(1, buffer, n); // Write to stdout

    // Skip 5 characters from current position
    printf("\nSkipping 5 charecters from current position in the file: \n");
    lseek(fd, 5, SEEK_CUR); // Move file pointer 5 chars ahead
    n = read(fd, buffer, 10);
    write(1, buffer, n);

    // Go 10 characters before the current position
    printf("\nGoing 10 charecters before the current position in the file: \n");
    lseek(fd, -10, SEEK_CUR); // Move file pointer 10 chars back
    n = read(fd, buffer, 10);
    write(1, buffer, n);

    // Go to 5th last character in the file
    printf("\nGoing to 5th last charecter in the file: \n");
    lseek(fd, -5, SEEK_END); // Move to 5th last char from end
    n = read(fd, buffer, 5);
    write(1, buffer, n);

    // Go to the 3rd character in the file
    printf("\nGoing to the 3rd charecter in the file: \n");
    lseek(fd, 3, SEEK_SET); // Move to 3rd char from start
    n = read(fd, buffer, 5);
    write(1, buffer, n);

    return 0;
}

/*
myfile.txt:
this is my file. it has charecters. we use open syscall to get the 
fild discriptor, and use read, write and lseek to jump across the file.

Output:
File discriptor is 3
Reading first 10 charecters from file:
this is my
Skipping 5 charecters from current position in the file:
. it has c
Going 10 charecters before the current position in the file:
. it has c
Going to 5th last charecter in the file:
ile.
Going to the 3rd charecter in the file:
s is
*/