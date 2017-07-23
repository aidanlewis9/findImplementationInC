/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int	    execute(const char *path, const Settings *settings) {
    int i = 0;
    if (settings->print)
    {
        printf("%s\n", path);
    }
    while (i < settings->exec_argc)
    {
        if (streq(settings->exec_argv[i], "{}"))
        {
            settings->exec_argv[i] = (char *) path;
            break;
        }
        i++;
    }
    pid_t child = fork();
    if (child < 0)
    {
        fprintf(stderr, "Fork failed: %s\n", strerror(errno));
        return -1;
    }
    else if (child == 0)
    {
        execvp(settings->exec_argv[0], settings->exec_argv);
        perror("Command does not work");
        return -1;
    }
    else
    {
        int child_status;
        wait(&child_status);
        settings->exec_argv[i] = "{}";
        return 0;
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
