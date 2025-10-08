#include "PrintJob.hpp"
#include "ArrayQueue.hpp"
#include "LinkedListQueue.hpp"
#include <iostream>

using namespace std;

void demonstrateArrayQueue() {
    cout << "\n##########################################################" << endl;
    cout << "###     ARRAY-BASED QUEUE IMPLEMENTATION DEMO         ###" << endl;
    cout << "##########################################################\n" << endl;
    
    ArrayQueue aq;
    
    // Test encryption and checksum
    cout << "=== Testing Caesar Cipher Encryption ===" << endl;
    string testName = "Alice";
    string encrypted = CaesarCipher::encrypt(testName);
    string decrypted = CaesarCipher::decrypt(encrypted);
    int checksum = CaesarCipher::calculateChecksum(encrypted);
    cout << "Original: " << testName << endl;
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << decrypted << endl;
    cout << "Checksum: " << checksum << endl << endl;
    
    // Enqueue jobs
    cout << "=== Enqueueing Jobs ===" << endl;
    aq.enqueue(101, "Alice", 10, 1);      // Faculty - Priority Queue A
    aq.enqueue(102, "Bob", 5, 2);         // Student - Normal Queue B
    aq.enqueue(103, "Charlie", 15, 1);    // Emergency - Priority Queue A
    aq.enqueue(104, "Diana", 8, 2);       // Student - Normal Queue B
    aq.enqueue(105, "Eve", 3, 1);         // Faculty - Priority Queue A
    cout << endl;
    
    // Display queues
    aq.displayQueues();
    
    // Dequeue operations
    cout << "=== Dequeue Operations ===" << endl;
    aq.dequeue();  // Should dequeue from Queue A (Job 101)
    aq.dequeue();  // Should dequeue from Queue A (Job 103)
    cout << endl;
    
    // Display queues after dequeue
    aq.displayQueues();
    
    // Cancel a job
    cout << "=== Cancel Job Operation ===" << endl;
    aq.cancelJob(104);  // Cancel Diana's job from Queue B
    cout << endl;
    
    // Display queues after cancel
    aq.displayQueues();
    
    // Dequeue remaining jobs
    cout << "=== Dequeuing Remaining Jobs ===" << endl;
    while (aq.getQueueASize() > 0 || aq.getQueueBSize() > 0) {
        aq.dequeue();
    }
    cout << endl;
    
    // Display final state
    aq.displayQueues();
}

void demonstrateLinkedListQueue() {
    cout << "\n##########################################################" << endl;
    cout << "###   LINKED-LIST QUEUE IMPLEMENTATION DEMO           ###" << endl;
    cout << "##########################################################\n" << endl;
    
    LinkedListQueue llq;
    
    // Enqueue jobs
    cout << "=== Enqueueing Jobs ===" << endl;
    llq.enqueue(201, "Frank", 12, 1);     // Faculty - Priority Queue A
    llq.enqueue(202, "Grace", 7, 2);      // Student - Normal Queue B
    llq.enqueue(203, "Henry", 20, 2);     // Student - Normal Queue B
    llq.enqueue(204, "Iris", 5, 1);       // Emergency - Priority Queue A
    cout << endl;
    
    // Display queues
    llq.displayQueues();
    
    // VIP Insert - unique to linked list implementation
    cout << "=== VIP Insert Operation (Front of Priority Queue) ===" << endl;
    llq.vipInsert(999, "VIPUser", 100);   // VIP insert at front of Queue A
    cout << endl;
    
    // Display queues after VIP insert
    llq.displayQueues();
    
    // Dequeue operations
    cout << "=== Dequeue Operations ===" << endl;
    llq.dequeue();  // Should dequeue VIP job (999)
    llq.dequeue();  // Should dequeue Job 201
    cout << endl;
    
    // Display queues
    llq.displayQueues();
    
    // Cancel a job
    cout << "=== Cancel Job Operation ===" << endl;
    llq.cancelJob(203);  // Cancel Henry's job from Queue B
    cout << endl;
    
    // Display queues after cancel
    llq.displayQueues();
    
    // Dequeue remaining jobs
    cout << "=== Dequeuing Remaining Jobs ===" << endl;
    while (llq.getQueueASize() > 0 || llq.getQueueBSize() > 0) {
        llq.dequeue();
    }
    cout << endl;
    
    // Display final state
    llq.displayQueues();
}

void demonstrateSecurityFeatures() {
    cout << "\n##########################################################" << endl;
    cout << "###         SECURITY FEATURES DEMONSTRATION           ###" << endl;
    cout << "##########################################################\n" << endl;
    
    // Test Caesar cipher with various inputs
    cout << "=== Caesar Cipher Tests ===" << endl;
    string names[] = {"John", "MARY", "Test123", "Alice Bob"};
    for (const string& name : names) {
        string enc = CaesarCipher::encrypt(name);
        string dec = CaesarCipher::decrypt(enc);
        int cs = CaesarCipher::calculateChecksum(enc);
        cout << "Name: " << name << " -> Encrypted: " << enc 
             << " -> Decrypted: " << dec << " -> Checksum: " << cs << endl;
    }
    cout << endl;
    
    // Test checksum validation
    cout << "=== Checksum Validation Test ===" << endl;
    PrintJob validJob(301, "TestUser", 10, 1);
    cout << "Valid job: ";
    validJob.display();
    
    // Create a job and manually corrupt the checksum
    PrintJob corruptedJob(302, "BadUser", 5, 2);
    corruptedJob.checksum = 9999;  // Incorrect checksum
    cout << "Corrupted job: ";
    corruptedJob.display();
    cout << endl;
    
    // Show audit log contents
    cout << "=== Audit Log Summary ===" << endl;
    cout << "Check 'audit.log' file for complete audit trail of all operations." << endl;
    cout << "Audit log includes: ENQUEUE, DEQUEUE, CANCEL, and VIP INSERT operations." << endl;
}

int main() {
    cout << "\n" << endl;
    cout << "************************************************************" << endl;
    cout << "***  SECURE MULTI-TIER PRINT QUEUE MANAGEMENT SYSTEM    ***" << endl;
    cout << "************************************************************" << endl;
    
    // Demonstrate array-based queue
    demonstrateArrayQueue();
    
    // Demonstrate linked-list queue
    demonstrateLinkedListQueue();
    
    // Demonstrate security features
    demonstrateSecurityFeatures();
    
    cout << "\n************************************************************" << endl;
    cout << "***              DEMONSTRATION COMPLETE                  ***" << endl;
    cout << "************************************************************\n" << endl;
    
    return 0;
}
