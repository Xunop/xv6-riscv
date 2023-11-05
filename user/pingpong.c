#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // fd[0] is for reading, fd[1] is for writing
    int parent_fd[2], child_fd[2];
    char buf[64];

    if (pipe(parent_fd) < 0 || pipe(child_fd) < 0) {
        printf("pipe() failed\n");
        exit(1);
    }

    // fork() returns 0 in the child process
    // and returns the child's pid in the parent.
    // Here is the parent process.
    if (fork()) {
        write(parent_fd[1], "ping", strlen("ping"));
        read(child_fd[0], buf, strlen("pong"));
        printf("%d: received %s\n", getpid(), buf);
    }
    else {
        // Child
        read(parent_fd[0], buf, strlen("ping"));
        printf("%d: received %s\n", getpid(), buf);
        write(child_fd[1], "pong", strlen("pong"));
    }
    exit(0);
}
