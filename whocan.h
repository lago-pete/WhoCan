#ifndef WHOCAN_H
#define WHOCAN_H

#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <string.h>

#define MAX_PATH 4096




int check_act(const char *action);
void print_rez(char **users, int num);
int act_perm(const char *act, struct stat *sb);
int compare_users(const void *a, const void *b);
int runner(const char *act, const char *file_name);
char **resize_array(char **users, int count, int cap);
int can_delete(const char *filepath, struct passwd *pw);
int perm_check(struct stat *sb, struct passwd *pw, int curr_perm);
int perm_check(struct stat *sb, struct passwd *pw, int curr_perm);


#endif





