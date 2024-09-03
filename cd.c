#include "functions.h"


int cd_command(char **command, int index)
{
    char p_dir[1000]; // present directory while executing 'cd' command
    getcwd(p_dir, 1000);

    if (index != 1)
    {
        char *t1 = strstr(command[1], "~"); // for '~/'
        char *t3 = strstr(command[1], "."); // for './' and '../'
        char *t2;
        if (t1)
        {
            // if '~' is found, we then check for '/'
            t2 = strstr(command[1], "/");
            if (t2)
            {
                // if '/' is also found,
                // we split the command into two parts
                //                                      cd ~
                //                                      cd path
                char *path;
                path = t2 + strlen("~/") - 1;
                command[1] = "~";
                cd_command(command, 2); // 'cd ~'
                command[1] = path;
                cd_command(command, 2);   // 'cd path'
                strcpy(dir_store, p_dir); // this line is for '-' command
                return 0;
            }
        }
        if (t3)
        {
            char c = '.';
            if (c == command[1][1]) // for '../'
            {
                // if command[1][1] is '.', it represents that it is ..
                t2 = strstr(command[1], "/");
                if (t2)
                {
                    // if '/' is also found,
                    // we split the command into two parts
                    //                                      cd ..
                    //                                      cd path
                    char *path;
                    path = t2 + strlen("../") - 2;
                    command[1] = "..";
                    cd_command(command, 2); // 'cd ..'
                    command[1] = path;
                    cd_command(command, 2);   // 'cd path'
                    strcpy(dir_store, p_dir); // this line is for '-' command
                    return 0;
                }
            }
            else
            {
                t2 = strstr(command[1], "/");
                if (t2)
                {
                    // if '/' is also found,
                    // we split the command into two parts
                    //                                      cd .
                    //                                      cd path
                    char *path;
                    path = t2 + strlen("./") - 1;
                    command[1] = ".";
                    cd_command(command, 2); // 'cd .'
                    command[1] = path;
                    cd_command(command, 2);   // 'cd path'
                    strcpy(dir_store, p_dir); // this line is for '-' command
                    return 0;
                }
            }
        }
    }

    // cd command can only have one path
    // cd + path(optinal) we should have the index as 1 or 2
    if (index > 2)
        perror("issue in cd command");
    else if (index == 1 || !strcmp(command[1], "~"))
    {
        // if index is 1, it is 'cd' which is same as 'cd ~'
        // this command directs us to the home directory which is stored in 'home_dir'
        strcpy(dir_store, p_dir); // this line is for '-' command
        chdir(home_dir);          // we direct into that path using 'chdir'
    }
    else if (!strcmp(command[1], "."))
    {
        // this command changes the present directory into current working directory
        strcpy(dir_store, p_dir); // this line is for '-' command
        chdir(p_dir);             // we direct into that path using 'chdir'
    }
    else if (!strcmp(command[1], "-"))
    {
        // for this command, i have stored the directory while executing the previous command into 'dir_store'
        chdir(dir_store);          // we direct into that path using 'chdir'
        printf("%s\n", dir_store); // it also has to be printed onto the terminal
        strcpy(dir_store, p_dir);  // this line is for '-' command
    }
    else if (!strcmp(command[1], ".."))
    {
        // for '..',
        // we move to the directory one step beyond the present directory
        strcpy(dir_store, p_dir); // this line is for '-' command
        // since we need the beyond directory, we search for the last '/ in the path of our present directory
        // we then store the path of the required directory into 'dir'
        char *temp = strrchr(p_dir, '/');
        char dir[1000];
        if (temp)
        {
            int x;
            for (x = 0; x < (temp - p_dir); x++)
                dir[x] = p_dir[x];

            dir[x] = '\0';
        }
        chdir(dir); // we direct into 'dir' using 'chdir'
    }
    else if (chdir(command[1]) == 0)
    {
        // if the command is cd 'path', we direct into that path using 'chdir'
        strcpy(dir_store, p_dir); // this line is for '-' command
        chdir(command[1]);
    }
    else
        perror("issue in cd command");

    return 0;
}