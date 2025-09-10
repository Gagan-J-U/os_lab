#include <stdio.h>

typedef struct {
    int pid; // Process ID
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    int rt;  // Response Time
    int done; // Flag to mark process as finished
} P;

void sjf_np(P p[], int n);

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    P p[n];
    // Input for each process
    for(int i = 0; i < n; i++) {
        printf("Process %d\n", i+1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        p[i].pid = i+1;
        p[i].done = 0;
    }
    sjf_np(p, n);
    return 0;
}

void sjf_np(P p[], int n) {
    int cur = 0, left = n; // cur: current time, left: processes left
    int tot_wt = 0, tot_tat = 0, tot_rt = 0;
    printf("\nGantt chart\n");
    while(left) {
        int idx = -1;
        int min_bt = 9999;
        // Select process with shortest burst time among those that have arrived and are unfinished
        for(int i = 0; i < n; i++) {
            if(!p[i].done && p[i].at <= cur && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if(idx == -1) {
            // No process has arrived, CPU idle
            printf("|(%d) idle (%d)", cur, cur+1);
            cur++;
        } else {
            int st = cur;
            p[idx].rt = cur - p[idx].at; // Response = time when scheduled - arrival
            cur += p[idx].bt;
            p[idx].ct = cur;            // Completion = time process finishes
            p[idx].tat = p[idx].ct - p[idx].at; // Turnaround = completion - arrival
            p[idx].wt = p[idx].tat - p[idx].bt; // Waiting = turnaround - burst
            p[idx].done = 1; // mark finished
            left--;
            // Gantt chart representation
            printf("|(%d)P%d(%d)", st, p[idx].pid, cur);
        }
    }
    printf("|\n");

    // Output observation table and averages
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i=0; i<n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
        tot_tat += p[i].tat;
        tot_wt += p[i].wt;
        tot_rt += p[i].rt;
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)tot_wt/n);
    printf("Average Turnaround Time: %.2f\n", (float)tot_tat/n);
    printf("Average Response Time: %.2f\n", (float)tot_rt/n);
}
