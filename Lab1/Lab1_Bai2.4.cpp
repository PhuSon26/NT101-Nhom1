#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;

string normalize(string key)
{
    string ans;
    for (char c : key)
    {
        if (isalpha(c)) ans += toupper(c);
    }
    return ans;
}



int main()
{

}