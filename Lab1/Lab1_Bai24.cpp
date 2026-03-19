#include<iostream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>

    using namespace std;

class PlayfairCipher
{
private:
    char matrix[5][5];
    pair<int, int> pos[26]; // vị trí của từng ký tự A-Z, J dùng chung với I

    string normalizeKey(const string &key)
    {
        string result;
        vector<bool> used(26, false);

        for (char ch : key)
        {
            if (isalpha(static_cast<unsigned char>(ch)))
            {
                ch = toupper(ch);
                if (ch == 'J')
                    ch = 'I';
                if (!used[ch - 'A'])
                {
                    used[ch - 'A'] = true;
                    result += ch;
                }
            }
        }

        for (char ch = 'A'; ch <= 'Z'; ch++)
        {
            if (ch == 'J')
                continue; // gộp J vào I
            if (!used[ch - 'A'])
            {
                used[ch - 'A'] = true;
                result += ch;
            }
        }

        return result;
    }

    string normalizeText(const string &text)
    {
        string result;
        for (char ch : text)
        {
            if (isalpha(static_cast<unsigned char>(ch)))
            {
                ch = toupper(ch);
                if (ch == 'J')
                    ch = 'I';
                result += ch;
            }
        }
        return result;
    }

    string preparePlaintext(const string &text)
    {
        string s = normalizeText(text);
        string prepared;

        for (size_t i = 0; i < s.size();)
        {
            char first = s[i];
            char second;

            if (i + 1 < s.size())
            {
                second = s[i + 1];
                if (first == second)
                {
                    prepared += first;
                    prepared += 'X';
                    i += 1;
                }
                else
                {
                    prepared += first;
                    prepared += second;
                    i += 2;
                }
            }
            else
            {
                prepared += first;
                prepared += 'X';
                i += 1;
            }
        }

        return prepared;
    }

    pair<char, char> encryptPair(char a, char b)
    {
        int r1 = pos[a - 'A'].first;
        int c1 = pos[a - 'A'].second;
        int r2 = pos[b - 'A'].first;
        int c2 = pos[b - 'A'].second;

        if (r1 == r2)
        {
            return {matrix[r1][(c1 + 1) % 5], matrix[r2][(c2 + 1) % 5]};
        }
        else if (c1 == c2)
        {
            return {matrix[(r1 + 1) % 5][c1], matrix[(r2 + 1) % 5][c2]};
        }
        else
        {
            return {matrix[r1][c2], matrix[r2][c1]};
        }
    }

    pair<char, char> decryptPair(char a, char b)
    {
        int r1 = pos[a - 'A'].first;
        int c1 = pos[a - 'A'].second;
        int r2 = pos[b - 'A'].first;
        int c2 = pos[b - 'A'].second;

        if (r1 == r2)
        {
            return {matrix[r1][(c1 + 4) % 5], matrix[r2][(c2 + 4) % 5]};
        }
        else if (c1 == c2)
        {
            return {matrix[(r1 + 4) % 5][c1], matrix[(r2 + 4) % 5][c2]};
        }
        else
        {
            return {matrix[r1][c2], matrix[r2][c1]};
        }
    }

    string removePaddingX(const string &text)
    {
        string result;

        for (size_t i = 0; i < text.size(); i++)
        {
            // Nếu gặp mẫu A X A thì bỏ X ở giữa
            if (i > 0 && i < text.size() - 1 &&
                text[i] == 'X' &&
                text[i - 1] == text[i + 1])
            {
                continue;
            }
            result += text[i];
        }

        // Nếu cuối chuỗi là X thì bỏ luôn
        if (!result.empty() && result.back() == 'X')
        {
            result.pop_back();
        }

        return result;
    }

public:
    PlayfairCipher(const string &key)
    {
        string fullKey = normalizeKey(key);

        int idx = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                matrix[i][j] = fullKey[idx++];
                pos[matrix[i][j] - 'A'] = {i, j};
            }
        }

        // J dùng cùng vị trí với I
        pos['J' - 'A'] = pos['I' - 'A'];
    }

    void printMatrix()
    {
        cout << "\nMa tran Playfair 5x5:\n";
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                cout << matrix[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    string encrypt(const string &plaintext)
    {
        string prepared = preparePlaintext(plaintext);
        string ciphertext;

        for (size_t i = 0; i < prepared.size(); i += 2)
        {
            auto p = encryptPair(prepared[i], prepared[i + 1]);
            ciphertext += p.first;
            ciphertext += p.second;
        }

        return ciphertext;
    }

    string decryptRaw(const string &ciphertext)
    {
        string s = normalizeText(ciphertext);

        if (s.size() % 2 != 0)
        {
            cerr << "Ciphertext khong hop le: do dai phai la so chan.\n";
            return "";
        }

        string plaintext;
        for (size_t i = 0; i < s.size(); i += 2)
        {
            auto p = decryptPair(s[i], s[i + 1]);
            plaintext += p.first;
            plaintext += p.second;
        }

        return plaintext;
    }

    string decrypt(const string &ciphertext)
    {
        string raw = decryptRaw(ciphertext);
        return removePaddingX(raw);
    }

    string getPreparedPlaintext(const string &plaintext)
    {
        return preparePlaintext(plaintext);
    }
};

int main()
{
    int choice;
    string key, text;

    cout << "===== PLAYFAIR CIPHER - C++ =====\n";
    cout << "1. Ma hoa\n";
    cout << "2. Giai ma\n";
    cout << "Chon: ";
    cin >> choice;
    cin.ignore();

    cout << "Nhap khoa: ";
    getline(cin, key);

    PlayfairCipher pf(key);
    pf.printMatrix();

    if (choice == 1)
    {
        cout << "Nhap ban ro: ";
        getline(cin, text);

        string prepared = pf.getPreparedPlaintext(text);
        string encrypted = pf.encrypt(text);

        cout << "\nBan ro sau khi chuan hoa va tach cap: " << prepared << '\n';
        cout << "Ciphertext: " << encrypted << '\n';
    }
    else if (choice == 2)
    {
        cout << "Nhap ciphertext: ";
        getline(cin, text);

        string decryptedRaw = pf.decryptRaw(text);
        string decryptedClean = pf.decrypt(text);

        cout << "\nPlaintext sau giai ma (giu nguyen): " << decryptedRaw << '\n';
        cout << "Plaintext sau khi bo ky tu X dem: " << decryptedClean << '\n';
    }
    else
    {
        cout << "Lua chon khong hop le.\n";
    }

    return 0;
}