#include "functions.h"

int function(const void *a, const void *b)
{
    const char **_a = (const char **)a;
    const char **_b = (const char **)b;
    return strcasecmp(*_a, *_b);
}
int ls_command(char **command, int index)
{
    // to check what all commands are given
    char ls[8][10];
    strcpy(ls[0], ".");
    strcpy(ls[1], "..");
    strcpy(ls[2], "~");
    strcpy(ls[3], "-a");
    strcpy(ls[4], "-l");
    strcpy(ls[5], "-al");
    strcpy(ls[6], "-la");
    strcpy(ls[7], "-a -l");

    // stores '1' if, that particular command is being called according to the above taken indexes
    int ls_store[] = {0, 0, 0, 0, 0, 0, 0, 0};
    struct dirent *d;
    DIR *directory;
    for (int i = 1; i < index; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!strcmp(command[i], ls[j]))
                ls_store[j] = 1;
        }
    }

    if (index == 1)
        ls_store[0] = 1;
    char *ls_commands[4];
    ls_commands[0] = ".";                              // for command 'ls .'
    ls_commands[1] = "..";                             // for command 'ls ..'
    ls_commands[3] = ".";                              // for command 'ls -a'
    ls_commands[2] = (char *)malloc(strlen(home_dir)+1); // for command 'ls ~'
    strcpy(ls_commands[2], home_dir);
    int x = 0;
    int check = 0;

    // the following loop handles 'ls .' & 'ls ..' & 'ls ~'
    for (int x = 0; x < 4; x++)
    {
        if ((ls_store[x] == 1 && index == 2) || (ls_store[x] == 1 && x == 0))
        {
            struct stat f_ = {0};
            directory = opendir(ls_commands[x]);
            // opening the required direcory
            if (directory == NULL)
            {
                printf("Cannot open the directory\n");
                return 0;
            }
            char *stored_names[100];
            int file_count = 0;

            // reading all the files/directories present in the present directory and storing in 'stored_names'
            while ((d = readdir(directory)) != NULL)
            {
                if (x == 3 || d->d_name[0] != '.')
                {
                    stored_names[file_count++] = d->d_name;
                }
            }
            closedir(directory);

            // sorting the file names
            qsort(stored_names, file_count, sizeof(char *), function);

            // giving the path for printing names with colour, and that needs to be checked if it is a file/folder/executable file
            char paths_5[1000];
            char paths_6[2000];

            if (strcmp(ls[x], "..") == 0)
            {

                getcwd(paths_5, sizeof(paths_5));
                char *p = strrchr(paths_5, '/');
                if (p)
                {
                    int g;
                    for (g = 0; g < (p - paths_5); g++)
                        paths_6[g] = paths_5[g];

                    paths_6[g] = '\0';
                }
                else
                    return 0;
                strcat(paths_6, "/");
            }
            else if (strcmp(ls[x], "~") == 0)
            {
                strcpy(paths_6, home_dir);
                strcat(paths_6, "/");
            }
            else
            {
                getcwd(paths_5, 1000);
                strcpy(paths_6, paths_5);
                strcat(paths_6, "/");
            }

            for (int i = 0; i < file_count; i++)
            {
                char dir_3[1000];
                strcpy(dir_3, paths_6);
                strcat(dir_3, stored_names[i]);
                ls_print(dir_3, stored_names[i]);

            }
            check = -1;
            return 0;
        }
    }
    int c = 0;
    if (index == 2 && ls_store[4] == 1)
        c = 4;
    else if ((index == 2 && ls_store[5] == 1) || (index == 2 && ls_store[6] == 1) || (index == 3 && ls_store[4] == 1 && ls_store[3] == 1))
        c = 5;

    if (check == 0 && (c == 4 || c == 5))
    {
        directory = opendir(".");
        if (directory == NULL)
        {
            printf("Cannot open current directory\n");
            return 0;
        }
        long long int total_files = 0;
        int file_count = 0;
        char *stored_names[100];
        while ((d = readdir(directory)) != NULL)
        {
            char paths_1[1000];
            char *paths_2;
            paths_2 = getcwd(paths_1, 1000);
            char dir_1[1000];
            strcat(paths_2, "/");
            strcpy(dir_1, paths_2);
            strcat(dir_1, d->d_name);
            struct stat directory = {0};

            if (c == 5 || d->d_name[0] != '.')
            {
                if (stat(dir_1, &directory) == 0)
                    total_files += directory.st_blocks;

                stored_names[file_count++] = d->d_name;
            }
        }
        printf("total %lld\n", total_files / 2);
        // file_count++;

        qsort(stored_names, file_count, sizeof(char *), function);

        char paths_3[1000];
        char *paths_4;
        paths_4 = getcwd(paths_3, 1000);
        char dir_2[1000];
        strcat(paths_4, "/");

        for (int i = 0; i < file_count; i++)
        {
            char temp[100];
            strcpy(temp, stored_names[i]);
            strcpy(dir_2, paths_4);
            strcat(dir_2, temp);
            ls_per_print(dir_2, temp);
        }

        closedir(directory);
        return 0;
    }
    else if (index == 3 || index == 2)
    {
        // for ls <flag> <directory/file>
        int x_ = 2;
        if (index == 3)
            x_ = 3;

        struct stat f_ = {0};
        int k = 0;
        if (x_ == 3)
        {
            if (strcmp(command[1], "-l") == 0)
                k = 1;
            else if (strcmp(command[1], "-a") == 0)
                k = 2;
        }

        directory = opendir(command[x_ - 1]);

        if (directory == NULL)
        {
            FILE *command_r = fopen(command[x_ - 1], "r");
            char path[1000];
            getcwd(path, sizeof(path));
            strcat(path, "/");
            strcat(path, command[x_ - 1]);
            if (command_r != NULL && k == 1)
                ls_per_print(path, command[x_ - 1]);
            else if (command_r != NULL && (k == 2 || x_ == 2))
                printf("%s\n", command[x_ - 1]);
            else
                printf("no such directory or file exists\n");

            fclose(command_r);
            return 0;
        }
        char *stored_names[100];
        int file_count = 0;
        long long int total_files = 0;
        char paths_1[1000];
        char *paths_2;
        getcwd(paths_1, 1000);
        char *temp;
        temp = strstr("/home/keerthi", paths_1);
        if (temp)
        {
            paths_2 = paths_1;
            strcat(paths_2, "/");
            strcat(paths_2, command[x_ - 1]);
        }
        else
            paths_2 = command[x_ - 1];

        char dir_1[1000];
        strcat(paths_2, "/");

        while ((d = readdir(directory)) != NULL)
        {

            strcpy(dir_1, paths_2);
            strcat(dir_1, d->d_name);
            struct stat directory = {0};

            if (k == 2 || d->d_name[0] != '.')
            {
                if (stat(dir_1, &directory) == 0)
                    total_files += directory.st_blocks;

                stored_names[file_count++] = d->d_name;
            }
        }
        if (k == 1 && index == 3)
            printf("total %lld\n", total_files / 2);

        // file_count++;

        // sorting the file names
        qsort(stored_names, file_count, sizeof(char *), function);

        char paths_5[1000];
        char *paths_6;
        getcwd(paths_5, 1000);
        char *temp1;
        temp1 = strstr("/home/keerthi", paths_5);
        if (temp1)
        {
            paths_6 = paths_5;
            strcat(paths_6, "/");
            strcat(paths_6, command[x_ - 1]);
        }
        else
            paths_6 = command[x_ - 1];

        strcat(paths_6, "/");

        for (int i = 0; i < file_count; i++)
        {
            char temp[100];
            strcpy(temp, stored_names[i]);

            char dir_3[1000];
            strcpy(dir_3, paths_6);
            strcat(dir_3, temp);
            if (k == 1 && index == 3)
                ls_per_print(dir_3, temp);
            else
            {
                ls_print(dir_3, stored_names[i]);
            }
        }
        closedir(directory);
    }
    else if (index == 6)
    {

        if (strcmp(command[2], "-l") != 0)
            return 0;
        else if (strcmp(command[4], "-a") != 0)
            return 0;

        FILE *command_r = fopen(command[5], "r");
        char path[1000];
        getcwd(path, sizeof(path));
        strcat(path, "/");
        strcat(path, command[1]);
        if (command_r != NULL)
            ls_per_print(path, command[5]);
        else
            printf("no such file exists\n");
        fclose(command_r);

        for (int y = 0; y < 2; y++)
        {
            struct stat f_ = {0};
            directory = opendir(command[(y * 2) + 1]);

            if (directory == NULL)
            {
                // check if file exists then print file name
                FILE *command_r = fopen(command[(y * 2) + 1], "r");
                if (command_r != NULL)
                    printf("%s\n", command[y * 2 + 1]);
                else
                    printf("no such directory or file exists\n");

                fclose(command_r);
                return 0;
            }
            char *stored_names[100];
            int file_count = 0;

            long long int total_files = 0;
            char paths_1[1000];
            char *paths_2;
            getcwd(paths_1, 1000);
            char *temp1;
            temp1 = strstr("/home/keerthi", paths_1);
            if (temp1)
            {
                paths_2 = paths_1;
                strcat(paths_2, "/");
                strcat(paths_2, command[(y * 2) + 1]);
            }
            else
                paths_2 = command[(y * 2) + 1];

            char dir_1[1000];
            strcat(paths_2, "/");

            while ((d = readdir(directory)) != NULL)
            {

                strcpy(dir_1, paths_2);
                strcat(dir_1, d->d_name);
                struct stat directory = {0};

                if (stat(dir_1, &directory) == 0)
                    total_files += directory.st_blocks;

                stored_names[file_count++] = d->d_name;
            }
            printf("%s:\n", command[(y * 2) + 1]);
            printf("total %lld\n", total_files / 2);

            // sorting the file names
            qsort(stored_names, file_count, sizeof(char *), function);

            char paths_5[1000];
            char *paths_6;
            getcwd(paths_5, 1000);
            char *temp;
            temp = strstr("/home/keerthi", paths_5);
            if (temp)
            {
                paths_6 = paths_5;
                strcat(paths_6, "/");
                strcat(paths_6, command[(y * 2) + 1]);
            }
            else
                paths_6 = command[(y * 2) + 1];

            strcat(paths_6, "/");

            for (int i = 0; i < file_count; i++)
            {
                char temp[100];
                strcpy(temp, stored_names[i]);

                char dir_3[1000];
                strcpy(dir_3, paths_6);
                strcat(dir_3, temp);
                ls_per_print(dir_3, temp);
            }

            closedir(directory);
        }
    }
}

void ls_per_print(char *f_path, char *f)
{
    struct stat f_ = {0};
    if (stat(f_path, &f_) == 0)
    {
        mode_t p = f_.st_mode;
        char p_array[11];
        p_array[0] = S_ISDIR(p) ? 'd' : '-';
        p_array[1] = ((p & S_IRUSR) ? 'r' : '-');
        p_array[2] = ((p & S_IWUSR) ? 'w' : '-');
        p_array[3] = ((p & S_IXUSR) ? 'x' : '-');
        p_array[4] = ((p & S_IRGRP) ? 'r' : '-');
        p_array[5] = ((p & S_IWGRP) ? 'w' : '-');
        p_array[6] = ((p & S_IXGRP) ? 'x' : '-');
        p_array[7] = ((p & S_IROTH) ? 'r' : '-');
        p_array[8] = ((p & S_IWOTH) ? 'w' : '-');
        p_array[9] = ((p & S_IXOTH) ? 'x' : '-');
        p_array[10] = '\0';

        char time[80];
        time_t t = f_.st_mtime;
        struct tm t_;
        localtime_r(&t, &t_);

        strftime(time, sizeof(time), "%b %d %Y %H:%M", &t_);
        struct passwd *pass_entry = getpwuid(f_.st_uid);
        struct group *grp_id = getgrgid(f_.st_gid);
        printf("%s %ld\t%s\t%s\t%ld\t%s ", p_array, f_.st_nlink, pass_entry->pw_name, grp_id->gr_name, f_.st_size, time);
        if (S_ISDIR(p))
        {
            if (redirection_check == 0)
                printf("\033[1;34m");
            printf("%s\n", f);
            if (redirection_check == 0)
                printf("\033[0m");
        }
        else if ((p & S_IXUSR))
        {
            if (redirection_check == 0)
                printf("\033[1;32m");
            printf("%s\n", f);
            if (redirection_check == 0)
                printf("\033[0m");
        }
        else
            printf("%s\n", f);
    }
    else
        perror("Error reading file permissions\n");
}

void ls_print(char *dir_3, char *f)
{
    struct stat f_ = {0};
    if (stat(dir_3, &f_) == 0 && S_ISDIR(f_.st_mode))
    {
        if (redirection_check == 0)
        printf("\033[1;34m");
        printf("%s\n", f);
        if (redirection_check == 0)
        printf("\033[0m");
    }
    else if (stat(dir_3, &f_) == 0 && (f_.st_mode & S_IXUSR))
    {
        if (redirection_check == 0)
        printf("\033[1;32m");
        printf("%s\n", f);
        if (redirection_check == 0)
        printf("\033[0m");
    }
    else
        printf("%s\n", f);
}
