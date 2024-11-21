#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
using namespace std;

struct Process {
    int arrival_time;
    int burst_time;
    int start_time = -1;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int remaining_time;
    bool isCompleted = false;
};

bool compareArrival(Process p1, Process p2) {
    return p1.arrival_time < p2.arrival_time;
}

void calculateFCFS(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrival_time) 
            currentTime = p.arrival_time;
        p.start_time = currentTime;
        p.completion_time = currentTime + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.response_time = p.start_time - p.arrival_time;
        currentTime = p.completion_time;
    }
}

void calculateSJF(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int idx = -1;
        int min_burst = 1e9;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= currentTime && !processes[i].isCompleted) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            if (processes[idx].start_time == -1)
                processes[idx].start_time = currentTime;
            currentTime += processes[idx].burst_time;
            processes[idx].completion_time = currentTime;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
            processes[idx].isCompleted = true;
            completed++;
        } else {
            currentTime++;
        }
    }
}

void calculateSRTF(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int idx = -1;
        int min_remaining = 1e9;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            if (processes[idx].start_time == -1)
                processes[idx].start_time = currentTime;
            currentTime++;
            processes[idx].remaining_time--;
            if (processes[idx].remaining_time == 0) {
                processes[idx].completion_time = currentTime;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
                completed++;
            }
        } else {
            currentTime++;
        }
    }
}

int main() {
    int n, choice;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter arrival time of process " << i + 1 << ": ";
        cin >> processes[i].arrival_time;
        cout << "Enter burst time of process " << i + 1 << ": ";
        cin >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    cout << "\nChoose scheduling algorithm: \n1. FCFS \n2. SJF (Non-preemptive) \n3. SRTF (Preemptive)\n";
    cin >> choice;

    if (choice == 1) {
        calculateFCFS(processes);
    } else if (choice == 2) {
        calculateSJF(processes);
    } else if (choice == 3) {
        calculateSRTF(processes);
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    float total_turnaround_time = 0, total_waiting_time = 0, total_response_time = 0;

    for (auto& p : processes) {
        total_turnaround_time += p.turnaround_time;
        total_waiting_time += p.waiting_time;
        total_response_time += p.response_time;
    }

    float avg_turnaround_time = total_turnaround_time / n;
    float avg_waiting_time = total_waiting_time / n;
    float avg_response_time = total_response_time / n;

    cout << "\nAT\tBT\tST\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++) {
        cout << processes[i].arrival_time << "\t" << processes[i].burst_time << "\t" << processes[i].start_time << "\t"
             << processes[i].completion_time << "\t" << processes[i].turnaround_time << "\t" << processes[i].waiting_time << "\t"
             << processes[i].response_time << "\n";
    }
    cout << "\nAverage Turnaround Time = " << avg_turnaround_time;
    cout << "\nAverage Waiting Time = " << avg_waiting_time;
    cout << "\nAverage Response Time = " << avg_response_time << endl;

    return 0;
}

