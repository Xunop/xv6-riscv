#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int i;
    char *nargc[MAXARG];

    if (argc < 2) {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }

    for(i = 1; i < argc; i++) {
        nargc[i - 1] = argv[i];
    }

    // Read from stdin
    char buf[512];
    int n;
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        for (i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                buf[i] = '\0';
                nargc[argc - 1] = buf;
                break;
            }
        }
        if (i == n) {
            fprintf(2, "xargs: input line too long\n");
            exit(1);
        }
    }

    if (fork()) {
        wait(0);
    } else {
        exec(nargc[0], nargc);
    }
 
    exit(0);
}
