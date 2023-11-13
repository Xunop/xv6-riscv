#include "kernel/types.h"
#include "user/user.h"

#define N 35

void sieve(int in, int out, int p);

// Child process reads from the pipe
// Get the number from left side of the pipe
// Sieve of Eratosthenes: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
// https://swtch.com/~rsc/thread/#2
int main(int argc, char *argv[]) {
    // fd[0] is for reading, fd[1] is for writing
    int pd[2];
    int prime;

    pipe(pd);

    if (fork()) {
        // Parent create
        close(pd[0]);
        for (int i = 2; i <= N; i++) {
            write(pd[1], &i, sizeof(i));
        }
        close(pd[1]);
        // Wait for the child process to finish.
        wait(0);

        exit(0);
    } else {
        // Child

        // Close the write end of the pipe
        close(pd[1]);

        // Create processes for each prime number in the pipeline
        while(read(pd[0], &prime, sizeof(prime)) > 0) {
            printf("prime %d\n", prime);

            int newPipes[2];
            pipe(newPipes);

            if (fork()) {
                // Parent
                close(pd[0]);
                close(newPipes[1]);
                // Update the pipe for the next iteration
                pd[0] = newPipes[0];
            } else {
                // Child
                close(newPipes[0]);
                sieve(pd[0], newPipes[1], prime);
            }
        }

        close(pd[0]);
        exit(0);
    }
    exit(0);
}

void sieve(int in, int out, int p) {
    int n;

    while(read(in, &n, sizeof(n)) > 0) {
        if (n % p != 0) {
            write(out, &n, sizeof(n));
        }
    }

    close(in);
    close(out);

    exit(0);
}
