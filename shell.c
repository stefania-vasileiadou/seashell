#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "shell.h"

/* Global variables */
FILE *fileid;

void init_shell()
{
    clear(); /* Clear the terminal */

    /* Print greeting */
    printf("       /\\\n");
    printf("      {.-}\n");
    printf("     ;_.-'\\        stefania-vasileiadou/seashell\n");
    printf("    {    _.}_      UNIX shell written in C\n");
    printf("     \\.-' /  `,\n");
    printf("      \\  |    /\n");
    printf("       \\ |  ,/     Use at your own risk...\n");
    printf("   svcs \\|_/\n\n");

    /* Open a file for session history */
    fileid = fopen(HISTORY_FILE, "w");
}

int main()
{
    init_shell();

    char cmdline[MAXLINE];

    while (1)
    {
        printf("seashell> ");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
        {
            fclose(fileid);
            unlink(HISTORY_FILE);
            exit(0);
        }

        eval(cmdline);
    }
}

void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    fputs(cmdline, fileid);   /* Update command-line history */
    fflush(fileid); /* Forces a write of all buffered data for the output file */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    if (argv[0] == NULL)
        return; /* Ignore empty lines */

    if (!builtin_command(argv))
    {
        if ((pid = fork()) == 0) /* child proc runs user job */
        { 
            if (execvp(argv[0], argv) < 0)
            {
                printf("%s: command not found.\n", argv[0]);
                exit(0);
            }
        }
        if (!bg)
        {
            int status;
            if (waitpid(pid, &status, 0) < 0) /* parent waits for foreground child to terminate */
            {
                char *msg = "waitfg: waitpid error";
                int errnum = errno;
                fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));
                exit(EXIT_FAILURE);
            }
        }
        else
            printf("%d %s", pid, cmdline);
    }

    return;
}

int parseline(char *buf, char **argv)
{
    char *delim; /* Points to first space delimiter */
    int argc;    /* Number of args */
    int bg;      /* Background job? */

    buf[strlen(buf) - 1] = ' ';   /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) /* quit command */
    {
        fclose(fileid);
        unlink(HISTORY_FILE);
        exit(0);
    }
    if (!strcmp(argv[0], "&")) /* Ignore singleton & */
        return 1;

    return 0; /* Not a builtin command */
}
