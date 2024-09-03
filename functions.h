#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include "global_var.h"

int cd_command(char **command, int index);
int all_files(char *path, char *if_e, char *dirr, int w, char *path_to_dir);
int discover_command(char **command, int index);
int echo_command(char **command, int index);
int pwd_command(char **command, int index);
int history_command(int count);
int function(const void *a, const void *b);
int ls_command(char **command, int index);
void ls_per_print(char *f_path, char *f);
void ls_print(char *dir_3, char *f);
int pinfo_command(char **command, int index);
char *redirect_command(char *input_);
void fore_back(char *inputs);
void background();
void pipelining_command(char *input_);

#endif