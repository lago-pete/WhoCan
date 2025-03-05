#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pwd.h>


#define PATH_MAX        4096 


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
    char resolved_path[PATH_MAX];
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



int main(int argc, char *argv[]) {
    check_permissions(argv[1]);
    check_path(argv[1]);
    get_info_pw();
    return 0;
}
