#include <stdio.h>
#include <stdlib.h>

typedef struct process {
    int pid;   // Process ID
    int at;    // Arrival Time
    int bt;    // Remaining Burst Time (for calculation)
    int ct;    // Completion Time
    int tat;   // Turnaround Time
    int wt;    // Waiting Time
    int pri;
    int temp_bt;
    int started; 
    int res; // Priority (lower = higher priority)
} P;

P p[15]; // Array now uses 'p' as in your request
void priority_p(P[],int );
int main() {
    int n;
    printf("\nEnter the number of processes:\n");
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        printf("Process %d\n", i+1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        printf("Enter Priority (lower number = higher priority): ");
        scanf("%d", &p[i].pri);
        p[i].pid = i+1;
        p[i].temp_bt=p[i].bt;
        p[i].started=0;
    }

    priority_p(p,n);
    return 0;
}

void priority_p(P p[],int n){
    int tot_wt = 0, tot_tat = 0;
    float avg_wt = 0, avg_tat = 0;
    printf("\nGantt Chart:\n");
    int left = n, cur = 0;
    while (left) {
        int idx = -1, minp = 9999;
        // Among ready & unfinished, pick process with highest priority
        for (int i = 0; i < n; i++) {
            if (p[i].at <= cur && p[i].bt > 0) {
                if (p[i].pri < minp || (p[i].pri == minp && p[i].at < p[idx].at)) {
                    minp = p[i].pri;
                    idx = i;
                }
            }
        }
        // If no ready process, advance time (idle)
        if(idx == -1) {
            printf("|(%d) idle (%d)", cur, cur+1);
            cur++;
            continue;
        }
        // If first time scheduled, compute response time
        if(p[idx].started == 0) {
            p[idx].res = cur - p[idx].at;
            p[idx].started = 1;
        }
        printf("|(%d)P%d(%d)", cur, p[idx].pid, cur+1);
        p[idx].bt--;
        cur++;
        if (p[idx].bt == 0) {
            // If process just completed, store metrics
            p[idx].ct = cur;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].temp_bt;
            tot_tat += p[idx].tat;
            tot_wt += p[idx].wt;
            left--;
        }
    }
        // If no process is ready, time simply advances (CPU idle, unprinted)
    
    printf("|\n");

    avg_tat = (float)tot_tat / n;
    avg_wt = (float)tot_wt / n;

    // Display result table with original burst times
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tP\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].temp_bt,
               p[i].ct, p[i].tat, p[i].wt, p[i].pri);
    }
    printf("\nAverage TAT = %.2f\nAverage WT = %.2f\n", avg_tat, avg_wt);
    
}
