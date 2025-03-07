#include "whocan.h"





int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    if(check_com(argv[0])) {
        return 1;
    }

    check_permissions(argv[1]);
    check_path(argv[1]);
    get_info_pw();
    get_info_gr();
    return 0;
}
