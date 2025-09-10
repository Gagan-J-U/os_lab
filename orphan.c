// ---- orphan.c ---- //
// This program demonstrates the concept of an ORPHAN PROCESS in Unix/Linux.
// An orphan process is a child process whose parent has exited before the child.
// Such orphan processes are adopted by the init process (PID 1) or systemd.

#include <sys/types.h>   // For pid_t
#include <unistd.h>      // For fork(), getpid(), getppid(), sleep()
#include <stdio.h>       // For printf()
#include <stdlib.h>      // For exit()

int main() {
    pid_t child_pid, my_pid, parent_pid;  // Variables to store PIDs
    

    // Create a child process using fork()
    // fork() creates a duplicate of the current process
    // Returns:
    // 0     → to the child process
    // > 0   → to the parent process (value is child's PID)
    // < 0   → on failure
    child_pid = fork();

    // Check if fork failed
    if (child_pid < 0) {
        printf("Fork failed. Exiting!\n");
        exit(0);
    }

    // If fork() returns 0, this block is executed by the child process
    if (child_pid == 0) {
        // --- CHILD PROCESS CODE ---
        printf("[CHILD] This is the child process.\n");

        my_pid = getpid();       // Get current process ID
        parent_pid = getppid();  // Get parent process ID

        printf("[CHILD] My pid is %d\n", my_pid);
        printf("[CHILD] My parent's pid is %d\n", parent_pid);

        // Sleep for 10 seconds to give the parent time to exit
        // After this, the child will become an orphan
        
        printf("[CHILD] Sleeping for 10 seconds.\n");
        sleep(10);

        // After sleep, the parent is gone; child is now orphan
        // getppid() will return 1 (or systemd PID), since init adopts it
        parent_pid = getppid();  // Get new parent PID
        printf("[CHILD] My parent ended. So I am an orphan process adopted by init process (new parent pid: %d).\n", parent_pid);
    } else {
        // --- PARENT PROCESS CODE ---
        printf("[PARENT] This is the parent process.\n");

        my_pid = getpid();       // Get current process ID (parent)
        parent_pid = getppid();  // Get parent's PID (usually the shell/terminal)

        printf("[PARENT] My pid is %d\n", my_pid);
        printf("[PARENT] My parent's pid is %d\n", parent_pid);
        
        // Exit the parent immediately to orphan the child
        printf("[PARENT] Exiting.\n");
        sleep(1);
        exit(0);
    }

    return 0;
}
// The child process will now be adopted by the init process (PID 1)
// It will continue to run until it finishes its sleep, demonstrating orphan behavior.
// After the parent exits, the child will print its new parent PID (1 or systemd)
/*Output:
[PARENT] This is the parent process.
[PARENT] My pid is 624
[PARENT] My parent's pid is 40
[PARENT] Exiting.
[CHILD] This is the child process.
[CHILD] My pid is 625
[CHILD] My parent's pid is 624
[CHILD] Sleeping for 10 seconds.
[CHILD] My parent ended. So I am an orphan process adopted by init process.*/