

Challenge Disc : 
You are provided with a forensic triage image containing only the user-level registry hives NTUSER.DAT and UsrClass.dat. Your objective is to identify the archive file that the intruder created for staging exfiltration data, and obtain the hidden flag.

i downloaded the file
extracted it
in C/users/flagyard there is NTUSER.DAT 
i opened it in reg editor

navigated to software/7zip and upon clicking on 7zip i found in the ArcHistory registry the path the looked sort of like a base 64

<img width="1395" height="737" alt="image" src="https://github.com/user-attachments/assets/38c7649a-dd6a-4774-9a05-90dd3a378803" />

i extracted it by hovering over the key name compression and clicking in file extract saving the output to a txt and copying it
i gave it to gpt and extarcted the individual bytes

using the script1 i extracted the base 64 decoded string
and script2 decoded it using the hidden message (Run the script to find out)

the purpose behind using script was that while using cyberchef it gave the wrong output and decoded it very badly

the encoding used was UTF-16Le rather then the common UTF-8 encoding

so after decoding the final hash we get the flag

#do it yourself
