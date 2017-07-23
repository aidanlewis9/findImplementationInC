/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int	    search(const char *root, const Settings *settings) {
    struct dirent *dp;
    DIR *dirp = opendir(root);
    if (dirp == NULL)
    {
        return 1;
    }
    while ((dp = readdir(dirp)) != NULL)
    {
        char newpath[strlen(root) + strlen(dp->d_name) + 2];
        sprintf(newpath,"%s/%s", root, dp->d_name);
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {
            continue;
        }
        if (!filter(newpath, settings))
        {
            if (settings->exec_argc > 0)
            {
                execute(newpath, settings);
            }
            else
            {
                printf("%s\n", newpath);
            }
        }
        struct stat s;
        if (0 == lstat(newpath, &s) && (S_ISDIR(s.st_mode)))
        {
            search(newpath, settings);
        }
    }
    closedir(dirp);
    
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
