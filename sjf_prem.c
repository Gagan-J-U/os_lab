#include <stdio.h>

typedef struct {
    int pid; // Process ID
    int at;  // Arrival Time
    int bt;  // Burst Time
    int rt;  // Remaining Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    int res; // Response Time
    int started; // 0: not started, 1: started
} P;

void sjf_p(P p[], int n);

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    P p[n];
    for(int i=0; i<n; i++) {
        printf("Process %d\n", i+1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        p[i].pid = i+1;
        p[i].rt = p[i].bt;
        p[i].started = 0;
    }
    sjf_p(p, n);
    return 0;
}

void sjf_p(P p[], int n) {
    int cur = 0, left = n;
    int tot_wt = 0, tot_tat = 0, tot_res = 0;
    while(left) {
        int idx = -1, min_rt = 9999;
        for(int i=0; i<n; i++) {
            if(p[i].at <= cur && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                idx = i;
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
        p[idx].rt--;
        cur++;
        
        if(p[idx].rt == 0) {
            // Process finished
            p[idx].ct = cur;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            left--;
            tot_tat += p[idx].tat;
            tot_wt += p[idx].wt;
            tot_res += p[idx].res;
        }
    }
    // Output observation table
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i=0; i<n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt,
            p[i].ct, p[i].tat, p[i].wt, p[i].res);
    }
 
    printf("\n");
    printf("\nAverage Waiting Time: %.2f\n", (float)tot_wt/n);
    printf("Average Turnaround Time: %.2f\n", (float)tot_tat/n);
    printf("Average Response Time: %.2f\n", (float)tot_res/n);
}
