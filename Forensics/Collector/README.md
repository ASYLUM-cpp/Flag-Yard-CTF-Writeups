
# Collector

## Description
The antivirus has detected multiple files being dropped on the machine.  
We need to identify the downloaded files and retrieve the flag.

---

## Approach

### 1. Initial Exploration
- Looked through **Windows Defender Operational Logs** → nothing useful.  
- Checked **PowerShell Operational Logs**, saw lots of Base64 blobs → decoded them, but only gibberish.  
- Wasted ~2 days on these before realizing they were just noise.

### 2. Pivoting to BITS
- Discovered that attackers often abuse **BITS (Background Intelligent Transfer Service)** for stealthy downloads.  
- Located the **BITS Operational Logs** in the challenge dataset.

### 3. Filtering Events
- Focused on **Event IDs 5 and 3** (BITS job creation & completion).  
- Noticed that **job titles** contained fragments of the flag, scrambled letter by letter.

### 4. Reconstruction
- Wrote a small Python script to parse the logs and extract the `JobTitle` fields.  
- Concatenated and reordered the pieces.  
- The scrambled fragments resolved into the final flag.

---

## Example Script

```python

    
    

import Evtx.Evtx as evtx
import xml.etree.ElementTree as ET

def extract_bits_job_titles(evtx_file):
    namespace = {'ns': 'http://schemas.microsoft.com/win/2004/08/events/event'}
    job_titles = []

    with evtx.Evtx(evtx_file) as log:
        for record in log.records():
            xml_str = record.xml()
            root = ET.fromstring(xml_str)
            
            event_id_elem = root.find('./ns:System/ns:EventID', namespace)
            if event_id_elem is not None and event_id_elem.text == '5':
                for data in root.findall('./ns:EventData/ns:Data', namespace):
                    if data.attrib.get('Name') == 'jobTitle':
                        job_titles.append(data.text)

    print(''.join(job_titles))

extract_bits_job_titles("Microsoft-Windows-Bits-Client%4Operational.evtx")
