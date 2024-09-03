#include "functions.h"



int echo_command(char **command, int index)
{
    // if the command 'echo' is alone, then we print an empty line
    if (index == 1)
    {
        printf("\n");
        return 0;
    }
    // x starts from 0 if echo is to be printed
    // if not, x starts from 1
    // since we remove spaces and read the data into seperate parts,
    // we now print them one by one with a space seperating them
    for (int x = 1; x < index - 1; x++)
        printf("%s ", command[x]);
    printf("%s\n", command[index - 1]);
    return 0;
}

int pwd_command(char **command, int index)
{
    // pwd command prints the present directory on the terminal
    // so, we read it using 'getcwd' and print it on the terminal
    char directory[1000];
    getcwd(directory, 1000);
    printf("%s\n", directory);
    return 0;
}