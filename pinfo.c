#include "functions.h"

int pinfo_command(char **command, int index)
{
    int process_ID;
    // that is , when command[1] is not empty, it means that a process id is given
    if (command[1] == NULL)
    {
        process_ID = getpid(); // reading the process ID
    }
    else
    {
        process_ID = atoi(command[1]); // reading the process ID
    }

    char fname[150];
    char ep[1000];
    printf("pid : %d\n", process_ID);

    sprintf(fname, "/proc/%d/stat", process_ID); // file path for status
    FILE *f = fopen(fname, "r");
    if (f == NULL)
    {
        write(STDERR_FILENO, "Process is not found\n", 22);
        return 0;
    }
    long long int a = -1;
    long long int b = -1;
    char status;
    fscanf(f, "%*s %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s", &status, &a, &b);
    if (a == b)
        printf("Process Status : %c+\n", status);
    else
        printf("Process Status : %c\n", status);
    fclose(f);

    sprintf(fname, "/proc/%d/statm", process_ID); // file path for memory
    FILE *mem_f = fopen(fname, "r");
    int memory;
    fscanf(f, "%d", &memory);
    if (memory)
        fprintf(stdout, "Memory : %d\n", memory);
    fclose(mem_f);

    sprintf(fname, "/proc/%d/exe", process_ID); // file path for executable path
    readlink(fname, ep, 1000);

    char *temp;
    temp = strstr(ep, home_dir); // to check if path is inside the homedirectory

    if (temp)
    {
        printf("Executable Path : ~%s\n", temp + strlen(home_dir));
    }
    else
        printf("Executable Path : %s\n", ep);

    return 0;
}