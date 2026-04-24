import math

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