#include "functions.h"

int history_command(int count)
{
    // since we only have to print to print 10 commands, I shall declare a starting index
    int start;
    if (command_count > 9)
        start = command_count - 10;
    else
        start = 0;

    for (int x = start; x < command_count; x++)
        printf("%s\n", command_store[x]);
}