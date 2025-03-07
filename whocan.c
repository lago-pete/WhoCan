#include "whocan.h"


void check_permissions(const char *filename) {
    struct stat sb;

    if (stat(filename, &sb) == -1) {
        perror("stat");
        return;
    }

    printf("Permissions for %s:\n", filename);

    // Owner permissions
    printf("Owner: %c%c%c\n",
           (sb.st_mode & S_IRUSR) ? 'r' : '-',
           (sb.st_mode & S_IWUSR) ? 'w' : '-',
           (sb.st_mode & S_IXUSR) ? 'x' : '-');

    // Group permissions
    printf("Group: %c%c%c\n",
           (sb.st_mode & S_IRGRP) ? 'r' : '-',
           (sb.st_mode & S_IWGRP) ? 'w' : '-',
           (sb.st_mode & S_IXGRP) ? 'x' : '-');

    // Others permissions
    printf("Others: %c%c%c\n",
           (sb.st_mode & S_IROTH) ? 'r' : '-',
           (sb.st_mode & S_IWOTH) ? 'w' : '-',
           (sb.st_mode & S_IXOTH) ? 'x' : '-');
}



int check_path(const char *filename)
{
    char resolved_path[MAX_PATH];
    if (realpath(filename, resolved_path) == NULL) {
        perror("realpath");
        return 1;
    }

    printf("Absolute path: %s\n", resolved_path);
    return 0;
}


// struct passwd {
//     char   *pw_name;       /* username */
//     char   *pw_passwd;     /* user password */
//     uid_t   pw_uid;        /* user ID */
//     gid_t   pw_gid;        /* group ID */
//     char   *pw_gecos;      /* user information */
//     char   *pw_dir;        /* home directory */
//     char   *pw_shell;      /* shell program */
// };


int get_info_pw()
{
    struct passwd *pw;

    while ((pw = getpwent()) != NULL) {
        printf("User: %s (UID: %d)   UserDir: %s\n", pw->pw_name, pw->pw_uid, pw->pw_dir);
    }

    endpwent();
    return 0;
}



int get_info_gr()
{
    struct group *gr;

    while ((gr = getgrent()) != NULL) {
        printf("Group: %s (GID: %d)\n", gr->gr_name, gr->gr_gid);
    }

    endgrent();
    return 0;
}


int check_com(const char *action)
{
    if(strcmp(action, "cd") != 0 && strcmp(action, "delete") != 0 &&
       strcmp(action, "ls") != 0 && strcmp(action, "read") != 0 &&
       strcmp(action, "write") != 0 && strcmp(action, "execute") != 0 && strcmp(action, "search") != 0) 
       {
        fprintf(stderr, "Usage: %s <file>\n", action);
        return 1;
        }
    return 0;
}