#include <iostream>
#include <fstream>
using namespace std;

const int maxsize = 100;

struct printjob {
    int jobid;
    char username[50];
    int pages;
    int priority;
    int checksum;
};

class printqueue {
private:
    printjob queuea[maxsize];
    printjob queueb[maxsize];
    int fronta, reara, sizea;
    int frontb, rearb, sizeb;
    
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
    
    int calculatechecksum(printjob job) {
        int sum = job.jobid + job.pages + job.priority;
        for (int i = 0; job.username[i] != '\0'; i++) {
            sum += job.username[i];
        }
        return sum % 1000;
    }
    
    bool validatechecksum(printjob job) {
        int calculated = calculatechecksum(job);
        return calculated == job.checksum;
    }
    
    void logaudit(const char* action, printjob job) {
        ofstream logfile("audit_log.txt", ios::app);
        if (logfile.is_open()) {
            char decryptedname[50];
            int idx = 0;
            for (int i = 0; job.username[i] != '\0' && i < 49; i++) {
                decryptedname[i] = job.username[i];
                idx = i + 1;
            }
            decryptedname[idx] = '\0';
            decryptusername(decryptedname);
            
            logfile << action << " - jobid: " << job.jobid 
                    << ", user: " << decryptedname 
                    << ", pages: " << job.pages 
                    << ", priority: " << job.priority << endl;
            logfile.close();
        }
    }
    
    void savechecksum(printjob job) {
        ofstream checksumfile("checksums.txt", ios::app);
        if (checksumfile.is_open()) {
            checksumfile << "jobid: " << job.jobid << ", checksum: " << job.checksum << endl;
            checksumfile.close();
        }
    }

public:
    printqueue() {
        fronta = 0; reara = -1; sizea = 0;
        frontb = 0; rearb = -1; sizeb = 0;
    }
    
    void enqueue(int jobid, const char* name, int pages, int priority) {
        printjob newjob;
        newjob.jobid = jobid;
        newjob.pages = pages;
        newjob.priority = priority;
        
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            newjob.username[i] = name[i];
            i++;
        }
        newjob.username[i] = '\0';
        
        encryptusername(newjob.username);
        newjob.checksum = calculatechecksum(newjob);
        savechecksum(newjob);
        
        if (priority == 1) {
            if (sizea < maxsize) {
                reara = (reara + 1) % maxsize;
                queuea[reara] = newjob;
                sizea++;
                logaudit("enqueued to priority queue", newjob);
                cout << "job added to priority queue" << endl;
            } else {
                cout << "priority queue full" << endl;
            }
        } else {
            if (sizeb < maxsize) {
                rearb = (rearb + 1) % maxsize;
                queueb[rearb] = newjob;
                sizeb++;
                logaudit("enqueued to normal queue", newjob);
                cout << "job added to normal queue" << endl;
            } else {
                cout << "normal queue full" << endl;
            }
        }
    }
    
    void dequeue() {
        if (sizea > 0) {
            printjob job = queuea[fronta];
            if (validatechecksum(job)) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; job.username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = job.username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "printing job from priority queue:" << endl;
                cout << "jobid: " << job.jobid << ", user: " << decryptedname 
                     << ", pages: " << job.pages << endl;
                
                logaudit("dequeued from priority queue", job);
                fronta = (fronta + 1) % maxsize;
                sizea--;
            } else {
                cout << "checksum validation failed for jobid: " << job.jobid << endl;
            }
        } else if (sizeb > 0) {
            printjob job = queueb[frontb];
            if (validatechecksum(job)) {
                char decryptedname[50];
                int idx = 0;
                for (int i = 0; job.username[i] != '\0' && i < 49; i++) {
                    decryptedname[i] = job.username[i];
                    idx = i + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "printing job from normal queue:" << endl;
                cout << "jobid: " << job.jobid << ", user: " << decryptedname 
                     << ", pages: " << job.pages << endl;
                
                logaudit("dequeued from normal queue", job);
                frontb = (frontb + 1) % maxsize;
                sizeb--;
            } else {
                cout << "checksum validation failed for jobid: " << job.jobid << endl;
            }
        } else {
            cout << "both queues are empty" << endl;
        }
    }
    
    void displayqueues() {
        cout << "\npriority queue (queue a):" << endl;
        if (sizea == 0) {
            cout << "empty" << endl;
        } else {
            for (int i = 0; i < sizea; i++) {
                int index = (fronta + i) % maxsize;
                printjob job = queuea[index];
                char decryptedname[50];
                int idx = 0;
                for (int j = 0; job.username[j] != '\0' && j < 49; j++) {
                    decryptedname[j] = job.username[j];
                    idx = j + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "jobid: " << job.jobid << ", user: " << decryptedname 
                     << ", pages: " << job.pages << ", priority: " << job.priority << endl;
            }
        }
        
        cout << "\nnormal queue (queue b):" << endl;
        if (sizeb == 0) {
            cout << "empty" << endl;
        } else {
            for (int i = 0; i < sizeb; i++) {
                int index = (frontb + i) % maxsize;
                printjob job = queueb[index];
                char decryptedname[50];
                int idx = 0;
                for (int j = 0; job.username[j] != '\0' && j < 49; j++) {
                    decryptedname[j] = job.username[j];
                    idx = j + 1;
                }
                decryptedname[idx] = '\0';
                decryptusername(decryptedname);
                
                cout << "jobid: " << job.jobid << ", user: " << decryptedname 
                     << ", pages: " << job.pages << ", priority: " << job.priority << endl;
            }
        }
        cout << endl;
    }
    
    void canceljob(int jobid) {
        bool found = false;
        
        for (int i = 0; i < sizea; i++) {
            int index = (fronta + i) % maxsize;
            if (queuea[index].jobid == jobid) {
                logaudit("cancelled from priority queue", queuea[index]);
                
                for (int j = i; j < sizea - 1; j++) {
                    int currentindex = (fronta + j) % maxsize;
                    int nextindex = (fronta + j + 1) % maxsize;
                    queuea[currentindex] = queuea[nextindex];
                }
                reara = (reara - 1 + maxsize) % maxsize;
                sizea--;
                found = true;
                cout << "job " << jobid << " cancelled from priority queue" << endl;
                break;
            }
        }
        
        if (!found) {
            for (int i = 0; i < sizeb; i++) {
                int index = (frontb + i) % maxsize;
                if (queueb[index].jobid == jobid) {
                    logaudit("cancelled from normal queue", queueb[index]);
                    
                    for (int j = i; j < sizeb - 1; j++) {
                        int currentindex = (frontb + j) % maxsize;
                        int nextindex = (frontb + j + 1) % maxsize;
                        queueb[currentindex] = queueb[nextindex];
                    }
                    rearb = (rearb - 1 + maxsize) % maxsize;
                    sizeb--;
                    found = true;
                    cout << "job " << jobid << " cancelled from normal queue" << endl;
                    break;
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
    
    cout << "print queue management system (array implementation)" << endl;
    cout << "=====================================================" << endl;
    
    while (true) {
        cout << "\nmenu:" << endl;
        cout << "1. add demo jobs" << endl;
        cout << "2. add custom job" << endl;
        cout << "3. print next job (dequeue)" << endl;
        cout << "4. display all queues" << endl;
        cout << "5. cancel job" << endl;
        cout << "6. exit" << endl;
        cout << "enter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            pq.enqueue(101, "alice", 5, 1);
            pq.enqueue(102, "bob", 10, 0);
            pq.enqueue(103, "charlie", 3, 1);
            pq.enqueue(104, "david", 7, 0);
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
            pq.dequeue();
        } else if (choice == 4) {
            pq.displayqueues();
        } else if (choice == 5) {
            int jobid;
            cout << "enter job id to cancel: ";
            cin >> jobid;
            pq.canceljob(jobid);
        } else if (choice == 6) {
            cout << "exiting..." << endl;
            break;
        } else {
            cout << "invalid choice" << endl;
        }
    }
    
    return 0;
}
