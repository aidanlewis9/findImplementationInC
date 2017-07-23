Project 01 - README
===================

Members
-------

- Jessica Hardey (jhardey@nd.edu)
- Aidan Lewis (alewis9@nd.edu)

Brief Summary
-------

With this project, we reimplemented the find command in shell using C code.

Responses
---------

**1.** How you handled parsing the command line options.

We parsed command line arguments with a while loop that ran while the current argument index was less than the total numbers arguments (when each argument had a length greater than 1, and when each started with '-' to indicate it was a flag). With a set of if statements, we then string compared the flag to each of the available flags, and if the comparison returned that the two strings were equal, we set the corresponding variable. For flags that were followed by a string, we set the corresponding variable equal to this string. The special case was the -exec flag. In this case, we had a nested while loop that checked the arguments following -exec, which would be the command the user wished to execute. We stored these arguments in a special array (settings.exec_argv) with strdup, and correspondingly increased settings.exec_argc with each additional argument.  

**2.** How you walked the directory tree.

We walked the directory tree by first opening the directory corresponding to a path, and checking to ensure it was not equal to NULL. We return if it is equal to NULL. Next, we use a while loop to go through the contents of the directory. The while runs while the readdir for the directory is not equal to NULL. After, a new path char[] is set with a length equal to that of the current root plus the current file's name plus 2 (1 for '/', and 1 for the null character). This newpath is then set using sprintf to the root followed by a '/' followed by the new file name. Next, string compare is used to check if the current file name is equal the current or parent directory (. or ..). In this case, the remainder of the loop is skipped, since we do not wish to check this. After, filter and possibly execute are run on the newpath. Finally, lstat is used to check if the newpath is a directory, and if it is, search is called recursively on it so that the files inside this directory may be checked.

**3.** How you determined whether or not to print a filesystem objects path.

For determining whether or not to print, we always print if the -exec flag is not used. More specifically, if settings->exec_argc is not greater than 0 (meaning exec was not used), we print the newpath. If settings->exec_argc is greater than 0 (meaning the -exec flag is used), then the print is handled in execute. In the execute function, if settings->print is set to true (meaning the -print flag has been used), the path is printed. Otherwise, (so if settings->print is not set to true, meaning the -print flag has not been used), the path will not be printed out if execute is specified.

**syscalls.py:**

In general, find uses more system calls than search does. It uses 10 more system calls than search with both "/etc" and "/etc -exec echo \{\} ;". For "/etc", find uses nearly double the amount of open calls, however search uses over 7000 calls to lstat whereas find does not use lstat at all. Rather, find uses newfstatat 714 times. The find command uses open nearly 800 times whereas our search program uses open around 400 times. The same is true for the close command. There are many calls that find uses that search does not including fchdir, rt_sigaction, ioctl, futex, uname, rt_sigprocmask, getrlimit, set_robust_list, set_tid_address, statfs. Overall the find command has a lot more system calls than search. 

For the "/etc -exec echo \{\} \;" test find also has a lot more calls overall and generally each call is done more times with find. This holds true except for with lstat which search call almost twice as much as find.

I am surprised that find has more system calls than search. I would think that the built in command would be more streamlined than a program we could write. Maybe this is because find has more capabilities beyond search and thus requires more system calls at a baseline level. But I would expect find to have fewer system calls and fewer calls to each system call than it did.

Contributions
-------------

Jessica Hardey:
  filter.c search.c main.c Makefile README.md

Aidan Lewis:
  utilities.c search.c main.c Makefile README.md syscalls.py

Programs listed twice were worked on collaboratively.

