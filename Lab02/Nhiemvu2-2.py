from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Util import Counter
from binascii import hexlify


plaintext = "UIT_LAB_UIT_LAB_UIT_LAB_UIT_LAB_".encode()


key = b'1234567890abcdef'


iv = b'0000000000000000'


padded = pad(plaintext, 16)

# ===== ECB =====
cipher_ecb = AES.new(key, AES.MODE_ECB)
ciphertext_ecb = cipher_ecb.encrypt(padded)

# ===== CBC =====
cipher_cbc = AES.new(key, AES.MODE_CBC, iv)
ciphertext_cbc = cipher_cbc.encrypt(padded)

# ===== CFB =====
cfb = AES.new(key, AES.MODE_CFB, iv)
ct_cfb = cfb.encrypt(plaintext)

# ===== OFB =====
ofb = AES.new(key, AES.MODE_OFB, iv)
ct_ofb = ofb.encrypt(plaintext)

# ===== CTR =====
ctr = AES.new(key, AES.MODE_CTR, counter=Counter.new(128))
ct_ctr = ctr.encrypt(plaintext)


def show(name, data):
    print(f"\n{name}:")
    print(hexlify(data).decode())

show("ECB", ciphertext_ecb)
show("CBC", ciphertext_cbc)
show("CFB", ct_cfb)
show("OFB", ct_ofb)
show("CTR", ct_ctr)