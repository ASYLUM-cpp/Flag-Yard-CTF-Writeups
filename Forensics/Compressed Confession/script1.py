import base64

def main():
    # The hex dump as a string (without the address and ASCII parts)
    hex_dump = """
        43 00 3a 00 5c 00 55 00  73 00 65 00 72 00 73 00
        5c 00 46 00 6c 00 61 00  67 00 59 00 61 00 72 00
        64 00 5c 00 5a 00 32 00  46 00 79 00 59 00 6d 00
        46 00 6e 00 5a 00 51 00  70 00 46 00 62 00 6d 00
        4e 00 76 00 5a 00 47 00  6c 00 75 00 5a 00 7a 00
        6f 00 67 00 57 00 45 00  39 00 53 00 49 00 48 00
        64 00 70 00 64 00 47 00  67 00 67 00 4d 00 44 00
        45 00 4b 00 52 00 57 00  35 00 6a 00 62 00 32 00
        52 00 6c 00 5a 00 43 00  42 00 47 00 62 00 47 00
        46 00 6e 00 4f 00 69 00  42 00 48 00 62 00 57 00
        42 00 6d 00 57 00 48 00  70 00 69 00 4d 00 54 00
        45 00 7a 00 59 00 47 00  51 00 32 00 59 00 7a 00
        41 00 34 00 5a 00 44 00  67 00 35 00 4d 00 57 00
        4a 00 6e 00 4d 00 54 00  5a 00 6c 00 5a 00 47 00
        4e 00 6a 00 4e 00 44 00  52 00 69 00 4f 00 57 00
        55 00 30 00 4e 00 6a 00  55 00 30 00 4d 00 58 00
        77 00 3d 00 2e 00 7a 00  69 00 70 00 00 00
    """

    # Remove whitespace and split into list of hex bytes
    hex_bytes = hex_dump.split()
    
    # Convert hex to bytes (ignore the null bytes for UTF-16LE)
    # Since it's UTF-16LE, we take every other byte starting from the first
    ascii_bytes = bytes.fromhex(''.join(hex_bytes))
    
    # Decode as UTF-16LE to get the full string
    full_string = ascii_bytes.decode('utf-16le')
    print("Full decoded string:")
    print(full_string)
    print()

    # The string is a Windows path: extract the Base64 part (between the last backslash and ".zip")
    # The path format: C:\Users\FlagYard\<base64_string>.zip
    base64_str = full_string.split('\\')[-1][:-4]  # Remove the ".zip" at the end
    print("Base64 string extracted:")
    print(base64_str)
    print(f"Length of Base64 string: {len(base64_str)} bytes")
    print()

    # Decode the Base64 string
    try:
        decoded_bytes = base64.b64decode(base64_str)
        decoded_str = decoded_bytes.decode('utf-8')
        print("Decoded Base64 string:")
        print(decoded_str)
    except Exception as e:
        print(f"Error decoding Base64: {e}")

if __name__ == "__main__":
    main()