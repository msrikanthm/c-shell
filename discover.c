#include "functions.h"

char r[1000];
char store[1000];


int all_files(char *path, char *if_e, char *dirr, int w, char *path_to_dir)
{
    struct dirent *d;
    DIR *directory;
    directory = opendir(path);
    if (directory == NULL)
    {
        printf("Cannot open the directory\n");
        return 0;
    }
    char em[2] = "";
    char paths_1[1000];
    while ((d = readdir(directory)) != NULL)
    {
        struct stat f = {0};

        strcpy(paths_1, path);
        strcat(paths_1, "/");
        strcat(paths_1, d->d_name);
        if (stat(paths_1, &f) == 0)
        {
            if ((w != 1 && w != 5) && (f.st_mode & S_IFREG))
            {
                if (strcmp(if_e, em) == 0)
                {
                    if (strcmp(dirr, em) == 0)
                    {
                        if (w == 4 || w == 6 || w == 7)
                            printf("./%s\n", d->d_name);
                        else if (w == 2 || w == 3)
                            printf("%s/%s\n", store, d->d_name);
                        else if (w == 8)
                            printf("%s/%s\n", home_dir, d->d_name);
                        else if (w == 9)
                            printf("../%s\n", d->d_name);
                    }
                    else
                    {
                        if (strcmp(dirr, d->d_name) == 0)
                        {
                            if (w == 4 || w == 6 || w == 7)
                                printf("./%s\n", d->d_name);
                            else if (w == 0 || w == 2 || w == 3)
                                printf("%s/%s\n", store, d->d_name);
                        }
                    }
                }
                else
                {
                    if (strcmp(dirr, em) == 0)
                    {
                        if (w == 4 || w == 6 || w == 7)
                            printf(".%s/%s\n", if_e, d->d_name);
                        else if (w == 2 || w == 3)
                            printf("%s%s/%s\n", store, if_e, d->d_name);
                        else if (w == 8)
                            printf("%s%s/%s\n", home_dir, if_e, d->d_name);
                        else if (w == 9)
                            printf("..%s/%s\n", if_e, d->d_name);
                    }
                    else
                    {
                        if (strcmp(dirr, d->d_name) == 0)
                        {
                            if (w == 4 || w == 6 || w == 7)
                                printf(".%s/%s\n", if_e, d->d_name);
                            else if (w == 0 || w == 2 || w == 3)
                                printf("%s%s/%s\n", store, if_e, d->d_name);
                        }
                    }
                }
            }
            if ((w != 2 && w != 6) && (f.st_mode & S_IFDIR) && d->d_name[0] != '.')
            {
                if (strcmp(if_e, em) == 0)
                {
                    if (strcmp(dirr, em) == 0)
                    {
                        if (w == 4 || w == 5 || w == 7)
                            printf("./%s\n", d->d_name);
                        else if (w == 1 || w == 3)
                            printf("%s/%s\n", store, d->d_name);
                        else if (w == 8)
                            printf("%s/%s\n", home_dir, d->d_name);
                        else if (w == 9)
                            printf("../%s\n", d->d_name);
                    }
                    else
                    {
                        if (strcmp(dirr, d->d_name) == 0)
                        {
                            if (w == 4 || w == 5 || w == 7)
                                printf("./%s\n", d->d_name);
                            else if (w == 0 || w == 1 || w == 3)
                                printf("%s/%s\n", store, d->d_name);
                        }
                    }
                }
                else
                {
                    if (strcmp(dirr, em) == 0)
                    {
                        if (w == 4 || w == 5 || w == 7)
                            printf(".%s/%s\n", if_e, d->d_name);
                        else if (w == 1 || w == 3)
                            printf("%s%s/%s\n", store, if_e, d->d_name);
                        else if (w == 8)
                            printf("%s%s/%s\n", home_dir, if_e, d->d_name);
                        else if (w == 9)
                            printf("..%s/%s\n", if_e, d->d_name);
                    }
                    else
                    {
                        if (strcmp(dirr, d->d_name) == 0)
                        {
                            if (w == 4 || w == 5 || w == 7)
                                printf(".%s/%s\n", if_e, d->d_name);
                            else if (w == 0 || w == 1 || w == 3)
                                printf("%s%s/%s\n", store, if_e, d->d_name);
                        }
                    }
                }
            }
            if ((f.st_mode & S_IFDIR) && d->d_name[0] != '.')
            {
                strcat(r, "/");
                strcat(r, d->d_name);
                all_files(paths_1, r, dirr, w, path_to_dir);
                strcpy(r, &path[strlen(path_to_dir)]);
            }
        }
    }
    closedir(directory);
}

int discover_command(char **command, int index)
{
    char current_directory[1000];
    getcwd(current_directory, 1000);
    char p_directory[1000];
    int ls_store[] = {0, 0, 0, 0, 0};
    char t[2] = "";
    char e[2] = "";
    char path_to_dir[1000];
    if (index == 1)
    {
        strcpy(path_to_dir, home_dir);
        all_files(current_directory, e, t, 0, path_to_dir);
    }
    if (index > 4)
        return 0;

    char dis[5][10];
    strcpy(dis[0], ".");
    strcpy(dis[1], "..");
    strcpy(dis[2], "~");
    strcpy(dis[3], "-f");
    strcpy(dis[4], "-d");
    strcpy(path_to_dir, current_directory);

    int check = 0;

    for (int i = 1; i < index; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (!strcmp(command[i], dis[j]))
                ls_store[j] = 1;
        }
        if (!strcmp(command[i], dis[1]))
        {
            char *temp = strrchr(current_directory, '/');
            char dir[1000];
            if (temp)
            {
                int x;
                for (x = 0; x < (temp - current_directory); x++)
                    dir[x] = current_directory[x];

                dir[x] = '\0';
            }
            strcpy(path_to_dir, dir);
            check = 4;
        }
        else if (!strcmp(command[i], dis[2]))
        {
            strcpy(path_to_dir, home_dir);
            check = 3;
        }
        else if (!strcmp(command[i], dis[0]))
            strcpy(path_to_dir, current_directory);
        else if (command[i][0] == '.' && command[i][1] == '/') // i.e, ./<dir>
        {
            check = 2;
            strcpy(store, command[i]);
            strcat(path_to_dir, &command[i][1]);
        }
        else if (command[i][0] == '"') // i.e, "<file>"
        {
            // to be written
            char ex[1000];
            strncpy(ex, &command[i][1], strlen(command[i]) - 2);
            ex[strlen(command[i]) - 2] = '\0';
            strcpy(t, ex);
        }
        else if (ls_store[3] == 0 && ls_store[4] == 0)
        {
            check = 2;

            strcpy(store, command[i]);
            strcat(path_to_dir, "/");
            strcat(path_to_dir, command[i]);
        }
    }
    int tem = 0;
    if (ls_store[3] == 1 && ls_store[4] == 0 && check == 2) // -f ./<> or -f <>
        tem = 2;
    else if (ls_store[4] == 1 && ls_store[3] == 0 && check == 2) // -d ./<> or -d <>
        tem = 1;
    else if (ls_store[4] == 1 && ls_store[3] == 1 && check == 2) // {-d, -f}*  ./<> or <>
        tem = 3;
    else if (ls_store[3] == 1 && ls_store[4] == 0 && check == 0) // -f or -f ""
        tem = 6;
    else if (ls_store[4] == 1 && ls_store[3] == 0 && check == 0) // -d or -d ""
        tem = 5;
    else if (ls_store[4] == 1 && ls_store[3] == 1 && check == 0) // -d -f
        tem = 7;
    else if (check == 0) // {{./, ., .. , ~},<>} " "
        tem = 4;
    else if (check == 2) //
        tem = 3;
    else if (check == 3)
        tem = 8;
    else if (check == 4)
        tem = 9;

    // -f -d - tem = 7 --- files
    // -d    - tem = 6
    // -f    - tem = 5
    //       - tem = 4
    // -f -d - tem = 3 --- directories
    // -d    - tem = 2
    // -f    - tem = 1
    //       - tem = 0

    all_files(path_to_dir, "", t, tem, path_to_dir);
}