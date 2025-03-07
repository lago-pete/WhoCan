#include "whocan.h"




int main(int argc, char *argv[]) {
   
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    if(runner(argv[1], argv[2]) == 1){
        return 1;
    }
    
    return 0;
}




//Testing individually

// ab_path = malloc(MAX_PATH);
// struct stat sb;

// if(check_act(argv[1])) {
//     return 1;
// }

// if(check_path(argv[2], ab_path)) {
//     return 1;
// }

// if (stat(argv[2], &sb) == -1) {  
//     perror("stat");
//     return;
// }
// else{
//     check_permissions(argv[2], sb);
// }
// check_path(argv[1]);
// get_info_pw();
// get_info_gr();