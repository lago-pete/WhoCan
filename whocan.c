#include "whocan.h"
int everyone = 1;



int runner(const char *act, const char * file_name)
{
    struct stat sb;
    char ab_path[100];
    int curr_perm = 0;
    struct passwd *pw;
    char **users = malloc(100 * sizeof(char *));
    int cap = 100;
    int num = 0;
    

    if(check_act(act)) {
        return 1;
    }
    if (realpath(file_name, ab_path) == NULL) {
        perror("realpath");
        return 1;
    }
    if(stat(ab_path, &sb) == -1) {
        perror("stat");
        return 1;
    }
    


    curr_perm =  act_perm(act, &sb);
    if (curr_perm == 0) {
        fprintf(stderr, "Invalid action: %s\n", act);
        return 1;
    }
    

    while ((pw = getpwent()) != NULL) {
        int has_perm = 0;
        
        if (strcmp(act, "delete") == 0) {
            has_perm = can_delete(ab_path, pw);
        } else {
            has_perm = perm_check(&sb, pw, curr_perm);
        }

        if (has_perm) {
            if (num >= cap) {
                cap *= 2;
                users = resize_array(users, num, cap);
                if (!users) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            users[num] = strdup(pw->pw_name);
            num++;
        } else {
            everyone = 0;
        }
    }

    endpwent();



    if (everyone) {
        printf("(everyone)\n");
    } else {
        print_rez(users, num);
    }

    //free(users); free(): double free detected in tcache 2 Aborted (core dumped): still not working: just had to make clean then re make, now working
    everyone = 1;

    return 0;
}


char **resize_array(char **users, int count, int cap) { 
    char **new_users = realloc(users, cap * sizeof(char *));
    return new_users;
}


void print_rez(char **users, int num)
{
    qsort(users, num, sizeof(char *), compare_users);
    for (int i = 0; i < num; i++) {
        printf("%s\n", users[i]);
        free(users[i]);  
    }
    free(users);  
}



//need to be able to jump back and then check 
// needs to check parent dir
// parent dir  -> rwxrwxrwxt (looks for sticky bit)
// if sticky then only owners can delete
// if not sticky then if write and execute then can delete

// dir -> drwxrwxrwt
// file -> -rw-r--r--




int can_delete(const char *filepath, struct passwd *pw)
{
    struct stat file_stat;
    struct stat parent_stat;
    char path_copy[MAX_PATH]; 

   
    if (stat(filepath, &file_stat) == -1)
    {
        perror("stat");
        return 0;
    }

    strncpy(path_copy, filepath, MAX_PATH - 1);
    path_copy[MAX_PATH - 1] = '\0';

    
    char *parent_path = dirname(path_copy);

    if (stat(parent_path, &parent_stat) == -1)
    {
        perror("stat parent dir");
        return 0;
    }

    
    if (parent_stat.st_mode & S_ISVTX)
    {
        return (file_stat.st_uid == pw->pw_uid || parent_stat.st_uid == pw->pw_uid);
    }

    
    return perm_check(&parent_stat, pw, S_IWUSR | S_IXUSR);
}

// 421 421 421

// rw-r-----
// 42- 4-- ---
// 110 100 000
// owner: 6octal group: 4octal others: 0octal
// does autoshift? test if
// 110 100 000
// 000 110 100
// 000 000 110

int perm_check(struct stat *sb, struct passwd *pw, int curr_perm)
{
    struct group *grp = getgrgid(sb->st_gid);
    if (sb->st_uid == pw->pw_uid) 
    {
        return sb->st_mode & curr_perm;
    }

    if (grp) {
        for (int i = 0; grp->gr_mem[i]; i++) {
            if (strcmp(grp->gr_mem[i], pw->pw_name) == 0) 
            {
                return sb->st_mode & (curr_perm >> 3);            
            }
        }
    }

    
    return sb->st_mode & (curr_perm >> 6);
}
    

//I had to use ai for these pointer referencess!
int compare_users(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}




int check_act(const char *action)
{
    if(strcmp(action, "cd") != 0 && strcmp(action, "delete") != 0 &&
       strcmp(action, "ls") != 0 && strcmp(action, "read") != 0 &&
       strcmp(action, "write") != 0 && strcmp(action, "execute") != 0 && strcmp(action, "search") != 0) 
    {
        fprintf(stderr, "Invaid Action %s \n", action);
        return 1;
    }
    return 0;
}


int act_perm(const char *act, struct stat *sb) {
    if (strcmp(act, "read") == 0 || strcmp(act, "ls") == 0) {
        return S_IRUSR;
    }
    if (strcmp(act, "write") == 0) {
        return S_IWUSR;
    }
    if (strcmp(act, "execute") == 0) {
        return S_IXUSR;
    }
    if (strcmp(act, "cd") == 0 || strcmp(act, "search") == 0) {
        if (S_ISDIR(sb->st_mode)) return S_IXUSR;
    }
    if (strcmp(act, "delete") == 0) {
        return S_IWUSR | S_IXUSR;  
    }
    return 0;
}



















//testing section






// int can_delete(const char *filepath, struct passwd *pw) {

//     struct stat file_stat, parent_stat;
//     char parent_path[MAX_PATH];

//     if (stat(filepath, &file_stat) == -1) {
//         perror("stat");
//         return 0;
//     }

    
//     strncpy(parent_path, filepath, MAX_PATH );
//     parent_path[MAX_PATH - 1] = '\0'; 
//     char *last_slash = strrchr(parent_path, '/');



//     if (last_slash) {
//         *last_slash = '\0';
//     } else {
//         strcpy(parent_path, ".");
//     }

//     if (stat(parent_path, &parent_stat) == -1) {
//         perror("stat parent dir");
//         return 0;
//     }


//     if (parent_stat.st_mode & S_ISVTX) {
//         return (file_stat.st_uid == pw->pw_uid || parent_stat.st_uid == pw->pw_uid);
//     }

//     return perm_check(&parent_stat, pw, S_IWUSR | S_IXUSR);
// }

//using getgrgid instead of getgrent
// int perm_check(struct stat *sb, struct passwd *pw, int curr_perm) {
//     struct group *grp;

    
//     if (sb->st_uid == pw->pw_uid) {
//         return sb->st_mode & curr_perm;
//     }

//     setgrent(); 
//     while ((grp = getgrent()) != NULL) {
//         if (grp->gr_gid == sb->st_gid) {  
//             for (int i = 0; grp->gr_mem[i]; i++) {
//                 if (strcmp(grp->gr_mem[i], pw->pw_name) == 0) {  
//                     return sb->st_mode & (curr_perm >> 3);
//                 }
//             }
//         }
//     }
//     endgrent();

   
//     return sb->st_mode & (curr_perm >> 6);
// }

//void check_permissions(const char *filename, struct stat *sb) {
    

    //     printf("Permissions for %s:\n", filename);
    
        
    //     printf("Owner: %c%c%c\n",
    //            (sb->st_mode & S_IRUSR) ? 'r' : '-',
    //            (sb->st_mode & S_IWUSR) ? 'w' : '-',
    //            (sb->st_mode & S_IXUSR) ? 'x' : '-');
    
    //     printf("Group: %c%c%c\n",
    //            (sb->st_mode & S_IRGRP) ? 'r' : '-',
    //            (sb->st_mode & S_IWGRP) ? 'w' : '-',
    //            (sb->st_mode & S_IXGRP) ? 'x' : '-');
    
    //     printf("Others: %c%c%c\n",
    //            (sb->st_mode & S_IROTH) ? 'r' : '-',
    //            (sb->st_mode & S_IWOTH) ? 'w' : '-',
    //            (sb->st_mode & S_IXOTH) ? 'x' : '-');
    // }
//
// int check_act(const char *action)
// {
//     if(strcmp(action, "cd") != 0 && strcmp(action, "delete") != 0 &&
//        strcmp(action, "ls") != 0 && strcmp(action, "read") != 0 &&
//        strcmp(action, "write") != 0 && strcmp(action, "execute") != 0 && strcmp(action, "search") != 0) 
//        {
//         fprintf(stderr, "Usage: %s <file>\n", action);
//         return 1;
//         }
//     return 0;
// }

    
// int check_path(const char *filename, char *resolved_path)
// {
//     if (realpath(filename, resolved_path) == NULL) {
//         perror("realpath");
//         return 1;
//     }
//     return 0; 
// }




// struct passwd {
//     char   *pw_name;       /* username */
//     char   *pw_passwd;     /* user password */
//     uid_t   pw_uid;        /* user ID */
//     gid_t   pw_gid;        /* group ID */
//     char   *pw_gecos;      /* user information */
//     char   *pw_dir;        /* home directory */
//     char   *pw_shell;      /* shell program */
// };


// int get_info_pw()
// {
//     struct passwd *pw;

//     while ((pw = getpwent()) != NULL) {
//         printf("User: %s (UID: %d)   UserDir: %s\n", pw->pw_name, pw->pw_uid, pw->pw_dir);
//     }

//     endpwent();
//     return 0;
// }



// int get_info_gr()
// {
//     struct group *gr;

//     while ((gr = getgrent()) != NULL) {
//         printf("Group: %s (GID: %d)\n", gr->gr_name, gr->gr_gid);
//     }

//     endgrent();
//     return 0;
// }

