import math
import base64

def get_keys(p, q, e):
    n = p * q
    phi = (p - 1) * (q - 1)
    d = pow(e, -1, phi)
    
    return (e, n), (d, n)

# 1. Xác định khóa 
p1, q1, e1 = 11, 17, 7

p2,q2,e2  = 20079993872842322116151219, 676717145751736242170789, 17
p3 = int("F7E75FDC469067FFDC4E847C51F452DF", 16)
q3 = int("E85CED54AF57E53E092113E62F436F4F", 16)
e3 = int("0D88C3", 16)
public_key1, private_key1 = get_keys(p1, q1, e1)
public_key2, private_key2 = get_keys(p2, q2, e2)
public_key3, private_key3 = get_keys(p3, q3, e3)

print(f"Public Key (PU1): {public_key1}")
print(f"Private Key (PR1): {private_key1}")
print(f"Public Key (PU2): {public_key2}")
print(f"Private Key (PR2): {private_key2}")
print(f"Public Key (PU3): {public_key3}")
print(f"Private Key (PR3): {private_key3}")


# 2. Mã hóa và giải mã bản rõ M = 5
M = 5
e, n = public_key1
d, _ = private_key1

#Confidentiality
# Mã hóa bằng PU, giải mã bằng PR
c_conf = pow(M, e, n)
m_recovered_conf = pow(c_conf, d, n)

#Authentication
# Mã hóa bằng PR, giải mã bằng PU
c_auth = pow(M, d, n)
m_recovered_auth = pow(c_auth, e, n)

print(f"\nConfidentiality: Ciphertext={c_conf}, Recovered={m_recovered_conf}")
print(f"Authentication: Ciphertext={c_auth}, Recovered={m_recovered_auth}")

# 3. Sử dụng các khóa trên để mã hóa thông điệp sau: The University of Information Technology. Xác định bản mã dưới dạng Base64.
message = "The University of Information Technology"
message_byte = message.encode()
m_int = int.from_bytes(message_byte, byteorder='big')
e, n = public_key2
c = pow(m_int, e, n)
c_bytes = c.to_bytes((c.bit_length() + 7) // 8, byteorder='big')
c_base64 = base64.b64encode(c_bytes).decode()
print("Ciphertext (Base64): ", c_base64)

# 4.  Tìm bản rõ tương ứng của mỗi bản mã sau, biết rằng chúng được mã hóa bằng một trong ba khóa đã cho ở trên.
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

# ====== (1) Base64 ======
c1_bytes = base64.b64decode("raUcesUlOkx/8ZhgodMoo0Uu18sC20yXlQFevSu7W/FDxIy0YRHMyXcHdD9PBvIT2aUft5fCQEGomiVVPv4I")
c1 = int.from_bytes(c1_bytes, 'big')
try_all_keys(c1)

# ====== (2) Hex ======
c2 = int("C87F570FC4F699CEC24020C6F54221ABAB2CE0C3", 16)
try_all_keys(c2)

# ====== (3) Base64 ======
c3_bytes = base64.b64decode("Z2BUSkJcg0w4XEpgm0JcMExEQmBlVH6dYEpNTHpMHptMQ7NgTHlgQrNMQ2BKTQ==")
c3 = int.from_bytes(c3_bytes, 'big')
try_all_keys(c3)

# ====== (4) Binary ======
c4 = int("001010000001010011111111101101110010111011001010111011000110011110111111001111110110100011001111001100001001010001010100111101010100110011101110111011110101101100000100", 2)
try_all_keys(c4)