#ifndef LINKEDLISTQUEUE_HPP
#define LINKEDLISTQUEUE_HPP

#include "PrintJob.hpp"
#include <iostream>

using namespace std;

// Node structure for linked list
struct Node {
    PrintJob job;
    Node* next;
    
    Node(const PrintJob& j) : job(j), next(nullptr) {}
};

class LinkedListQueue {
private:
    Node* frontA;  // Priority Queue (faculty/emergency)
    Node* rearA;
    Node* frontB;  // Normal Queue (student)
    Node* rearB;
    int sizeA;
    int sizeB;
    
public:
    LinkedListQueue() {
        frontA = rearA = nullptr;
        frontB = rearB = nullptr;
        sizeA = sizeB = 0;
    }
    
    ~LinkedListQueue() {
        // Clear Queue A
        while (frontA != nullptr) {
            Node* temp = frontA;
            frontA = frontA->next;
            delete temp;
        }
        
        // Clear Queue B
        while (frontB != nullptr) {
            Node* temp = frontB;
            frontB = frontB->next;
            delete temp;
        }
    }
    
    // Enqueue a job to the appropriate queue
    bool enqueue(int jobID, const string& userName, int pages, int priority) {
        PrintJob job(jobID, userName, pages, priority);
        
        // Validate checksum
        if (!job.validateChecksum()) {
            cout << "Error: Checksum validation failed for job " << jobID << endl;
            return false;
        }
        
        Node* newNode = new Node(job);
        
        if (priority == 1) {  // High priority - Queue A
            if (frontA == nullptr) {
                frontA = rearA = newNode;
            } else {
                rearA->next = newNode;
                rearA = newNode;
            }
            sizeA++;
            AuditLogger::logEnqueue(job, "Priority Queue A");
            cout << "Job " << jobID << " enqueued to Priority Queue A" << endl;
        } else {  // Normal priority - Queue B
            if (frontB == nullptr) {
                frontB = rearB = newNode;
            } else {
                rearB->next = newNode;
                rearB = newNode;
            }
            sizeB++;
            AuditLogger::logEnqueue(job, "Normal Queue B");
            cout << "Job " << jobID << " enqueued to Normal Queue B" << endl;
        }
        return true;
    }
    
    // VIP insert - insert at the front of Priority Queue A
    bool vipInsert(int jobID, const string& userName, int pages) {
        PrintJob job(jobID, userName, pages, 1);  // VIP jobs have high priority
        
        // Validate checksum
        if (!job.validateChecksum()) {
            cout << "Error: Checksum validation failed for VIP job " << jobID << endl;
            return false;
        }
        
        Node* newNode = new Node(job);
        
        if (frontA == nullptr) {
            frontA = rearA = newNode;
        } else {
            newNode->next = frontA;
            frontA = newNode;
        }
        sizeA++;
        
        ofstream logFile("audit.log", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            char buf[80];
            struct tm* timeinfo = localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
            logFile << "[" << buf << "] VIP INSERT - Job ID: " << job.jobID 
                    << " | User: " << job.userName 
                    << " | Pages: " << job.pages 
                    << endl;
            logFile.close();
        }
        
        cout << "VIP Job " << jobID << " inserted at front of Priority Queue A" << endl;
        return true;
    }
    
    // Dequeue from priority queue first, then normal queue
    bool dequeue() {
        if (frontA != nullptr) {  // Dequeue from Priority Queue A first
            Node* temp = frontA;
            PrintJob job = temp->job;
            frontA = frontA->next;
            if (frontA == nullptr) {
                rearA = nullptr;
            }
            delete temp;
            sizeA--;
            
            AuditLogger::logDequeue(job);
            cout << "Dequeued from Priority Queue A: ";
            job.display();
            return true;
        } else if (frontB != nullptr) {  // Then dequeue from Normal Queue B
            Node* temp = frontB;
            PrintJob job = temp->job;
            frontB = frontB->next;
            if (frontB == nullptr) {
                rearB = nullptr;
            }
            delete temp;
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
        if (frontA == nullptr) {
            cout << "Queue A is empty." << endl;
        } else {
            Node* current = frontA;
            while (current != nullptr) {
                current->job.display();
                current = current->next;
            }
        }
        
        cout << "\n========== NORMAL QUEUE B (Student) ==========" << endl;
        if (frontB == nullptr) {
            cout << "Queue B is empty." << endl;
        } else {
            Node* current = frontB;
            while (current != nullptr) {
                current->job.display();
                current = current->next;
            }
        }
        cout << "\nTotal Jobs - Queue A: " << sizeA << ", Queue B: " << sizeB << endl;
        cout << "========================================================\n" << endl;
    }
    
    // Cancel a specific job by ID
    bool cancelJob(int jobID) {
        // Search in Queue A
        if (frontA != nullptr && frontA->job.jobID == jobID) {
            Node* temp = frontA;
            PrintJob job = temp->job;
            frontA = frontA->next;
            if (frontA == nullptr) {
                rearA = nullptr;
            }
            delete temp;
            sizeA--;
            
            AuditLogger::logCancel(job);
            cout << "Job " << jobID << " cancelled from Priority Queue A" << endl;
            return true;
        }
        
        Node* current = frontA;
        while (current != nullptr && current->next != nullptr) {
            if (current->next->job.jobID == jobID) {
                Node* temp = current->next;
                PrintJob job = temp->job;
                current->next = temp->next;
                if (temp == rearA) {
                    rearA = current;
                }
                delete temp;
                sizeA--;
                
                AuditLogger::logCancel(job);
                cout << "Job " << jobID << " cancelled from Priority Queue A" << endl;
                return true;
            }
            current = current->next;
        }
        
        // Search in Queue B
        if (frontB != nullptr && frontB->job.jobID == jobID) {
            Node* temp = frontB;
            PrintJob job = temp->job;
            frontB = frontB->next;
            if (frontB == nullptr) {
                rearB = nullptr;
            }
            delete temp;
            sizeB--;
            
            AuditLogger::logCancel(job);
            cout << "Job " << jobID << " cancelled from Normal Queue B" << endl;
            return true;
        }
        
        current = frontB;
        while (current != nullptr && current->next != nullptr) {
            if (current->next->job.jobID == jobID) {
                Node* temp = current->next;
                PrintJob job = temp->job;
                current->next = temp->next;
                if (temp == rearB) {
                    rearB = current;
                }
                delete temp;
                sizeB--;
                
                AuditLogger::logCancel(job);
                cout << "Job " << jobID << " cancelled from Normal Queue B" << endl;
                return true;
            }
            current = current->next;
        }
        
        cout << "Error: Job " << jobID << " not found in any queue!" << endl;
        return false;
    }
    
    // Get queue sizes
    int getQueueASize() const { return sizeA; }
    int getQueueBSize() const { return sizeB; }
};

#endif // LINKEDLISTQUEUE_HPP
