#include "functions.h"

char *redirect_command(char *input_)
{
    char *commandline;
    char *command_r[500];
    int j_start = 0;
    int input_r = -1, output_r = -1, check_in_r = 0, check_out_r = 0, pipe = -1, pipe_r = -1;
    int append_check = -1;
    int pipe_check = -1;

    command_r[j_start] = strtok(input_, " ");

    while (command_r[j_start] != NULL)
    {
        // we can only have one >/</>> in a given command
        if (strcmp(command_r[j_start], "<") == 0)
        {
            if (check_in_r == 1)
            {
                printf("syntax error\n");
                return "";
            }
            check_in_r = 1;
            input_r = j_start;
        }
        else if (strcmp(command_r[j_start], ">") == 0)
        {
            if (check_out_r == 1)
            {
                printf("syntax error\n");
                return "";
            }
            check_out_r = 1;
            output_r = j_start;
        }
        else if (strcmp(command_r[j_start], ">>") == 0)
        {
            if (check_out_r == 1)
            {
                printf("syntax error\n");
                return "";
            }
            append_check = 0;
            check_out_r = 1;
            output_r = j_start;
        }
        else if (strcmp(command_r[j_start], "|") == 0 && pipe_check == -1)
        {
            pipe = j_start;
            pipe_check = 0;
        }
        else if (strcmp(command_r[j_start], "|") == 0)
            pipe_r = j_start;

        command_r[++j_start] = strtok(NULL, " ");
    }
    if (output_r == -1)
        output_r = j_start + 1;
    if (pipe == -1)
        pipe = j_start + 1;

    if (j_start + 1 >= 20)
    {
        printf("too many commands\n");
        return " ";
    }

    // first | should be after <, last | should be before >, < should be before >
    if (pipe < input_r || pipe_r > output_r || input_r > output_r)
    {
        printf("syntax error\n");
        return "";
    }

    if (output_r < j_start)
    {
        int f;

        if (append_check == 0)
            f = open(command_r[output_r + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            f = open(command_r[output_r + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (f < 0)
            printf("Error in opening file\n");

        if (dup2(f, STDOUT_FILENO) < 0)
        {
            perror("Unable to duplicate file descriptor.");
            close(f);
            return "";
        }
        close(f);
    }
    else if (output_r == j_start || input_r == 0)
        return "";
    // when > is at last, we dont have a file, when < is at satrt we don't have data

    char *command = malloc(3000);
    strcpy(command, "");
    if (output_r == (j_start + 1))
        output_r = j_start;
    for (int i = 0; i < output_r; i++)
    {
        if (strcmp(command_r[i], "<") != 0)
            strcat(command, command_r[i]);
        strcat(command, " ");
    }

    return command;
}