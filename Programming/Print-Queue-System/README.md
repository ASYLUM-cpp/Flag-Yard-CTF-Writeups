# Secure Multi-Tier Print Queue Management System

## Overview
This is a comprehensive implementation of a secure print queue management system with both array-based and linked-list implementations. The system includes encryption, checksum validation, and audit logging features.

## Features

### 1. Job Structure (`PrintJob.hpp`)
- **Job ID**: Unique identifier for each print job
- **User Name**: Original username (stored for display)
- **Encrypted User Name**: Caesar cipher encrypted username (shift +3)
- **Pages**: Number of pages to print
- **Priority Level**: 
  - Priority 1: High priority (Faculty/Emergency) - Queue A
  - Priority 2: Normal priority (Student) - Queue B
- **Checksum**: Sum of ASCII values of encrypted name for validation

### 2. Security Features

#### Caesar Cipher Encryption
- Shift value: +3
- Encrypts both uppercase and lowercase letters
- Preserves non-alphabetic characters
- Reversible decryption function

#### Checksum Validation
- Calculated as sum of ASCII values of encrypted username
- Validated on job creation and display
- Prevents data corruption and tampering

#### Audit Logging
- All operations logged to `audit.log` with timestamps
- Logged operations: ENQUEUE, DEQUEUE, CANCEL, VIP INSERT
- Includes job details: ID, user, pages, priority

### 3. Queue Implementations

#### Array-Based Queue (`ArrayQueue.hpp`)
- Two circular queues with fixed size (default: 100)
- Queue A: Priority queue for faculty/emergency jobs
- Queue B: Normal queue for student jobs
- Operations:
  - `enqueue(jobID, userName, pages, priority)`: Add job to appropriate queue
  - `dequeue()`: Remove job (priority queue first)
  - `displayQueues()`: Show all jobs in both queues
  - `cancelJob(jobID)`: Remove specific job by ID
  - `getQueueASize()`, `getQueueBSize()`: Get queue sizes

#### Linked-List Queue (`LinkedListQueue.hpp`)
- Dynamic node-based implementation
- Queue A: Priority queue (linked list)
- Queue B: Normal queue (linked list)
- All array queue operations plus:
  - `vipInsert(jobID, userName, pages)`: Insert at front of priority queue
- No size limit (dynamic allocation)

### 4. Queue Priority Logic
- Dequeue operations always prioritize Queue A (high priority) over Queue B
- VIP insert (linked-list only) adds jobs to the front of Queue A
- Cancel operations search both queues and remove the specified job

## Building and Running

### Using Makefile (Recommended)
```bash
# Build the program
make

# Build and run
make run

# Clean compiled files and logs
make clean

# Display help
make help
```

### Manual Compilation
```bash
g++ -std=c++11 -Wall -o print_queue main.cpp
```

### Execution
```bash
./print_queue
```

## File Structure
```
Print-Queue-System/
├── PrintJob.hpp           # Job structure and security utilities
├── ArrayQueue.hpp         # Array-based queue implementation
├── LinkedListQueue.hpp    # Linked-list queue implementation
├── main.cpp              # Demonstration program
├── Makefile              # Build automation
├── README.md             # This file
└── audit.log             # Generated audit log (after running)
```

## Example Output

### Caesar Cipher Encryption
```
Original: Alice
Encrypted: Dolfh
Decrypted: Alice
Checksum: 493
```

### Queue Display
```
========== PRIORITY QUEUE A (Faculty/Emergency) ==========
Job ID: 101 | User: Alice (Encrypted: Dolfh) | Pages: 10 | Priority: HIGH | Checksum: 493 [VALID]
Job ID: 103 | User: Charlie (Encrypted: Fkduolh) | Pages: 15 | Priority: HIGH | Checksum: 717 [VALID]

========== NORMAL QUEUE B (Student) ==========
Job ID: 102 | User: Bob (Encrypted: Ere) | Pages: 5 | Priority: NORMAL | Checksum: 284 [VALID]
```

### Audit Log Sample
```
[2025-10-08 04:17:21] ENQUEUE - Queue: Priority Queue A | Job ID: 101 | User: Alice | Pages: 10 | Priority: HIGH
[2025-10-08 04:17:21] DEQUEUE - Job ID: 101 | User: Alice | Pages: 10 | Priority: HIGH
[2025-10-08 04:17:21] CANCEL - Job ID: 104 | User: Diana | Pages: 8 | Priority: NORMAL
[2025-10-08 04:17:21] VIP INSERT - Job ID: 999 | User: VIPUser | Pages: 100
```

## Implementation Details

### Array Queue
- Uses circular array implementation to efficiently use space
- Fixed maximum size prevents overflow
- O(1) enqueue and dequeue operations
- O(n) cancel operation (requires shifting)

### Linked-List Queue
- Dynamic memory allocation
- No size limit
- O(1) enqueue, dequeue, and VIP insert operations
- O(n) cancel operation (requires traversal)
- Additional VIP insert feature for urgent jobs

## Security Considerations

1. **Encryption**: All usernames are encrypted using Caesar cipher before storage
2. **Integrity**: Checksum validation ensures data hasn't been corrupted
3. **Accountability**: Complete audit trail of all operations
4. **Access Control**: Two-tier priority system separates faculty and student jobs

## Testing

The `main.cpp` file includes comprehensive demonstrations:
1. Array-based queue with all operations
2. Linked-list queue with VIP insert feature
3. Security features (encryption, checksum validation)
4. Multiple test cases for each operation

Run the program to see all features in action!
