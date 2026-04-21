from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad

BLOCK_SIZE = 16

def count_corrupted_blocks(p1, p2):
    blocks1 = [p1[i:i+BLOCK_SIZE] for i in range(0, len(p1), BLOCK_SIZE)]
    blocks2 = [p2[i:i+BLOCK_SIZE] for i in range(0, len(p2), BLOCK_SIZE)]

    corrupted = 0
    for b1, b2 in zip(blocks1, blocks2):
        if b1 != b2:
            corrupted += 1
    return corrupted

def flip_bit(ciphertext, bit_pos):
    num = int.from_bytes(ciphertext, 'big')
    num ^= (1 << bit_pos)
    return num.to_bytes(len(ciphertext), 'big')

def test_mode(mode_name, mode):
    print(f"\n===== MODE: {mode_name} =====")

    data = get_random_bytes(1000)
    key = get_random_bytes(16)

    # ===== MÃ HÓA =====
    if mode_name == "ECB":
        cipher = AES.new(key, mode)
        ciphertext = cipher.encrypt(pad(data, BLOCK_SIZE))
        iv = None
    else:
        cipher = AES.new(key, mode)
        ciphertext = cipher.encrypt(pad(data, BLOCK_SIZE))
        iv = cipher.iv

    print("\n--- Ciphertext ---")
    print(ciphertext)

    # ===== LÀM HỎNG =====
    corrupted_cipher = flip_bit(ciphertext, 26)

    print("\n--- Ciphertext sau khi flip bit ---")
    print(corrupted_cipher)

    # ===== GIẢI MÃ =====
    if mode_name == "ECB":
        cipher_dec = AES.new(key, mode)
    else:
        cipher_dec = AES.new(key, mode, iv)

    try:
        decrypted = unpad(cipher_dec.decrypt(corrupted_cipher), BLOCK_SIZE)
        print("\n--- Plaintext ---")
        print(decrypted)
    except:
        decrypted = cipher_dec.decrypt(corrupted_cipher)
        print("\n--- Plaintext lỗi (raw, chua unpad) ---")
        print(decrypted)

    # ===== SO SÁNH =====
    original = pad(data, BLOCK_SIZE)
    corrupted_blocks = count_corrupted_blocks(original, decrypted)

    print("\nSo block bi hong:", corrupted_blocks)

def main():
    test_mode("ECB", AES.MODE_ECB)
    test_mode("CBC", AES.MODE_CBC)
    test_mode("CFB", AES.MODE_CFB)
    test_mode("OFB", AES.MODE_OFB)

if __name__ == "__main__":
    main()