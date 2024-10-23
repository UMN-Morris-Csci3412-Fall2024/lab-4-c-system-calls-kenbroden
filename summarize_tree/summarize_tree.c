#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

static int num_dirs, num_regular;

bool is_dir(const char* path) {
    /*
     * Use the stat() function (try "man 2 stat") to determine if the file
     * referenced by path is a directory or not. Call stat, and then use
     * S_ISDIR to see if the file is a directory. Make sure you check the
     * return value from stat() in case there is a problem, e.g., maybe the
     * the file doesn't actually exist.
     *
     * You'll need to allocate memory for a buffer you pass to stat(); make
     * sure you free up that space before you return from this function or
     * you'll have a substantial memory leak (think of how many times this
     * will be called!).
     */
    struct stat buf;
    if (stat(path, &buf) != 0) {
        perror("stat");
        exit(1);
    }
    return S_ISDIR(buf.st_mode);
}

/*
 * This is necessary this because the mutual recursion means there's no way to
 * order them so that all the functions are defined before they're called.
 */
void process_path(const char* path);

void process_directory(const char* path) {
    /*
     * Update the number of directories seen, use opendir() to open the
     * directory, and then use readdir() to loop through the entries
     * and process them. You have to be careful not to process the
     * "." and ".." directory entries, or you'll end up spinning in
     * (infinite) loops. Also make sure you closedir() when you're done.
     *
     * You'll also want to use chdir() to move into this new directory,
     * with a matching call to chdir() to move back out of it when you're
     * done.
     */
    num_dirs++;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char new_path[PATH_MAX];
            snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
            process_path(new_path);
        }
    }
    closedir(dir);
}

void process_file(const char* path) {
    /*
     * Update the number of regular files.
     * This is as simple as it seems. :-)
     */
    num_regular++;
}

void process_path(const char* path) {
    if (is_dir(path)) {
        process_directory(path);
    } else {
        process_file(path);
    }
}

int main(int argc, char* argv[]) {
    // Ensure an argument was provided.
    if (argc != 2) {
        printf("Usage: %s <path>\n", argv[0]);
        printf("       where <path> is the file or root of the tree you want to summarize.\n");
        return 1;
    }

    num_dirs = 0;
    num_regular = 0;

    process_path(argv[1]);

    printf("There were %d directories.\n", num_dirs);
    printf("There were %d regular files.\n", num_regular);

    return 0;
}