from Crypto.Cipher import DES

def bytes_to_binary(data):
    number = int.from_bytes(data, 'big')
    binary_string = format(number, '064b')
    return binary_string

def hamming_distance(bin1, bin2):
    count = 0
    for b1, b2 in zip(bin1, bin2):
        if b1 != b2:
            count += 1
    return count

def avalanche_test(key):
    p1 = b"STAYHOME"
    p2 = b"STAYHOMA"
    
    cipher = DES.new(key, DES.MODE_ECB)
    
    c1 = cipher.encrypt(p1)
    c2 = cipher.encrypt(p2)
    
    bin_c1 = bytes_to_binary(c1)
    bin_c2 = bytes_to_binary(c2)
    
    diff_bits = hamming_distance(bin_c1, bin_c2)
    total_bits = 64
    percent = (diff_bits / total_bits) * 100
    
    print("==================================================")
    print(f"Key dùng để mã hóa: {key}")
    print(f"P1: {p1}")
    print(f"P2: {p2}")
    print(f"Ciphertext 1 (hex): {c1.hex()}")
    print(f"Ciphertext 2 (hex): {c2.hex()}")
    print(f"Ciphertext 1 (bin): {bin_c1}")
    print(f"Ciphertext 2 (bin): {bin_c2}")
    print(f"So bit khac nhau: {diff_bits}")
    print(f"Ty le thay doi: {percent:.2f}%")
    
    return diff_bits, percent

def main():
    keys = [
        b"87654321",
        b"12345678",
        b"11223344",
        b"55667788"
    ]
    
    for key in keys:
        avalanche_test(key)

if __name__ == "__main__":
    main()