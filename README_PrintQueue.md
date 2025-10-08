# Secure Multi-Tier Print Queue Management System

This repository contains two implementations of a secure print queue management system with encryption, checksums, and audit logging.

## Files

### Array.cpp
Array-based implementation using circular arrays
- **Maximum capacity**: 100 jobs per queue
- **Data structure**: Circular array with front/rear pointers
- **Queues**: Priority Queue (Queue A) and Normal Queue (Queue B)

### LinkedList.cpp
Linked list-based implementation with dynamic memory allocation
- **Data structure**: Singly linked list with head/tail pointers
- **Queues**: Priority Queue (Queue A) and Normal Queue (Queue B)
- **Additional feature**: VIP job insertion at front of priority queue

## Features

### Security
- **Username Encryption**: Caesar cipher with shift +3
- **Username Decryption**: Caesar cipher with shift -3
- **Checksum Calculation**: `(jobID + pages + priority + sum of username chars) % 1000`
- **Checksum Validation**: Validated before printing each job

### Job Structure
Each print job contains:
- `jobID`: Unique identifier
- `userName`: User who submitted the job (encrypted)
- `pages`: Number of pages to print
- `priority`: 1 for high priority (Queue A), 0 for normal (Queue B), 2 for VIP
- `checksum`: Calculated checksum for validation

### Operations

#### Common Operations (Both Implementations)
1. **enqueue(jobID, userName, pages, priority)**: Add job to appropriate queue
2. **dequeue()**: Print and remove next job (priority queue first)
3. **displayQueues()**: Show all jobs in both queues with decrypted usernames
4. **cancelJob(jobID)**: Remove job from queue by ID

#### LinkedList-Only Operation
5. **vipinsert(jobID, userName, pages)**: Insert VIP job at front of priority queue

### Audit Logging
- **Array implementation**: Logs to `audit_log.txt`
- **LinkedList implementation**: Logs to `audit_log_ll.txt`
- **Log entries**: All enqueue, dequeue, cancel, and VIP insert operations with decrypted usernames

### Checksum Storage
- **Array implementation**: Saves to `checksums.txt`
- **LinkedList implementation**: Saves to `checksums_ll.txt`
- **Format**: `jobid: [ID], checksum: [value]`

## Compilation

```bash
# Compile Array implementation
g++ -o array_test Array.cpp

# Compile LinkedList implementation
g++ -o linkedlist_test LinkedList.cpp
```

## Usage

### Array Implementation
```bash
./array_test
```

Menu options:
1. Add demo jobs (4 pre-configured jobs)
2. Add custom job (manual input)
3. Print next job (dequeue)
4. Display all queues
5. Cancel job
6. Exit

### LinkedList Implementation
```bash
./linkedlist_test
```

Menu options:
1. Add demo jobs (4 pre-configured jobs)
2. Add custom job (manual input)
3. Add VIP job (insert at front)
4. Print next job (dequeue)
5. Display all queues
6. Cancel job
7. Exit

## Example Workflow

```
1. Add demo jobs
   - 2 jobs to priority queue (alice, charlie)
   - 2 jobs to normal queue (bob, david)

2. Display queues
   - Shows all jobs with decrypted usernames

3. Print jobs
   - Priority queue jobs print first
   - Checksums validated before printing

4. Cancel a job
   - Job removed from queue
   - Operation logged to audit file
```

## Implementation Details

### Caesar Cipher
- **Encryption**: Each letter shifted +3 positions in alphabet
  - `a` → `d`, `b` → `e`, `z` → `c`
  - Preserves case (uppercase/lowercase)
  - Non-alphabetic characters unchanged

### Circular Array (Array.cpp)
- Uses modulo arithmetic for wraparound
- Maintains front and rear indices
- Efficient O(1) enqueue and dequeue

### Linked List (LinkedList.cpp)
- Dynamic memory allocation with `new`/`delete`
- Proper cleanup in destructor
- VIP insertion at head for immediate priority

### Queue Priority
1. **Queue A (Priority)**: Always processes first
2. **Queue B (Normal)**: Processes only when Queue A is empty
3. **VIP Jobs**: Insert at front of Queue A (LinkedList only)

## Code Style
- Lowercase variable names
- No STL usage (pure C++ with standard library)
- Minimal comments
- Beginner-friendly implementation

## Requirements Met
✅ Two separate implementations (Array & LinkedList)  
✅ Caesar cipher encryption/decryption  
✅ Checksum calculation and validation  
✅ Audit logging to files  
✅ Priority queue processing  
✅ All required functions implemented  
✅ Interactive user menus  
✅ VIP insertion (LinkedList only)  
✅ No STL usage  
✅ Lowercase naming convention  
