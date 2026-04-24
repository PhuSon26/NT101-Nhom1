#include <iostream>
#include <string>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

// Hàm đổi ký tự hex -> số
int hexValue(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Đọc số lớn từ chuỗi ở hệ 10 hoặc 16
cpp_int parseBigInt(const string& s, int base = 10) {
    cpp_int result = 0;
    for (char c : s) {
        if (c == ' ' || c == '\n' || c == '\t') continue;
        int digit;
        if (base == 10) {
            if (!isdigit(c)) continue;
            digit = c - '0';
        } else if (base == 16) {
            digit = hexValue(c);
            if (digit == -1) continue;
        } else {
            throw runtime_error("Base not supported");
        }
        result *= base;
        result += digit;
    }
    return result;
}

// Đổi cpp_int sang chuỗi hex in hoa
string toHex(cpp_int x) {
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        int digit = (int)(x & 0xF);
        if (digit < 10) s.push_back('0' + digit);
        else s.push_back('A' + digit - 10);
        x >>= 4;
    }
    reverse(s.begin(), s.end());
    return s;
}

// Extended Euclid: tìm x, y sao cho ax + by = gcd(a, b)
cpp_int extendedGCD(cpp_int a, cpp_int b, cpp_int &x, cpp_int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    cpp_int x1, y1;
    cpp_int g = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// Nghịch đảo modulo: tìm d sao cho e*d ≡ 1 (mod phi)
cpp_int modInverse(cpp_int e, cpp_int phi) {
    cpp_int x, y;
    cpp_int g = extendedGCD(e, phi, x, y);
    if (g != 1) {
        throw runtime_error("Khong ton tai nghich dao modulo");
    }
    x %= phi;
    if (x < 0) x += phi;
    return x;
}

void solveRSA(const cpp_int& p, const cpp_int& q, const cpp_int& e, bool printHex = false) {
    cpp_int n = p * q;
    cpp_int phi = (p - 1) * (q - 1);
    cpp_int d = modInverse(e, phi);

    if (!printHex) {
        cout << "p   = " << p << "\n";
        cout << "q   = " << q << "\n";
        cout << "e   = " << e << "\n";
        cout << "n   = " << n << "\n";
        cout << "phi = " << phi << "\n";
        cout << "d   = " << d << "\n";
        cout << "PU  = (" << e << ", " << n << ")\n";
        cout << "PR  = (" << d << ", " << n << ")\n";
    } else {
        cout << "p   = " << toHex(p) << "\n";
        cout << "q   = " << toHex(q) << "\n";
        cout << "e   = " << toHex(e) << "\n";
        cout << "n   = " << toHex(n) << "\n";
        cout << "phi = " << toHex(phi) << "\n";
        cout << "d   = " << toHex(d) << "\n";
        cout << "PU  = (" << toHex(e) << ", " << toHex(n) << ")\n";
        cout << "PR  = (" << toHex(d) << ", " << toHex(n) << ")\n";
    }
    cout << "----------------------------------------\n";
}

int main() {
    try {
        // Bộ 1 (hệ 10)
        cpp_int p1 = 11;
        cpp_int q1 = 17;
        cpp_int e1 = 7;

        // Bộ 2 (hệ 10)
        cpp_int p2 = parseBigInt("20079993872842322116151219", 10);
        cpp_int q2 = parseBigInt("676717145751736242170789", 10);
        cpp_int e2 = 17;

        // Bộ 3 (hệ 16)
        cpp_int p3 = parseBigInt("F7E75FDC469067FFDC4E847C51F452DF", 16);
        cpp_int q3 = parseBigInt("E85CED54AF57E53E092113E62F436F4F", 16);
        cpp_int e3 = parseBigInt("0D88C3", 16);

        cout << "=== CASE 1 (DECIMAL) ===\n";
        solveRSA(p1, q1, e1, false);

        cout << "=== CASE 2 (DECIMAL) ===\n";
        solveRSA(p2, q2, e2, false);

        cout << "=== CASE 3 (HEXADECIMAL) ===\n";
        solveRSA(p3, q3, e3, true);
    }
    catch (const exception& ex) {
        cerr << "Loi: " << ex.what() << endl;
    }

    return 0;
}