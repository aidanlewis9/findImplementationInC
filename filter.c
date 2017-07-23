/* filter.c */

#include "search.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */
bool        filter(const char *path, const Settings *settings) {
    struct stat s;
    lstat(path, &s);
    if (settings->type == 102 && S_ISREG(s.st_mode) == 0)
    {
        return true;
    }
    else if (settings->type == 100 && S_ISDIR(s.st_mode) == 0)
    {
        return true;
    }
    // check access
    if (settings->access && access(path, settings->access) != 0)
    {
        //printf("FILTER: 3\n");
        return true;
    }
    // check empty
    if (settings->empty && ((S_ISLNK(s.st_mode) != 0) || (S_ISREG(s.st_mode) && s.st_size != 0) || (S_ISDIR(s.st_mode) && !is_directory_empty(path))))
    {
        return true;
    }
    // check name/pattern matching
    if (settings->name != NULL && fnmatch(settings->name, basename((char *)path), 0) != 0) //might need something for flag argument
    {
        return true;  
    }

    if (settings->path != NULL && fnmatch(settings->path, path, 0) != 0)
    {
        return true;
    }

    // check permissions
    char buf[BUFSIZ]; // this might f things up
    sprintf(buf, "%o", s.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    if (settings -> perm && settings->perm != atoi(buf))
    {
        return true;
    }

    // check newer - is settings->newer a file or is the file in an argument??
    if (settings->newer && (long)get_mtime(path) - (long)settings->newer <= 0)
    {
        return true;
    }
   
    // check uid - possibly uint not necessary
    if (settings->uid != -1 && (unsigned int)settings->uid != s.st_uid)
    {
        return true;
    }

    // check gid
    if (settings->gid != -1 && (unsigned int)settings->gid != s.st_gid)
    {
        return true;
    }
    return false;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
