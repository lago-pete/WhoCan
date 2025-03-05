#include <sys/stat.h>
#include <stdio.h>

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






int main(int argc, char *argv[]) {
    check_permissions(argv[1]);
    return 0;
}
