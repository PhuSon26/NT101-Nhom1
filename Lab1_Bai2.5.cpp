#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string normalize(string text) {
    string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            result += toupper(c);
        }
    }
    return result;
}

string generateKey(string text, string key) {
    string newKey = "";
    int m = key.length();

    for (int i = 0; i < text.length(); i++) {
        newKey += toupper(key[i % m]);
    }

    return newKey;
}

string encrypt(string plain, string key) {
    string cipher = "";

    for (int i = 0; i < plain.length(); i++) {
        int p = plain[i] - 'A';
        int k = key[i] - 'A';

        char c = (p + k) % 26 + 'A';
        cipher += c;
    }

    return cipher;
}

string decrypt(string cipher, string key) {
    string plain = "";

    for (int i = 0; i < cipher.length(); i++) {
        int c = cipher[i] - 'A';
        int k = key[i] - 'A';

        char p = (c - k + 26) % 26 + 'A';
        plain += p;
    }

    return plain;
}

int main() {
    int choice;
    string text, key;

    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "Choose: ";
    cin >> choice;
    cin.ignore();

    cout << "Enter key: ";
    getline(cin, key);

    cout << "Enter text: ";
    getline(cin, text);

    text = normalize(text);
    key = normalize(key);

    string newKey = generateKey(text, key);

    if (choice == 1) {
        string cipher = encrypt(text, newKey);
        cout << "Ciphertext: " << cipher << endl;
    }
    else if (choice == 2) {
        string plain = decrypt(text, newKey);
        cout << "Plaintext: " << plain << endl;
    }
    else {
        cout << "Invalid choice\n";
    }
}