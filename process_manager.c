#include "process_manager.h"

int run_basic_demo(void) {
    int pipe_fd[2];
    pid_t producer_pid, consumer_pid;
    int status;

    printf("\nParent process (PID: %d) creating children...\n", getpid());

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return -1;
    }

    producer_pid = fork();

    if (producer_pid == 0) {
        close(pipe_fd[0]);
        producer_process(pipe_fd[1], 1);
    }

    printf("Created producer child (PID: %d)\n", producer_pid);

    consumer_pid = fork();

    if (consumer_pid == 0) {
        close(pipe_fd[1]);
        consumer_process(pipe_fd[0], 0);
    }

    printf("Created consumer child (PID: %d)\n", consumer_pid);

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(producer_pid, &status, 0);
    printf("Producer exited with status %d\n", WEXITSTATUS(status));

    waitpid(consumer_pid, &status, 0);
    printf("Consumer exited with status %d\n", WEXITSTATUS(status));

    return 0;
}

int run_multiple_pairs(int num_pairs) {
    pid_t pids[10];
    int pid_count = 0;

    printf("\nParent creating %d producer-consumer pairs...\n", num_pairs);

    for (int i = 0; i < num_pairs; i++) {
        printf("=== Pair %d ===\n", i + 1);

        int fd[2];
        pipe(f
