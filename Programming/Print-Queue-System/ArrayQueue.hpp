#ifndef ARRAYQUEUE_HPP
#define ARRAYQUEUE_HPP

#include "PrintJob.hpp"
#include <iostream>

using namespace std;

const int MAX_QUEUE_SIZE = 100;

class ArrayQueue {
private:
    PrintJob* queueA;  // Priority Queue (faculty/emergency)
    PrintJob* queueB;  // Normal Queue (student)
    int frontA, rearA, sizeA;
    int frontB, rearB, sizeB;
    int maxSize;
    
public:
    ArrayQueue(int size = MAX_QUEUE_SIZE) : maxSize(size) {
        queueA = new PrintJob[maxSize];
        queueB = new PrintJob[maxSize];
        frontA = rearA = sizeA = 0;
        frontB = rearB = sizeB = 0;
    }
    
    ~ArrayQueue() {
        delete[] queueA;
        delete[] queueB;
    }
    
    // Enqueue a job to the appropriate queue
    bool enqueue(int jobID, const string& userName, int pages, int priority) {
        PrintJob job(jobID, userName, pages, priority);
        
        // Validate checksum
        if (!job.validateChecksum()) {
            cout << "Error: Checksum validation failed for job " << jobID << endl;
            return false;
        }
        
        if (priority == 1) {  // High priority - Queue A
            if (sizeA >= maxSize) {
                cout << "Error: Priority Queue A is full!" << endl;
                return false;
            }
            queueA[rearA] = job;
            rearA = (rearA + 1) % maxSize;
            sizeA++;
            AuditLogger::logEnqueue(job, "Priority Queue A");
            cout << "Job " << jobID << " enqueued to Priority Queue A" << endl;
        } else {  // Normal priority - Queue B
            if (sizeB >= maxSize) {
                cout << "Error: Normal Queue B is full!" << endl;
                return false;
            }
            queueB[rearB] = job;
            rearB = (rearB + 1) % maxSize;
            sizeB++;
            AuditLogger::logEnqueue(job, "Normal Queue B");
            cout << "Job " << jobID << " enqueued to Normal Queue B" << endl;
        }
        return true;
    }
    
    // Dequeue from priority queue first, then normal queue
    bool dequeue() {
        PrintJob job;
        
        if (sizeA > 0) {  // Dequeue from Priority Queue A first
            job = queueA[frontA];
            frontA = (frontA + 1) % maxSize;
            sizeA--;
            AuditLogger::logDequeue(job);
            cout << "Dequeued from Priority Queue A: ";
            job.display();
            return true;
        } else if (sizeB > 0) {  // Then dequeue from Normal Queue B
            job = queueB[frontB];
            frontB = (frontB + 1) % maxSize;
            sizeB--;
            AuditLogger::logDequeue(job);
            cout << "Dequeued from Normal Queue B: ";
            job.display();
            return true;
        } else {
            cout << "Error: Both queues are empty!" << endl;
            return false;
        }
    }
    
    // Display all jobs in both queues
    void displayQueues() const {
        cout << "\n========== PRIORITY QUEUE A (Faculty/Emergency) ==========" << endl;
        if (sizeA == 0) {
            cout << "Queue A is empty." << endl;
        } else {
            int count = 0;
            for (int i = frontA; count < sizeA; i = (i + 1) % maxSize, count++) {
                queueA[i].display();
            }
        }
        
        cout << "\n========== NORMAL QUEUE B (Student) ==========" << endl;
        if (sizeB == 0) {
            cout << "Queue B is empty." << endl;
        } else {
            int count = 0;
            for (int i = frontB; count < sizeB; i = (i + 1) % maxSize, count++) {
                queueB[i].display();
            }
        }
        cout << "\nTotal Jobs - Queue A: " << sizeA << ", Queue B: " << sizeB << endl;
        cout << "========================================================\n" << endl;
    }
    
    // Cancel a specific job by ID
    bool cancelJob(int jobID) {
        // Search in Queue A
        int count = 0;
        for (int i = frontA; count < sizeA; i = (i + 1) % maxSize, count++) {
            if (queueA[i].jobID == jobID) {
                PrintJob job = queueA[i];
                AuditLogger::logCancel(job);
                
                // Shift elements to remove the job
                int j = i;
                for (int k = 0; k < sizeA - count - 1; k++) {
                    int next = (j + 1) % maxSize;
                    queueA[j] = queueA[next];
                    j = next;
                }
                rearA = (rearA - 1 + maxSize) % maxSize;
                sizeA--;
                
                cout << "Job " << jobID << " cancelled from Priority Queue A" << endl;
                return true;
            }
        }
        
        // Search in Queue B
        count = 0;
        for (int i = frontB; count < sizeB; i = (i + 1) % maxSize, count++) {
            if (queueB[i].jobID == jobID) {
                PrintJob job = queueB[i];
                AuditLogger::logCancel(job);
                
                // Shift elements to remove the job
                int j = i;
                for (int k = 0; k < sizeB - count - 1; k++) {
                    int next = (j + 1) % maxSize;
                    queueB[j] = queueB[next];
                    j = next;
                }
                rearB = (rearB - 1 + maxSize) % maxSize;
                sizeB--;
                
                cout << "Job " << jobID << " cancelled from Normal Queue B" << endl;
                return true;
            }
        }
        
        cout << "Error: Job " << jobID << " not found in any queue!" << endl;
        return false;
    }
    
    // Get queue sizes
    int getQueueASize() const { return sizeA; }
    int getQueueBSize() const { return sizeB; }
};

#endif // ARRAYQUEUE_HPP
