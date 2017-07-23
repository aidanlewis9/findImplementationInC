/* driver.c */

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Functions */

void	    usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s PATH [OPTIONS] [EXPRESSION]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -executable     File is executable or directory is searchable to user\n");
    fprintf(stderr, "    -readable       File readable to user\n");
    fprintf(stderr, "    -writable       File is writable to user\n\n");
    fprintf(stderr, "    -type [f|d]     File is of type f for regular file or d for directory\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -name  pattern  Base of file name matches shell pattern\n");
    fprintf(stderr, "    -path  pattern  Path of file matches shell pattern\n\n");
    fprintf(stderr, "    -perm  mode     File's permission bits are exactly mode (octal)\n");
    fprintf(stderr, "    -newer file     File was modified more recently than file\n\n");
    fprintf(stderr, "    -uid   n        File's numeric user ID is n\n");
    fprintf(stderr, "    -gid   n        File's numeric group ID is n\n");
    fprintf(stderr, "\nExpressions:\n\n");
    fprintf(stderr, "    -print          Display file path (default)\n");
    fprintf(stderr, "    -exec cmd {} ;  Execute command on path\n");
    exit(status);
}

/* Main Execution */

int	    main(int argc, char *argv[]) {
    int argind = 1;
    Settings settings = {
        .access = 0,
        .uid    = -1,
        .gid    = -1,
    };
    
    const char *path_name = argv[argind++];
    
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-')
    {
        char *arg = argv[argind++];
        if (arg[1] == 'h') 
        {
            usage(argv[0], 0);
        }
        else if (streq(arg, "-executable")) 
        {
            settings.access |= X_OK;   
        }    
        else if (streq(arg, "-readable")) 
        {
            settings.access |= R_OK;   
        }    
        else if (streq(arg, "-writable")) 
        {
            settings.access |= W_OK;   
        }    
        else if (streq(arg, "-type")) 
        {
            settings.type = argv[argind++][0];   
        }    
        else if (streq(arg, "-empty")) 
        {
            settings.empty = true;   
        }
        else if (streq(arg, "-name")) 
        {
            settings.name = argv[argind++];   
        }
        else if (streq(arg, "-path")) 
        {
            settings.path = argv[argind++];   
        }
        else if (streq(arg, "-perm")) 
        {
            settings.perm = atoi(argv[argind++]);   
        }
        else if (streq(arg, "-newer"))
        {
            settings.newer = get_mtime(argv[argind++]);
        }
        else if (streq(arg, "-uid"))
        {
            settings.uid = atoi(argv[argind++]);
        }
        else if (streq(arg, "-gid"))
        {
            settings.gid = atoi(argv[argind++]);
        }
        else if (streq(arg, "-print"))
        {
            settings.print = true;    
        }
        else if (streq(arg, "-exec"))
        {
            settings.exec_argv = malloc(sizeof(char *) * (argc - argind));
            while (argind < argc - 1 && strlen(argv[argind]) > 1)
            {
                settings.exec_argv[settings.exec_argc++] = argv[argind++];
            }
            settings.exec_argv[settings.exec_argc] = NULL;
        }
        else
        {
            usage(argv[0], 1);
        }
    }
    if (!filter(path_name, &settings))
    {
        printf("%s\n", path_name);
    }

    search(path_name, &settings);
    if (settings.exec_argv)
    {
        free(settings.exec_argv);
    }

    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
