#include <iostream>
#include <fstream>
using namespace std;

struct printjob {
    int jobid;
    char username[50];
    int pages;
    int priority;
    int checksum;
    printjob* next;
};

class printqueue {
private:
    printjob* heada;
    printjob* taila;
    printjob* headb;
    printjob* tailb;
    
    char encrypt(char c) {
        if (c >= 'a' && c <= 'z') {
            return ((c - 'a' + 3) % 26) + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            return ((c - 'A' + 3) % 26) + 'A';
        }
        return c;
    }
    
    char decrypt(char c) {
        if (c >= 'a' && c <= 'z') {
            return ((c - 'a' - 3 + 26) % 26) + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            return ((c - 'A' - 3 + 26) % 26) + 'A';
        }
        return c;
    }
    
    void encryptusername(char* name) {
        for (int i = 0; name[i] != '\0'; i++) {
            name[i] = encrypt(name[i]);
        }
    }
    
    void decryptusername(char* name) {
        for (int i = 0; name[i] != '\0'; i++) {
            name[i] = decrypt(name[i]);
        }
    }
    
    int calculatechecksum(printjob* job) {
        int sum = job->jobid + job->pages + job->priority;
        for (int i = 0; job->username[i] != '\0'; i++) {
            sum += job->username[i];
        }
        return sum % 1000;
    }
    
    bool validatechecksum(printjob* job) {
        int calculated = calculatechecksum(job);
        return calculated == job->checksum;
    }
    
    void logaudit(const char* action, printjob* job) {
        ofstream logfile("audit_log_ll.txt", ios::app);
        if (logfile.is_open()) {
            char decryptedname[50];
            int idx = 0;
            for (int i = 0; job->username[i] != '\0' && i < 49; i++) {
                decryptedname[i] = job->username[i];
                idx = i + 1;
            }
            decryptedname[idx] = '\0';
            decryptusername(decryptedname);
            
            logfile << action << " - jobid: " << job->jobid 
                    << ", user: " << decryptedname 
                    << ", pages: " << job->pages 
                    << ", priority: " << job->priority << endl;
            logfile.close();
        }
    }
    
    void savechecksum(printjob* job) {
        ofstream checksumfile("checksums_ll.txt", ios::app);
        if (checksumfile.is_open()) {
            checksumfile << "jobid: " << job->jobid << ", checksum: " << job->checksum << endl;
            checksumfile.close();
        }
    }

public:
    printqueue() {
        heada = nullptr;
        taila = nullptr;
        headb = nullptr;
        tailb = nullptr;
    }
    
    ~printqueue() {
        while (heada != nullptr) {
            printjob* temp = heada;
            heada = heada->next;
            delete temp;
        }
        while (headb != nullptr) {
            printjob* temp = headb;
            headb = headb->next;
            delete temp;
        }
    }
    
    void enqueue(int jobid, const char* name, int pages, int priority) {
        printjob* newjob = new printjob;
        newjob->jobid = jobid;
        newjob->pages = pages;
        newjob->priority = priority;
        newjob->next = nullptr;
        
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            newjob->username[i] = name[i];
            i++;
        }
        newjob->username[i] = '\0';
        
        encryptusername(newjob->username);
        newjob->checksum = calculatechecksum(newjob);
        savechecksum(newjob);
        
        if (priority == 1) {
            if (heada == nullptr) {
                heada = newjob;
                taila = newjob;
            } else {
                taila->next = newjob;
                taila = newjob;
            }
            logaudit("enqueued to priority queue", newjob);
            cout << "job added to priority queue" << endl;
        } else {
            if (headb == nullptr) {
                headb = newjob;
                tailb = newjob;
            } else {
                tailb->next = newjob;
                tailb = newjob;
            }
            logaudit("enqueued to normal queue", newjob);
            cout << "job added to normal queue" << endl;
        }
    }
    
    void vipinsert(int jobid, const char* name, int pages) {
        printjob* newjob = new printjob;
        newjob->jobid = jobid;
        newjob->pages = pages;
        newjob->priority = 2;
        newjob->next = nullptr;
        
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            newjob->username[i] = name[i];
            i++;
        }
        newjob->username[i] = '\0';
        
        encryptusername(newjob->username);
        newjob->checksum = calculatechecksum(newjob);
        savechecksum(newjob);
        
        newjob->next = heada;
        heada = newjob;
        if (taila == nullptr) {
            taila = newjob;
        }
        
        logaudit("vip inserted to priority queue", newjob);
        cout << "vip job inserted at front of priority queue" << endl;
    }
    
    void dequeue() {
        if (heada != nullptr) {
            printjob* job = heada;
            if (validatechecksum(job)) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; job->username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = job->username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "printing job from priority queue:" << endl;
                cout << "jobid: " << job->jobid << ", user: " << decryptedname 
                     << ", pages: " << job->pages << endl;
                
                logaudit("dequeued from priority queue", job);
                heada = heada->next;
                if (heada == nullptr) {
                    taila = nullptr;
                }
                delete job;
            } else {
                cout << "checksum validation failed for jobid: " << job->jobid << endl;
            }
        } else if (headb != nullptr) {
            printjob* job = headb;
            if (validatechecksum(job)) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; job->username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = job->username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "printing job from normal queue:" << endl;
                cout << "jobid: " << job->jobid << ", user: " << decryptedname 
                     << ", pages: " << job->pages << endl;
                
                logaudit("dequeued from normal queue", job);
                headb = headb->next;
                if (headb == nullptr) {
                    tailb = nullptr;
                }
                delete job;
            } else {
                cout << "checksum validation failed for jobid: " << job->jobid << endl;
            }
        } else {
            cout << "both queues are empty" << endl;
        }
    }
    
    void displayqueues() {
        cout << "\npriority queue (queue a):" << endl;
        if (heada == nullptr) {
            cout << "empty" << endl;
        } else {
            printjob* current = heada;
            while (current != nullptr) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; current->username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = current->username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "jobid: " << current->jobid << ", user: " << decryptedname 
                     << ", pages: " << current->pages << ", priority: " << current->priority << endl;
                current = current->next;
            }
        }
        
        cout << "\nnormal queue (queue b):" << endl;
        if (headb == nullptr) {
            cout << "empty" << endl;
        } else {
            printjob* current = headb;
            while (current != nullptr) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; current->username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = current->username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "jobid: " << current->jobid << ", user: " << decryptedname 
                     << ", pages: " << current->pages << ", priority: " << current->priority << endl;
                current = current->next;
            }
        }
        cout << endl;
    }
    
    void canceljob(int jobid) {
        bool found = false;
        
        if (heada != nullptr && heada->jobid == jobid) {
            logaudit("cancelled from priority queue", heada);
            printjob* temp = heada;
            heada = heada->next;
            if (heada == nullptr) {
                taila = nullptr;
            }
            delete temp;
            found = true;
            cout << "job " << jobid << " cancelled from priority queue" << endl;
        } else {
            printjob* current = heada;
            while (current != nullptr && current->next != nullptr) {
                if (current->next->jobid == jobid) {
                    logaudit("cancelled from priority queue", current->next);
                    printjob* temp = current->next;
                    current->next = current->next->next;
                    if (current->next == nullptr) {
                        taila = current;
                    }
                    delete temp;
                    found = true;
                    cout << "job " << jobid << " cancelled from priority queue" << endl;
                    break;
                }
                current = current->next;
            }
        }
        
        if (!found) {
            if (headb != nullptr && headb->jobid == jobid) {
                logaudit("cancelled from normal queue", headb);
                printjob* temp = headb;
                headb = headb->next;
                if (headb == nullptr) {
                    tailb = nullptr;
                }
                delete temp;
                found = true;
                cout << "job " << jobid << " cancelled from normal queue" << endl;
            } else {
                printjob* current = headb;
                while (current != nullptr && current->next != nullptr) {
                    if (current->next->jobid == jobid) {
                        logaudit("cancelled from normal queue", current->next);
                        printjob* temp = current->next;
                        current->next = current->next->next;
                        if (current->next == nullptr) {
                            tailb = current;
                        }
                        delete temp;
                        found = true;
                        cout << "job " << jobid << " cancelled from normal queue" << endl;
                        break;
                    }
                    current = current->next;
                }
            }
        }
        
        if (!found) {
            cout << "job " << jobid << " not found" << endl;
        }
    }
};

int main() {
    printqueue pq;
    int choice;
    
    cout << "print queue management system (linked list implementation)" << endl;
    cout << "==========================================================" << endl;
    
    while (true) {
        cout << "\nmenu:" << endl;
        cout << "1. add demo jobs" << endl;
        cout << "2. add custom job" << endl;
        cout << "3. add vip job (insert at front)" << endl;
        cout << "4. print next job (dequeue)" << endl;
        cout << "5. display all queues" << endl;
        cout << "6. cancel job" << endl;
        cout << "7. exit" << endl;
        cout << "enter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            pq.enqueue(201, "alice", 5, 1);
            pq.enqueue(202, "bob", 10, 0);
            pq.enqueue(203, "charlie", 3, 1);
            pq.enqueue(204, "david", 7, 0);
            cout << "demo jobs added" << endl;
        } else if (choice == 2) {
            int jobid, pages, priority;
            char username[50];
            
            cout << "enter job id: ";
            cin >> jobid;
            cout << "enter username: ";
            cin >> username;
            cout << "enter pages: ";
            cin >> pages;
            cout << "enter priority (1=high, 0=normal): ";
            cin >> priority;
            
            pq.enqueue(jobid, username, pages, priority);
        } else if (choice == 3) {
            int jobid, pages;
            char username[50];
            
            cout << "enter vip job id: ";
            cin >> jobid;
            cout << "enter username: ";
            cin >> username;
            cout << "enter pages: ";
            cin >> pages;
            
            pq.vipinsert(jobid, username, pages);
        } else if (choice == 4) {
            pq.dequeue();
        } else if (choice == 5) {
            pq.displayqueues();
        } else if (choice == 6) {
            int jobid;
            cout << "enter job id to cancel: ";
            cin >> jobid;
            pq.canceljob(jobid);
        } else if (choice == 7) {
            cout << "exiting..." << endl;
            break;
        } else {
            cout << "invalid choice" << endl;
        }
    }
    
    return 0;
}
