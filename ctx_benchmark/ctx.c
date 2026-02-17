#include <stdio.h>      // printf
#include <time.h>       // clock_t, CLOCKS_PER_SEC
#include <sys/time.h>   // gettimeofday
#include <unistd.h>     // fork, getpid, getppid, sleep, exec*
#include <sys/wait.h>   // wait
#include <stdbool.h>

void measure_sys_call(void) {
    const int iter = 5;
    struct timeval tv;
    double total = 0.;

    for (int i = 0; i < iter; i++) {
        clock_t start = clock();

        gettimeofday(&tv, NULL);

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        total += elapsed;

        printf("Total time: %f seconds\n", elapsed);
    }

    printf("Average call time: %f seconds\n", total / iter);
}

// TODO
bool measure_ctx_switch(void) {
    int p2c[2];
    int c2p[2];
    if (pipe(p2c) == -1) { perror("p2c"); return false; }
    if (pipe(p2c) == -1) { perror("p2c"); return false; }

    pid_t pid1 = fork();
    if (getpid() == -1) { perror("fork error"); return false; }

    if (pid1 == 0) {
        close(p2c[0]);
        close(p2c[1]);
    }

    return true;
}

int main(void) {
    measure_sys_call();

    if (!measure_ctx_switch()) { return 1; }

    return 0;
}
