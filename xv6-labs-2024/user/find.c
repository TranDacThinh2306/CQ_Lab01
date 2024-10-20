#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

// Helper function to combine 2 string
char* strcat(char* dest, const char* src) {
    char *os = dest;

    while (*dest != '\0') {
        dest++; 
    }

    while ((*dest++ = *src++) != 0) {
    }

    return os;
}

// Find files in a directory tree with a specific name
void find(char *path, char *filename)
{
    char buf[512];
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type == T_DIR) {
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) {
                continue;
            }
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }

            // Create path by concatenating strings
            strcpy(buf, path);               
            if (path[strlen(path) - 1] != '/') {  // add '/' to path
                strcat(buf, "/");            
            }
            strcat(buf, de.name);            

            if (stat(buf, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", path);
                continue;
            }

            if (st.type == T_DIR) {
                find(buf, filename);  
            } else if (strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);  // Print path if file name matches
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
