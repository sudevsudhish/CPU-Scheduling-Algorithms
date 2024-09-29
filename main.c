#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** Global Constants **/
const char* TRACE = "trace";
const char* SHOW_STATISTICS = "stats";
const char* ALGORITHMS[9] = {"", "FCFS", "RR-", "SPN", "SRT"};

typedef struct {
    char name[10];
    int arrivalTime;
    int serviceTime;
} Process;

Process processes[100];
int process_count = 0;
int last_instant = 100;
char timeline[100][100];
int finishTime[100];
int turnAroundTime[100];
double normTurn[100];

// Sort functions in C
int sortByServiceTime(const void* a, const void* b) {
    Process* pa = (Process*)a;
    Process* pb = (Process*)b;
    return pa->serviceTime - pb->serviceTime;
}

int sortByArrivalTime(const void* a, const void* b) {
    Process* pa = (Process*)a;
    Process* pb = (Process*)b;
    return pa->arrivalTime - pb->arrivalTime;
}

void clear_timeline() {
    for (int i = 0; i < last_instant; i++)
        for (int j = 0; j < process_count; j++)
            timeline[i][j] = ' ';
}

char* getProcessName(Process* a) {
    return a->name;
}

int getArrivalTime(Process* a) {
    return a->arrivalTime;
}

int getServiceTime(Process* a) {
    return a->serviceTime;
}

double calculate_response_ratio(int wait_time, int service_time) {
    return (wait_time + service_time) * 1.0 / service_time;
}

void fillInWaitTime() {
    for (int i = 0; i < process_count; i++) {
        int arrivalTime = getArrivalTime(&processes[i]);
        for (int k = arrivalTime; k < finishTime[i]; k++) {
            if (timeline[k][i] != '*')
                timeline[k][i] = '.';
        }
    }
}

void firstComeFirstServe() {
    int time = getArrivalTime(&processes[0]);
    for (int i = 0; i < process_count; i++) {
        int arrivalTime = getArrivalTime(&processes[i]);
        int serviceTime = getServiceTime(&processes[i]);

        finishTime[i] = time + serviceTime;
        turnAroundTime[i] = finishTime[i] - arrivalTime;
        normTurn[i] = turnAroundTime[i] * 1.0 / serviceTime;

        for (int j = time; j < finishTime[i]; j++)
            timeline[j][i] = '*';
        for (int j = arrivalTime; j < time; j++)
            timeline[j][i] = '.';
        time += serviceTime;
    }
}

void roundRobin(int originalQuantum) {
    int currentQuantum = originalQuantum;
    int time = 0, j = 0;

    while (j < process_count) {
        int processIndex = j;
        int arrivalTime = getArrivalTime(&processes[processIndex]);
        int serviceTime = getServiceTime(&processes[processIndex]);

        if (arrivalTime <= time) {
            int remainingServiceTime = serviceTime - currentQuantum;

            for (int t = time; t < time + currentQuantum && remainingServiceTime > 0; t++) {
                timeline[t][processIndex] = '*';
            }
            time += currentQuantum;

            if (remainingServiceTime > 0) {
                currentQuantum = remainingServiceTime;
            } else {
                finishTime[processIndex] = time;
                turnAroundTime[processIndex] = finishTime[processIndex] - arrivalTime;
                normTurn[processIndex] = turnAroundTime[processIndex] * 1.0 / serviceTime;
                j++;
            }
        }
    }

    fillInWaitTime();
}

void printAlgorithm(int algorithm_index) {
    int algorithm_id = ALGORITHMS[algorithm_index][0] - '0';
    printf("%s\n", ALGORITHMS[algorithm_id]);
}

void printProcesses() {
    printf("Process    ");
    for (int i = 0; i < process_count; i++)
        printf("|  %s  ", getProcessName(&processes[i]));
    printf("|\n");
}

void printArrivalTime() {
    printf("Arrival    ");
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", getArrivalTime(&processes[i]));
    printf("|\n");
}

void printServiceTime() {
    printf("Service    |");
    for (int i = 0; i < process_count; i++)
        printf("%3d  |", getServiceTime(&processes[i]));
    printf(" Mean|\n");
}

void printFinishTime() {
    printf("Finish     ");
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", finishTime[i]);
    printf("|-----|\n");
}

void printTurnAroundTime() {
    printf("Turnaround |");
    int sum = 0;
    for (int i = 0; i < process_count; i++) {
        printf("%3d  |", turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    printf(" %2.2f|\n", (1.0 * sum) / process_count);
}

void printNormTurn() {
    printf("NormTurn   |");
    float sum = 0;
    for (int i = 0; i < process_count; i++) {
        printf("%2.2f|", normTurn[i]);
        sum += normTurn[i];
    }
    printf(" %2.2f|\n", sum / process_count);
}

void printStats(int algorithm_index) {
    printAlgorithm(algorithm_index);
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
    printNormTurn();
}

void execute_algorithm(char algorithm_id, int quantum, const char* operation) {
    switch (algorithm_id) {
        case '1':
            if (strcmp(operation, TRACE) == 0) printf("FCFS  ");
            firstComeFirstServe();
            break;
        case '2':
            if (strcmp(operation, TRACE) == 0) printf("RR-%d  ", quantum);
            roundRobin(quantum);
            break;
        // Add other algorithms here
        default:
            break;
    }
}

int main() {
    // Parse input to populate processes array, process_count etc.
    // Here would go code for parsing input and populating processes

    for (int idx = 0; idx < 9; idx++) {
        clear_timeline();
        execute_algorithm(ALGORITHMS[idx][0], 4, TRACE);  // Example with quantum 4
        if (strcmp(TRACE, "trace") == 0) {
            printStats(idx);
        }
    }

    return 0;
}
