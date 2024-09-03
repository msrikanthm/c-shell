#ifndef DIR_H
#define DIR_H

// stores the home directory
char home_dir[1000];
// to store the directory in which we stay ""while executing the previous command""
char dir_store[1000];
char command_store[20][1000];
int command_count;
int redirection_check;
//int back_check;
//int time_taken;
//int last;
//int symcheck;

#endif