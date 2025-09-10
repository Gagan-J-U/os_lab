/* 
   Program to simulate the grep command of the Linux operating system.
   It finds the required pattern in the given file, and prints the lines containing the pattern.
*/

#include <stdio.h>
#include <string.h>

void main() {
    char fn[10], pat[10], temp[200];  // fn = file name, pat = pattern, temp = line buffer
    FILE *fp;

    printf("Enter file name\n");
    scanf("%s", fn);

    printf("Enter pattern to be searched\n");
    scanf("%s", pat);

    fp = fopen(fn, "r");  // Open the file in read mode

    // Check if file is successfully opened
    if (fp == NULL) {
        printf("File not found or unable to open.\n");
        return;
    }

    // Loop until end of file
    while (!feof(fp)) {
        // fgets() reads a line (up to 99 characters) from the file into 'temp'
        fgets(temp, 100, fp);

        // strstr() checks if 'pat' exists within the line 'temp'
        if (strstr(temp, pat))
            printf("%s", temp);  // Print the line containing the pattern
    }

    fclose(fp);  // Close the file
}