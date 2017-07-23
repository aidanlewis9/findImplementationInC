/* utilities.c */

#include "search.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Test whether or not a directory is empty.
 * @param   path        Path to directory.
 * @return  Whether or not a directory is empty.
 */
bool        is_directory_empty(const char *path) {
    struct dirent *dp;
    struct stat s;
    if (stat(path, &s) && !S_ISDIR(s.st_mode))
    {
        fprintf(stderr, "%s is not a directory: %s\n", path, strerror(errno));
        return false;
    }
    DIR *dirp = opendir(path);
    if (dirp == NULL)
    {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        return false;
    }
    dp = readdir(dirp);
    dp = readdir(dirp);
    if ((dp = readdir(dirp)) == NULL)
    {
        closedir(dirp);
        return true;
    }
    closedir(dirp);
    return false;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t      get_mtime(const char *path) {
    if (access(path, F_OK) == -1)
    {
        perror("File does not exist");
        return 0;
    }
    struct stat s;
    lstat(path, &s);
    return s.st_mtime;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
