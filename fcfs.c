#include<stdio.h>

// Process structure with short variable names for turnaround and waiting times
typedef struct {
    int pid; // process id
    int at;  // arrival time
    int bt;  // burst time
    int ct;  // completion time
    int tat; // turn around time
    int wt;  // waiting time
    int rt;  // response time
} P;

// Function prototype
void fcfs(P[], int);

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    P p[n];
    // Input process details
    for(int i = 0; i < n; i++) {
        printf("Process %d\n", i+1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        p[i].pid = i+1;
    }

    // Sort processes by arrival time (ensures correct FCFS order)
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(p[j].at > p[j+1].at) {
                P temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    // Call FCFS scheduling function
    fcfs(p, n);
    return 0;
}

// FCFS scheduling algorithm
void fcfs(P p[], int n) {
    int cur = 0; // Current (elapsed) time
    int tot_wt = 0, tot_tat = 0, tot_rt = 0; // Totals for averages
    float avg_wt, avg_tat, avg_rt;

    printf("\nGantt chart\n");
    for(int i = 0; i < n; i++) {
        // If process arrives after current time, CPU is idle
        if(p[i].at > cur)
            cur = p[i].at;

        int st = cur; // Start time for this process

        p[i].rt = cur - p[i].at; // response time is start time - arrival time
        cur += p[i].bt;          // add burst time to current to get completion
        p[i].ct = cur;           // completion time

        p[i].tat = p[i].ct - p[i].at; // turn around time
        p[i].wt = p[i].tat - p[i].bt; // waiting time

        // Show Gantt chart part
        printf("|(%d)P%d(%d)", st, p[i].pid, cur);
    }
    printf("|\n");

    // Output observation table and calculate averages
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i=0; i<n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].at, p[i].bt, p[i].ct,
            p[i].tat, p[i].wt, p[i].rt);
        tot_tat += p[i].tat;
        tot_wt += p[i].wt;
        tot_rt += p[i].rt;
    }
    avg_tat = (float)tot_tat / n;
    avg_wt = (float)tot_wt / n;
    avg_rt = (float)tot_rt / n;
    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);
    printf("Average Response Time: %.2f\n", avg_rt);
}
