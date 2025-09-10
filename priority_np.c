#include <stdio.h>
#include <stdlib.h>

typedef struct process {
    int pid;      // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time
    int ct;       // Completion Time
    int tat;      // Turnaround Time
    int wt;       // Waiting Time
    int done;   // Completion status (0 = not finished, 1 = finished)
    int rt;    //response time
    int pri;      // Priority (lower = higher priority)
} P;

P p[15];  // Array of processes with short name

void priority_np(P p[], int n);
int main() {
    int n;
    printf("\nEnter the number of processes:\n");
    scanf("%d", &n);

    // Input: Arrival Time, Burst Time, and Priority for each process
    for(int i = 0; i < n; i++) {
        printf("Process %d\n", i+1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        printf("Enter Priority (lower number = higher priority): ");
        scanf("%d", &p[i].pri);
        p[i].pid = i+1;
        p[i].done = 0;
    }
priority_np(p, n);
    return 0;
    
}

void priority_np(P p[], int n) {
    int tot_wt = 0, tot_tat = 0;
    float avg_wt = 0, avg_tat = 0;
    int left = n, cur = 0;
    printf("\nGantt Chart:\n\n");

    while (left) {
        int idx = -1, minp = 9999;
        // Find process with highest priority among arrived and not finished
        for (int i = 0; i < n; i++) {
            if (p[i].at <= cur && p[i].done == 0) {
                if (p[i].pri < minp || (p[i].pri == minp && p[i].at < p[idx].at)) {
                    minp = p[i].pri;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            printf("|(%d) idle (%d)", cur, cur+1);
            cur++;
        } else {
            
            int start = cur;
            p[idx].rt=cur - p[idx].at;
            cur += p[idx].bt;
            p[idx].ct = cur;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].done = 1;
            left--;
            printf("|(%d) P%d (%d)", start, p[idx].pid, cur);
        }
    }
    printf("|\n");

    

    // Display results
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tP\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct,
               p[i].tat, p[i].wt, p[i].pri);
        tot_tat += p[i].tat;
        tot_wt += p[i].wt;
    }

    avg_tat = (float)tot_tat / n;
    avg_wt = (float)tot_wt / n;
    printf("\nAverage TAT = %.2f\nAverage WT = %.2f\n", avg_tat, avg_wt);
    return 0;
}
