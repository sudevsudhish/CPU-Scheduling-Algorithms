#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int service_time;
    int remaining_time;
    int finish_time;
    int turn_around_time;
    int waiting_time;
} Process;

int process_count;

void inputProcesses(Process processes[]) {
    printf("Enter number of processes: ");
    scanf("%d", &process_count);
    for (int i = 0; i < process_count; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time and service time for process %d: ", i + 1);
        scanf("%d%d", &processes[i].arrival_time, &processes[i].service_time);
        processes[i].remaining_time = processes[i].service_time;
    }
}

void fcfs(Process processes[]) {
    int time = 0;
    for (int i = 0; i < process_count; i++) {
        if (time < processes[i].arrival_time) time = processes[i].arrival_time;
        processes[i].finish_time = time + processes[i].service_time;
        processes[i].turn_around_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turn_around_time - processes[i].service_time;
        time += processes[i].service_time;
    }
}

void sjf(Process processes[]) {
    bool visited[MAX_PROCESSES] = {false};
    int time = 0, completed = 0;
    while (completed < process_count) {
        int min_time = 1e9, min_index = -1;
        for (int i = 0; i < process_count; i++) {
            if (!visited[i] && processes[i].arrival_time <= time && processes[i].service_time < min_time) {
                min_time = processes[i].service_time;
                min_index = i;
            }
        }
        if (min_index != -1) {
            visited[min_index] = true;
            time += processes[min_index].service_time;
            processes[min_index].finish_time = time;
            processes[min_index].turn_around_time = processes[min_index].finish_time - processes[min_index].arrival_time;
            processes[min_index].waiting_time = processes[min_index].turn_around_time - processes[min_index].service_time;
            completed++;
        } else time++;
    }
}

void srt(Process processes[]) {
    int time = 0, completed = 0;
    while (completed < process_count) {
        int min_remaining_time = 1e9, min_index = -1;
        for (int i = 0; i < process_count; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0 && processes[i].remaining_time < min_remaining_time) {
                min_remaining_time = processes[i].remaining_time;
                min_index = i;
            }
        }
        if (min_index != -1) {
            processes[min_index].remaining_time--;
            time++;
            if (processes[min_index].remaining_time == 0) {
                processes[min_index].finish_time = time;
                processes[min_index].turn_around_time = processes[min_index].finish_time - processes[min_index].arrival_time;
                processes[min_index].waiting_time = processes[min_index].turn_around_time - processes[min_index].service_time;
                completed++;
            }
        } else time++;
    }
}

void round_robin(Process processes[], int quantum) {
    int time = 0, completed = 0;
    int remaining[MAX_PROCESSES];
    for (int i = 0; i < process_count; i++) remaining[i] = processes[i].service_time;
    while (completed < process_count) {
        for (int i = 0; i < process_count; i++) {
            if (processes[i].arrival_time <= time && remaining[i] > 0) {
                int execute_time = (remaining[i] < quantum) ? remaining[i] : quantum;
                time += execute_time;
                remaining[i] -= execute_time;
                if (remaining[i] == 0) {
                    processes[i].finish_time = time;
                    processes[i].turn_around_time = processes[i].finish_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turn_around_time - processes[i].service_time;
                    completed++;
                }
            }
        }
    }
}

void displayResults(Process processes[]) {
    printf("PID\tArrival\tService\tFinish\tTurnAround\tWaiting\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].service_time,
               processes[i].finish_time, processes[i].turn_around_time, processes[i].waiting_time);
    }
}

int main() {
    Process processes[MAX_PROCESSES];
    inputProcesses(processes);
    printf("Select scheduling algorithm:\n1. FCFS\n2. SJF\n3. SRT\n4. Round Robin\n");
    int choice, quantum;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            fcfs(processes);
            break;
        case 2:
            sjf(processes);
            break;
        case 3:
            srt(processes);
            break;
        case 4:
            printf("Enter time quantum: ");
            scanf("%d", &quantum);
            round_robin(processes, quantum);
            break;
        default:
            printf("Invalid choice\n");
            return 0;
    }
    displayResults(processes);
    return 0;
}
