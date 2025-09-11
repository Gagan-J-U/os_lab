#include <stdio.h>
#include <stdlib.h>

// Short-notation struct for each process
typedef struct process {
    int id;    // Process ID
    int at;    // Arrival Time
    int bt;    // Remaining Burst Time
    int ct;    // Completion Time
    int tat;   // Turnaround Time
    int wt;    // Waiting Time
    int flag;  // 1 if added to queue, else 0
} pro;

pro p[15];

void swap(pro *a, pro *b) {
    pro temp = *a;
    *a = *b;
    *b = temp;
}

// Sorts processes by arrival time (critical for RR correctness)
void sort(int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(p[j].at > p[j+1].at) {
                swap(&p[j], &p[j+1]);
            }
        }
    }
}

int main() {
    int n, temp_bt[15], total_wt = 0, total_tat = 0, quantum;
    float avg_wt = 0, avg_tat = 0;

    printf("\nEnter the number of processes:\n");
    scanf("%d", &n);

    // Input arrival and burst times for all processes
    printf("\nEnter the arrival time and burst time of each process:\n");
    printf("AT BT\n");
    for(int i = 0; i < n; i++) {
        p[i].id = (i+1);
        scanf("%d%d", &p[i].at, &p[i].bt);
        temp_bt[i] = p[i].bt;   // Store original burst times for reporting
        p[i].flag = 0;          // Not yet in queue
    }
    printf("\nEnter the time quantum:\n");
    scanf("%d", &quantum);
    sort(n);

    // Round Robin core scheduling loop with queue management
    int left = n, cur_idx, cur_time = p[0].at;
    int *queue, front = 0, rear = 0;
    queue = (int *)malloc(n * sizeof(int));
    queue[rear] = 0;          // Start RR with first process
    p[0].flag = 1;            // Mark first process as queued

    printf("\nGantt chart (process, quantum, completion time shown):\n");

    while (left) {
        cur_idx = queue[front];
        front = (front + 1) % n;

        // If process has more burst than quantum, run quantum time
        if(p[cur_idx].bt > quantum) {
            p[cur_idx].bt -= quantum;
            cur_time += quantum;
            printf("|(%d) P%d(%d) ",-(quantum-cur_time) ,p[cur_idx].id,cur_time);
        } else {
            // Last slice for this process
            cur_time += p[cur_idx].bt;
            printf("|(%d) P%d(%d) ",cur_time-p[cur_idx].bt ,p[cur_idx].id,cur_time);
            p[cur_idx].bt = 0;
            p[cur_idx].ct = cur_time;
            p[cur_idx].tat = p[cur_idx].ct - p[cur_idx].at;
            total_tat += p[cur_idx].tat;
            // Waiting time: turnaround - original burst
            p[cur_idx].wt = p[cur_idx].tat - temp_bt[cur_idx];
            total_wt += p[cur_idx].wt;
        }

        // Enqueue new arrivals up to current time if not already queued/not done
        for(int i = 0; i < n && p[i].at <= cur_time; i++) {
            if(i == cur_idx || p[i].flag == 1 || p[i].bt == 0)
                continue;
            rear = (rear + 1) % n;
            p[i].flag = 1;
            queue[rear] = i;
        }

        // If process still unfinished, re-enqueue it at the back
        if(p[cur_idx].bt > 0) {
            rear = (rear + 1) % n;
            queue[rear] = cur_idx;
        } else {
            left--;
        }
    } // End of RR loop

    avg_tat = (float)total_tat / n;
    avg_wt = (float)total_wt / n;

    // Display all statistics
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, temp_bt[i], p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage TAT = %.2f\nAverage WT = %.2f\n", avg_tat, avg_wt);

    free(queue);
    return 0;
}
