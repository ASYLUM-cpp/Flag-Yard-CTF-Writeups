#ifndef PRINTJOB_HPP
#define PRINTJOB_HPP

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

// Caesar cipher encryption utility
class CaesarCipher {
public:
    // Encrypt a string using Caesar cipher with shift +3
    static string encrypt(const string& plaintext) {
        string encrypted = "";
        for (char c : plaintext) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                encrypted += char((c - base + 3) % 26 + base);
            } else {
                encrypted += c;
            }
        }
        return encrypted;
    }
    
    // Decrypt a string using Caesar cipher with shift -3
    static string decrypt(const string& ciphertext) {
        string decrypted = "";
        for (char c : ciphertext) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                decrypted += char((c - base - 3 + 26) % 26 + base);
            } else {
                decrypted += c;
            }
        }
        return decrypted;
    }
    
    // Calculate checksum (sum of ASCII values of encrypted name)
    static int calculateChecksum(const string& encryptedName) {
        int checksum = 0;
        for (char c : encryptedName) {
            checksum += static_cast<int>(c);
        }
        return checksum;
    }
};

// Print Job Structure
struct PrintJob {
    int jobID;
    string encryptedUserName;
    string userName;  // Original name (for display purposes)
    int pages;
    int priority;  // 1 = high priority (faculty/emergency), 2 = normal (student)
    int checksum;
    
    PrintJob() : jobID(0), pages(0), priority(2), checksum(0) {}
    
    PrintJob(int id, const string& user, int p, int prio) 
        : jobID(id), userName(user), pages(p), priority(prio) {
        encryptedUserName = CaesarCipher::encrypt(user);
        checksum = CaesarCipher::calculateChecksum(encryptedUserName);
    }
    
    // Validate checksum
    bool validateChecksum() const {
        return checksum == CaesarCipher::calculateChecksum(encryptedUserName);
    }
    
    // Display job information
    void display() const {
        cout << "Job ID: " << jobID 
             << " | User: " << userName 
             << " (Encrypted: " << encryptedUserName << ")"
             << " | Pages: " << pages 
             << " | Priority: " << (priority == 1 ? "HIGH" : "NORMAL")
             << " | Checksum: " << checksum;
        if (validateChecksum()) {
            cout << " [VALID]";
        } else {
            cout << " [INVALID]";
        }
        cout << endl;
    }
};

// Audit logging utility
class AuditLogger {
private:
    static string getTimestamp() {
        time_t now = time(0);
        char buf[80];
        struct tm* timeinfo = localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buf);
    }
    
public:
    static void logDequeue(const PrintJob& job) {
        ofstream logFile("audit.log", ios::app);
        if (logFile.is_open()) {
            logFile << "[" << getTimestamp() << "] DEQUEUE - Job ID: " << job.jobID 
                    << " | User: " << job.userName 
                    << " | Pages: " << job.pages 
                    << " | Priority: " << (job.priority == 1 ? "HIGH" : "NORMAL")
                    << endl;
            logFile.close();
        }
    }
    
    static void logCancel(const PrintJob& job) {
        ofstream logFile("audit.log", ios::app);
        if (logFile.is_open()) {
            logFile << "[" << getTimestamp() << "] CANCEL - Job ID: " << job.jobID 
                    << " | User: " << job.userName 
                    << " | Pages: " << job.pages 
                    << " | Priority: " << (job.priority == 1 ? "HIGH" : "NORMAL")
                    << endl;
            logFile.close();
        }
    }
    
    static void logEnqueue(const PrintJob& job, const string& queueType) {
        ofstream logFile("audit.log", ios::app);
        if (logFile.is_open()) {
            logFile << "[" << getTimestamp() << "] ENQUEUE - Queue: " << queueType 
                    << " | Job ID: " << job.jobID 
                    << " | User: " << job.userName 
                    << " | Pages: " << job.pages 
                    << " | Priority: " << (job.priority == 1 ? "HIGH" : "NORMAL")
                    << endl;
            logFile.close();
        }
    }
};

#endif // PRINTJOB_HPP
