


encoded = "Gm`fXzb113`d6c08d891bg16edcc44b9e46541|"
decoded = "".join(chr(ord(c) ^ 0x01) for c in encoded)
print(decoded)
