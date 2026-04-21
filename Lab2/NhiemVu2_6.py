import random
import math

def miller_rabin(n, k = 5):
    if n < 2:
        return False
    s = 0
    d = n - 1
    while d % 2 == 0:
        d //= 2
        s += 1
    for i in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for j in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def top10_below_mersene():
    top10 = []
    cnt = 0
    Mp = 2**89 - 1
    Mp -= 1
    while cnt < 10:
        if miller_rabin(Mp):
            top10.append(Mp)
            cnt += 1
        Mp -= 1
    return top10

def generate_rd_prime():
    choice = int(input("1. Prime 8 bits\n2. Prime 16 bits\n3. Prime 64 bits\n"))
    if choice == 1:
        while True:
            x = random.getrandbits(8)
            if miller_rabin(x):
                break           
        print("So nguyen to 8 bits: ", x)
    elif choice == 2:
        while True:
            x = random.getrandbits(16)
            if miller_rabin(x):
                break           
        print("So nguyen to 16 bits: ", x)
    elif choice == 3:
        while True:
            x = random.getrandbits(64)
            if miller_rabin(x):
                break           
        print("So nguyen to 64 bits: ", x)
    else:
        print("Gia tri nhap khong hop le!")

def largest_gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def mod_exp(a, x, p):
    ans = 1
    a = a % p
    while x > 0:
        if x % 2 == 1:
            ans = (ans * a) % p
        a = (a * a) % p
        x //= 2
    return ans

def main():
    # Sinh số nguyên tố ngẫu nhiên
    generate_rd_prime()
    
    # 10 số nguyên tố lớn nhất < Mersenne thứ 10
    print("\n10 số nguyên tố lớn nhất < Mersenne thứ 10:")
    for i, p in enumerate(top10_below_mersene(), 1):
        print(f"{i}: {p}")
    
    # Kiểm tra số nguyên tố tùy ý
    n = int(input("\nNhập số nguyên cần kiểm tra (< 2^89-1): "))
    if miller_rabin(n):
        print(f"{n} là số nguyên tố (có khả năng)")
    else:
        print(f"{n} không phải là số nguyên tố")
    
    # Tính GCD
    a = int(input("\nNhập số nguyên thứ nhất: "))
    b = int(input("Nhập số nguyên thứ hai: "))
    print(f"GCD({a}, {b}) = {largest_gcd(a, b)}")
    
    # Tính lũy thừa modulo
    a = int(input("\nNhập cơ số a: "))
    x = int(input("Nhập số mũ x: "))
    p = int(input("Nhập modulo p: "))
    print(f"{a}^{x} mod {p} = {mod_exp(a, x, p)}")

if __name__ == "__main__":
    main()