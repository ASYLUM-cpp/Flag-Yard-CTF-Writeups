


encoded = ""
decoded = "".join(chr(ord(c) ^ 0x01) for c in encoded)
print(decoded)

