// ---- zombie.c ---- //
// This program demonstrates the creation of a Zombie process.

#include <sys/types.h>  // For pid_t: data type used for process IDs
#include <unistd.h>     // For fork(), getpid(), getppid(), sleep()
#include <stdio.h>      // For printf()
#include <stdlib.h>     // For exit()

int main() {
    // pid_t is a data type used to store process IDs
    pid_t child_pid, my_pid, parent_pid;

    // Create a new child process
    child_pid = fork();

    // fork() returns:
    // - 0 to the child process
    // - >0 (child's PID) to the parent process
    // - <0 if fork fails
    if (child_pid < 0) {
        printf("Fork failed. Exiting!\n");
        exit(0);
    }

    // -------------------- CHILD PROCESS --------------------
    if (child_pid == 0) {
        // Even though this block is written first,
        // whether parent or child runs first is decided by the OS scheduler.
        // In most systems, the parent gets scheduled first because
        // it's already in memory, and child is a new process.

        printf("[CHILD] This is the child process.\n");

        my_pid = getpid();      // getpid() returns the current process's ID
        parent_pid = getppid(); // getppid() returns the parent process's ID

        printf("[CHILD] My pid is %d\n", my_pid);
        printf("[CHILD] My parent's pid is %d\n", parent_pid);

        printf("[CHILD] Exiting.\n");
        exit(0); // Child exits immediately — becomes a zombie until the parent reaps it
    }

    // -------------------- PARENT PROCESS --------------------
    else {
        printf("[PARENT] This is the parent process.\n");

        my_pid = getpid();       // Get parent's PID
        parent_pid = getppid();  // Get parent's parent PID (usually shell or terminal)

        printf("[PARENT] My pid is %d\n", my_pid);
        printf("[PARENT] My parent's pid is %d\n", parent_pid);

        printf("[PARENT] Sleeping for 10 seconds.\n");
        sleep(10); // Parent sleeps — child exits and becomes a zombie

        // Since parent hasn't called wait(), the child remains a zombie during this time
        printf("[PARENT] Child pid = %d has ended, but it has an entry in process table.\n", child_pid);
        printf("[PARENT] It is a zombie process.\n");
    }

    return 0;
}



/*
======================== EXPLANATION ========================

What is pid_t?
- pid_t is a data type used to represent process IDs.

What are getpid() and getppid()?
- getpid() returns the process ID of the current process.
- getppid() returns the process ID of the parent process.

Why does the parent process usually run first?
- Although the child process block is written before the parent,
  the OS scheduler decides which one executes first.
- Since the parent process already exists in memory,
  it is typically given CPU time before the newly created child.

What is a Zombie Process?
- A zombie is a child process that has finished execution
  but still remains in the process table because the parent
  hasn't yet collected its exit status using wait().
- In this program, the parent sleeps for 10 seconds
  without calling wait(), causing the child to become a zombie.

To observe the zombie:
1. Run this program in a Linux terminal.
2. While it's sleeping, open another terminal and run:
   ps -elf | grep Z

=============================================================
*/
/*Output:
[PARENT] This is the parent process.
[CHILD] This is the child process.
[PARENT] My pid is 578
[PARENT] My parent's pid is 40
[PARENT] Sleeping for 10 seconds.
[CHILD] My pid is 579
[CHILD] My parent's pid is 578
[CHILD] Exiting.
[PARENT] Child pid = 579 has ended, but it has an entry in process table.
[PARENT] It is a zombie process.*/