#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int mod26(int x)
{
    return (x % 26 + 26) % 26;
}

int det(int key[2][2])
{
    return mod26(key[0][0] * key[1][1] - key[0][1] * key[1][0]);
}

int modInverse(int a)
{
    a = mod26(a);
    for (int i = 1; i < 26; i++)
    {
        if ((a * i) % 26 == 1) return i;
    }
    return -1;
}

string normalize(string text)
{
    string ans = "";
    for (char c : text)
    {
        if (isalpha(c)) ans += toupper(c);
    }
    if (ans.size() % 2 == 1) ans += 'X';
    return ans;
}

bool inverseKey(int key[2][2], int inv[2][2]) {
    int d = det(key);
    int invDet = modInverse(d);

    if (invDet == -1) return false;

    inv[0][0] = mod26( key[1][1] * invDet);
    inv[0][1] = mod26(-key[0][1] * invDet);
    inv[1][0] = mod26(-key[1][0] * invDet);
    inv[1][1] = mod26( key[0][0] * invDet);

    return true;
}

string encrypt(string text, int key[2][2])
{
    string ans = "";
    for (int i = 0; i < text.size(); i += 2)
    {
        int x = text[i] - 'A';
        int y = text[i + 1] - 'A';
        int c1 = mod26(key[0][0] * x + key[0][1] * y);
        int c2 = mod26(key[1][0] * x + key[1][1] * y);        
        ans += char(c1 + 'A');
        ans += char(c2 + 'A'); 
    }
    return ans;
}

string decrypt(string text, int key[2][2])
{
    int inv[2][2];
    if (!inverseKey(key, inv)) return "Khong co ma tran nghich dao!\n";
    string ans = "";
    for (int i = 0; i < text.size(); i += 2)
    {
        int x = text[i] - 'A';
        int y = text[i + 1] - 'A';
        int p1 = mod26(inv[0][0] * x + inv[0][1] * y);
        int p2 = mod26(inv[1][0] * x + inv[1][1] * y);        
        ans += char(p1 + 'A');
        ans += char(p2 + 'A'); 
    }
    return ans; 
}

int main() {
    int choice;
    string text;
    int key[2][2];
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "Choose: ";
    cin >> choice;
    cout << "Enter key matrix (2x2):\n";
    cout << "k11 k12: ";
    cin >> key[0][0] >> key[0][1];
    cout << "k21 k22: ";
    cin >> key[1][0] >> key[1][1];

    cin.ignore(); 

    cout << "Enter text: ";
    getline(cin, text);

    text = normalize(text); 

    if (choice == 1) {
        string cipher = encrypt(text, key);
        cout << "Ciphertext: " << cipher << endl;
    }
    else if (choice == 2) {
        string plain = decrypt(text, key);
        cout << "Plaintext: " << plain << endl;
    }
    else {
        cout << "Invalid choice\n";
    }
}