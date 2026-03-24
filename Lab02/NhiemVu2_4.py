from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad

def SEED_Lab():
    data = get_random_bytes(1000)
    key = get_random_bytes(16)
    print("Data: \n", data)
    print("Key: \n", key)
    cipher = AES.new(key, AES.MODE_CBC)
    ciphertext = cipher.encrypt(pad(data, AES.block_size))
    print("Ciphertext: \n", ciphertext)
    
    ciphertext = bin(int.from_bytes(ciphertext, 'big'))
    ciphertext = ciphertext ^ (1 << 26)
    print("Ciphertext sau khi lam hong ban ma: \n", ciphertext)

    plain = AES.new(key, AES.MODE_CBC, cipher.iv)
    

def main():
    SEED_Lab()

if __name__ == "__main__":
    main()