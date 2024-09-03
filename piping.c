#include "functions.h"

void pipelining_command(char *input_)
{
    char *input_parts[500];
    int i_start = 0;
    int start = 0;
    input_parts[i_start] = strtok(input_, "|");

    while (input_parts[i_start] != NULL)
        input_parts[++i_start] = strtok(NULL, "|");

    int file_descriptors[2], descriptor1 = 0;

    for (int i = 0; i < i_start; i++)
    {
        if(i == i_start-1)
            start = 1;
        
        int check = pipe(file_descriptors);
        if (check < 0)
        {
            perror("error");
            return;
        }

        int pid = fork();
        if (pid == 0)
        {
            if (descriptor1 != 0)
            {
                dup2(descriptor1, 0);
                close(descriptor1);
            }
            if (file_descriptors[1] != 1 && start == 0)
            {
                dup2(file_descriptors[1], 1);
                close(file_descriptors[1]);
            }
            char *command_p[500];
            int j_start = 0;

            // we store the data of the command without space
            command_p[j_start] = strtok(input_parts[i], " ");

            while (command_p[j_start] != NULL)
                command_p[++j_start] = strtok(NULL, " ");

            command_p[++j_start] = NULL;

            if(strcmp(command_p[0],"pinfo")==0)
                pinfo_command(command_p, j_start);
            else
                execvp(command_p[0], command_p);

            exit(0);
        }
        else
        {
            int s;
            waitpid(pid, &s, 0);
        }
        descriptor1 = file_descriptors[0];
        close(file_descriptors[1]);
    }
}