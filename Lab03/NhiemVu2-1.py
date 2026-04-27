import math
import base64

# ================= KEY =================
def get_keys(p, q, e):
    n = p * q
    phi = (p - 1) * (q - 1)
    d = pow(e, -1, phi)
    return (e, n), (d, n)

# 1. Khóa
p1, q1, e1 = 11, 17, 7

p2, q2, e2 = 20079993872842322116151219, 676717145751736242170789, 17

p3 = int("F7E75FDC469067FFDC4E847C51F452DF", 16)
q3 = int("E85CED54AF57E53E092113E62F436F4F", 16)
e3 = int("0D88C3", 16)

public_key1, private_key1 = get_keys(p1, q1, e1)
public_key2, private_key2 = get_keys(p2, q2, e2)
public_key3, private_key3 = get_keys(p3, q3, e3)

print("=== KEYS ===")
print("PU1:", public_key1)
print("PR1:", private_key1)
print("PU2:", public_key2)
print("PR2:", private_key2)
print("PU3:", public_key3)
print("PR3:", private_key3)

# ================= PART 2 =================
print("\n=== PART 2 ===")
M = 5
e, n = public_key1
d, _ = private_key1

# Confidentiality
c_conf = pow(M, e, n)
m_conf = pow(c_conf, d, n)

# Authentication
c_auth = pow(M, d, n)
m_auth = pow(c_auth, e, n)

print("Confidentiality:", c_conf, "->", m_conf)
print("Authentication:", c_auth, "->", m_auth)

# ================= BLOCK RSA =================
def rsa_encrypt_message(message, public_key):
    e, n = public_key
    block_size = (n.bit_length() // 8) - 1

    message_bytes = message.encode()
    blocks = [message_bytes[i:i+block_size] for i in range(0, len(message_bytes), block_size)]

    encrypted_blocks = []
    for block in blocks:
        m_int = int.from_bytes(block, 'big')
        c = pow(m_int, e, n)
        encrypted_blocks.append(c.to_bytes((c.bit_length()+7)//8, 'big'))

    return base64.b64encode(b"||".join(encrypted_blocks)).decode()

def rsa_decrypt_message(cipher_b64, private_key):
    d, n = private_key
    blocks = base64.b64decode(cipher_b64).split(b"||")

    message = b""
    for block in blocks:
        c_int = int.from_bytes(block, 'big')
        m_int = pow(c_int, d, n)
        m_bytes = m_int.to_bytes((m_int.bit_length()+7)//8, 'big')
        message += m_bytes

    return message.decode(errors='ignore')

# ================= PART 3 =================
print("\n=== PART 3 ===")
message = "The University of Information Technology"

cipher_b64 = rsa_encrypt_message(message, public_key2)
print("Ciphertext (Base64):", cipher_b64)

# Test giải lại
print("Decrypted:", rsa_decrypt_message(cipher_b64, private_key2))

# ================= PART 4 =================
print("\n=== PART 4 ===")

def rsa_decrypt(c_int, private_key):
    d, n = private_key
    return pow(c_int, d, n)

def int_to_text(m_int):
    try:
        m_bytes = m_int.to_bytes((m_int.bit_length()+7)//8, 'big')
        return m_bytes.decode(errors='ignore')
    except:
        return None

def try_all_keys(c_int):
    keys = [private_key1, private_key2, private_key3]
    for i, key in enumerate(keys, 1):
        m = rsa_decrypt(c_int, key)
        text = int_to_text(m)
        print(f"\nKey {i}:")
        print("Recovered:", text)

# ===== (1) Base64 =====
print("\n--- C1 ---")
c1_bytes = base64.b64decode("raUcesUlOkx/8ZhgodMoo0Uu18sC20yXlQFevSu7W/FDxIy0YRHMyXcHdD9PBvIT2aUft5fCQEGomiVVPv4I")
c1 = int.from_bytes(c1_bytes, 'big')
try_all_keys(c1)

# ===== (2) Hex =====
print("\n--- C2 ---")
c2 = int("C87F570FC4F699CEC24020C6F54221ABAB2CE0C3", 16)
try_all_keys(c2)

# ===== (3) Base64 =====
print("\n--- C3 ---")
c3_bytes = base64.b64decode("Z2BUSkJcg0w4XEpgm0JcMExEQmBlVH6dYEpNTHpMHptMQ7NgTHlgQrNMQ2BKTQ==")
c3 = int.from_bytes(c3_bytes, 'big')
try_all_keys(c3)

# ===== (4) Binary =====
print("\n--- C4 ---")
c4 = int("001010000001010011111111101101110010111011001010111011000110011110111111001111110110100011001111001100001001010001010100111101010100110011101110111011110101101100000100", 2)
try_all_keys(c4)