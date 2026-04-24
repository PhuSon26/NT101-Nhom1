import base64


def ma_hoa_rsa(text, e, n):
    # Bước 1: đổi thông điệp thành chuỗi số
    # a-z  -> 00..25
    # A-Z  -> 26..51
    # " "  -> 62
    # "."  -> 63

    chuoi_so = ""

    for ky_tu in text:
        if "a" <= ky_tu <= "z":
            ma = ord(ky_tu) - ord("a")
            chuoi_so += str(ma).zfill(2)

        elif "A" <= ky_tu <= "Z":
            ma = ord(ky_tu) - ord("A") + 26
            chuoi_so += str(ma).zfill(2)

        elif ky_tu == " ":
            chuoi_so += "62"

        elif ky_tu == ".":
            chuoi_so += "63"

    # Bước 2: tìm xem mỗi block chứa tối đa bao nhiêu ký tự
    # để khi đổi sang số thì block đó vẫn nhỏ hơn n
    so_ky_tu_toi_da = 0
    so_lon_nhat = 0

    while so_lon_nhat * 100 + 66 < n:
        so_lon_nhat = so_lon_nhat * 100 + 66
        so_ky_tu_toi_da += 1

    # Mỗi ký tự là 2 chữ số
    do_dai_block = so_ky_tu_toi_da * 2

    # Bước 3: nếu block cuối chưa đủ thì thêm khoảng trắng
    while len(chuoi_so) % do_dai_block != 0:
        chuoi_so += "62"

    # Bước 4: chia block và mã hóa từng block bằng RSA
    tat_ca_bytes = b""

    # số byte cần để chứa 1 ciphertext block
    so_byte_moi_block = (n.bit_length() + 7) // 8

    for i in range(0, len(chuoi_so), do_dai_block):
        block_text = chuoi_so[i:i + do_dai_block]
        m = int(block_text)

        # công thức RSA: C = M^e mod n
        c = pow(m, e, n)

        # đổi số c sang bytes để lát nữa encode Base64
        tat_ca_bytes += c.to_bytes(so_byte_moi_block, byteorder="big")

    # Bước 5: đổi toàn bộ sang Base64
    ket_qua_base64 = base64.b64encode(tat_ca_bytes).decode()
    return ket_qua_base64


def main():
    thong_diep = "The University of Information Technology."

    # 3 public key
    e1 = 7
    n1 = 187

    e2 = 17
    n2 = 13588476140342208394395166469647627226674348541791

    e3 = int("0D88C3", 16)
    n3 = int("E103ABD94892E3E74AFD724BF28E78366D9676BCCC70118BD0AA1968DBB143D1", 16)

    print("Key 1:")
    print(ma_hoa_rsa(thong_diep, e1, n1))
    print()

    print("Key 2:")
    print(ma_hoa_rsa(thong_diep, e2, n2))
    print()

    print("Key 3:")
    print(ma_hoa_rsa(thong_diep, e3, n3))
    print()


if __name__ == "__main__":
    main()