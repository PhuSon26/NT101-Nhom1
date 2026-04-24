import base64

def main():
    message = "The University of Information Technology."

    # 3 public key dùng để mã hóa
    keys = {
        "Key 1": (7, 187),
        "Key 2": (17, 13588476140342208394395166469647627226674348541791),
        "Key 3": (
            int("0D88C3", 16),
            int("E103ABD94892E3E74AFD724BF28E78366D9676BCCC70118BD0AA1968DBB143D1", 16)
        )
    }

    # đổi chuỗi thành dãy số 2 chữ số / ký tự
    # a-z -> 00..25
    # A-Z -> 26..51
    # space -> 62
    # . -> 63
    number_string = ""

    for ch in message:
        if 'a' <= ch <= 'z':
            number_string += str(ord(ch) - ord('a')).zfill(2)
        elif 'A' <= ch <= 'Z':
            number_string += str(ord(ch) - ord('A') + 26).zfill(2)
        elif ch == ' ':
            number_string += "62"
        elif ch == '.':
            number_string += "63"

    for name, (e, n) in keys.items():
        # tìm số ký tự tối đa trong 1 block sao cho block < n
        max_chars = 0
        biggest = 0
        while biggest * 100 + 66 < n:
            biggest = biggest * 100 + 66
            max_chars += 1

        digits_per_block = max_chars * 2
        temp = number_string

        # nếu block cuối chưa đủ thì thêm khoảng trắng
        while len(temp) % digits_per_block != 0:
            temp += "62"

        # chia block và mã hóa RSA
        cipher_bytes = b""
        block_bytes = (n.bit_length() + 7) // 8

        for i in range(0, len(temp), digits_per_block):
            block = int(temp[i:i + digits_per_block])
            cipher = pow(block, e, n)   # C = M^e mod n
            cipher_bytes += cipher.to_bytes(block_bytes, byteorder="big")

        # đổi sang Base64
        cipher_base64 = base64.b64encode(cipher_bytes).decode()

        print(name)
        print(cipher_base64)
        print()


if __name__ == "__main__":
    main()