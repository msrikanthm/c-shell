#include "functions.h"

char *bkground[10000000] = {NULL};
int exit_command()
{
    exit(0);
}
void background()
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    if (pid <= 0)
        return;

    if (status != 0)
        printf("\nProcess %s, with process id = %d, FAILED\n", bkground[pid], pid);
    else
        printf("\nProcess %s, with process id = %d, has exited\n", bkground[pid], pid);
}

int main()
{
    char present_dir[2000]; // the directory in which the command is being run at present
    char user_name[1000];   // user name
    char host_name[1000];   // host name
    command_count = 0;

    // commands to get directories, user and host names
    // home directory is the directory in which the code is present
    getcwd(home_dir, 1000);
    getlogin_r(user_name, 1000);
    gethostname(host_name, 1000);
    getcwd(dir_store, 1000);
    int time_taken = 0;

    // reading the previous commands into a array "command_store"
    // a file is created to store the commands as an array looses data after terminating the code once
    FILE *command_r = fopen("History.txt", "r");
    if (command_r == NULL)
    {
        // to create the file "History.txt" in case it isn't created earlier
        FILE *command_w = fopen("History.txt", "w");
        fclose(command_w);
    }
    else
    {
        int f_c_count = 0;
        char store[1000];
        while (fscanf(command_r, "%[^\n]%*c", store) != EOF)
        {
            strcpy(command_store[f_c_count], store); // reading the commands inot the array "command_store"
            f_c_count++;
        }
        command_count = f_c_count; // the count of the commands stored in the file "History.txt"
        fclose(command_r);
    }
    int last = -1;
    int symcheck = -1;

    for (;;)
    {

    loop:
        if (getcwd(present_dir, 2000) != NULL)
        {
            char *temp;
            temp = strstr(present_dir, home_dir);
            // to check if the present directry is inside our home directory
            // if yes, we print it as ~/...
            // else, we print its original path from home of our system /home/...
            printf("\033[0;35m");
            if (time_taken > 1 && symcheck == 0)
            {
                if (temp)
                    printf("<%s@%s:~%s took %ds>", user_name, host_name, temp + strlen(home_dir), time_taken);
                else
                    printf("<%s@%s:%s took %ds>", user_name, host_name, present_dir, time_taken);
            }
            else
            {
                if (temp)
                    printf("<%s@%s:~%s>", user_name, host_name, temp + strlen(home_dir));
                else
                    printf("<%s@%s:%s>", user_name, host_name, present_dir);
            }
            printf("\033[0m");
            symcheck = -1;
        }
        else
        {
            perror("error in reading current directory");
            exit(EXIT_FAILURE);
        }

        // to read the commands

        char input[3000];
        // fgets(input, 3000, stdin);
        scanf("%[^\n]%*c", input);
        redirection_check = 0;
        char inputs[3000];
        char redirect[3000];
        strcpy(inputs, input);
        strcpy(redirect, input);

        if (!input)
        {
            exit_command();
        }

        if (command_count != 20)
        {
            // if count is zero, it means this is the first time we are running the code
            // if the previous command is same as the current command, we shouldn't be storing it again
            if (command_count == 0 || strcmp(command_store[command_count - 1], input) != 0)
                strcpy(command_store[command_count++], input);
        }
        else
        {
            // since we only store 20 commands if count is > 20, we overwrite the commands
            // it is to be done only when we encounter a new command
            // so, we compare it with it's previous command

            if (strcmp(command_store[19], input) != 0)
            {
                for (int i = 1; i < 20; i++)
                    strcpy(command_store[i - 1], command_store[i]);

                strcpy(command_store[19], input);
            }
        }

        // we write the file with new set of 20 commands
        FILE *rewrite = fopen("History.txt", "w");
        for (int r = 0; r < command_count; r++)
            fprintf(rewrite, "%s\n", command_store[r]);

        fclose(rewrite);
        int fd1 = dup(1), fd2 = dup(0);

        char *check1, *check2, *check3, *check4;
        check1 = strstr(input, ">");
        check2 = strstr(input, "<");
        check3 = strstr(input, ">>");
        check4 = strstr(input, " | ");

        if (check1 != NULL || check2 != NULL || check3 != NULL || check4 != NULL)
        {
            strcpy(input, redirect_command(redirect));
            strcpy(inputs, input);
            redirection_check = 1;
            if (check4 != NULL)
            {
                pipelining_command(input);
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
                dup2(fd2, STDIN_FILENO);
                close(fd2);
                continue;
            }
        }

        char *input_parts[500];
        int i_start = 0;
        // we can give two or more commands at once seperating them by ; or &
        // so we execute each command seperately for which we use 'strtok' to break the original line into two commands
        input_parts[i_start] = strtok(input, "';','&'");

        while (input_parts[i_start] != NULL)
            input_parts[++i_start] = strtok(NULL, "';','&'");

        pid_t p, pi;
        int numb = 0;
        for (int i = 0; i < i_start; i++)
        {
            char *command_p[500];
            int j_start = 0;

            // we store the data of the command without space or tabs
            command_p[j_start] = strtok(input_parts[i], "' ','\t', '\n'");

            while (command_p[j_start] != NULL)
                command_p[++j_start] = strtok(NULL, "' ','\t', '\n'");

            // stores the commands
            char *functions[] = {"cd", "echo", "pwd", "pinfo", "exit", "history", "ls", "discover"};

            // if the command is same as function[.],then strcmp returns <0 / >0
            // so, if returns 0, we enter into the if statement
            if (strcmp(command_p[0], functions[0]) == 0)
                cd_command(command_p, j_start);
            else if (strcmp(command_p[0], functions[1]) == 0)
                echo_command(command_p, j_start);
            else if (strcmp(command_p[0], functions[2]) == 0)
                pwd_command(command_p, j_start);
            else if (strcmp(command_p[0], functions[3]) == 0)
                pinfo_command(command_p, j_start);
            else if (strcmp(command_p[0], functions[4]) == 0)
                exit_command();
            else if (strcmp(command_p[0], functions[5]) == 0)
                history_command(command_count);
            else if (strcmp(command_p[0], functions[6]) == 0)
                ls_command(command_p, j_start);
            else if (strcmp(command_p[0], functions[7]) == 0)
                discover_command(command_p, j_start);
            else
            {
                time_t t1, t2;
                t1 = time(0);

                char *input_partss[500] = {NULL};
                int i_starts = 1;

                if (inputs[strlen(inputs) - 1] == '&')
                    symcheck = 1;
                else
                    symcheck = 0;

                char *tempor = strrchr(inputs, '&');
                if (tempor)
                    last = 1;
                else
                    last = 0;

                // we can give two or more commands at once seperating them by ; or &
                // so we execute each command seperately for which we use 'strtok' to break the original line into two commands
                char *token1;
                token1 = strtok(inputs, "&");
                input_partss[0] = token1;

                while (token1 != NULL)
                {
                    token1 = strtok(NULL, "&");
                    if (token1 != NULL)
                    {
                        input_partss[i_starts++] = token1;
                    }
                }

                int i = i_starts - 1;
                int ch = 0;
                while ((i != -1) && (input_partss[i] != NULL))
                {
                    char *command_ps[500] = {"\0"};

                    // we store the data of the command without space or tabs
                    int j_starts = 0;
                    char *token2;
                    token2 = strtok(input_partss[i], " \t\n");
                    command_ps[j_starts++] = token2;

                    while (token2 != NULL)
                    {
                        token2 = strtok(NULL, " \t\n");
                        if (token2 != NULL)
                            command_ps[j_starts++] = token2;
                    }

                    signal(SIGCHLD, background);

                    p = fork();

                    if (p == 0)
                        execvp(command_ps[0], command_ps);
                    else
                    {
                        if (i_starts == 2 && last == 1 && ch == 0)
                            pi = p;
                        else if (i_starts == 1 && last == 0)
                            pi = p;
                        else
                        {
                            printf("[%d] %d\n", i_starts - i, p);
                            bkground[p] = command_ps[0];
                        }
                        i--;
                        i_starts--;
                    }

                    if (last == 0)
                    {
                        waitpid(pi, NULL, 0);
                    }
                    else if (last == 1 && ch == 0 && i_starts == 1)
                    {
                        waitpid(pi, NULL, 0);
                    }

                    ch = 2;
                }
                t2 = time(0);
                time_taken = t2 - t1;
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
                dup2(fd2, STDIN_FILENO);
                close(fd2);
                goto loop;
            }
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            dup2(fd2, STDIN_FILENO);
            close(fd2);
        }
    }
}