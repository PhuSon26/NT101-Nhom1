#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

char matrix[5][5];

string prepare_key(string key)
{
    vector<bool> used(26,false);
    string result="";

    for(char c:key)
    {
        if(isalpha(c))
        {
            c=toupper(c);
            if(c=='J') c='I';

            if(!used[c-'A'])
            {
                used[c-'A']=true;
                result+=c;
            }
        }
    }

    for(char c='A';c<='Z';c++)
    {
        if(c=='J') continue;

        if(!used[c-'A'])
        {
            used[c-'A']=true;
            result+=c;
        }
    }

    return result;
}

void create_matrix(string key)
{
    string s=prepare_key(key);

    int k=0;

    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
            matrix[i][j]=s[k++];
}

void print_matrix()
{
    cout<<"Playfair Matrix:\n";

    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }
}

void find_pos(char c,int &row,int &col)
{
    if(c=='J') c='I';

    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(matrix[i][j]==c)
            {
                row=i;
                col=j;
                return;
            }
        }
    }

    row=col=0;
}

string clean_text(string text)
{
    string result="";

    for(char c:text)
    {
        if(isalpha(c))
        {
            c=toupper(c);
            if(c=='J') c='I';
            result+=c;
        }
    }

    return result;
}

string prepare_text(string text)
{
    text=clean_text(text);

    string result="";
    int i=0;

    while(i<text.length())
    {
        char a=text[i];
        char b=(i+1<text.length())?text[i+1]:'X';

        if(a==b)
        {
            result+=a;
            result+='X';
            i++;
        }
        else
        {
            result+=a;
            result+=b;
            i+=2;
        }
    }

    if(result.length()%2!=0)
        result+='X';

    return result;
}

string encrypt(string text)
{
    string result="";

    for(int i=0;i<text.length();i+=2)
    {
        if(i+1>=text.length()) break;

        char a=text[i];
        char b=text[i+1];

        int r1,c1,r2,c2;

        find_pos(a,r1,c1);
        find_pos(b,r2,c2);

        if(r1==r2)
        {
            result+=matrix[r1][(c1+1)%5];
            result+=matrix[r2][(c2+1)%5];
        }
        else if(c1==c2)
        {
            result+=matrix[(r1+1)%5][c1];
            result+=matrix[(r2+1)%5][c2];
        }
        else
        {
            result+=matrix[r1][c2];
            result+=matrix[r2][c1];
        }
    }

    return result;
}

string decrypt(string text)
{
    text=clean_text(text);

    string result="";

    for(int i=0;i<text.length();i+=2)
    {
        if(i+1>=text.length()) break;

        char a=text[i];
        char b=text[i+1];

        int r1,c1,r2,c2;

        find_pos(a,r1,c1);
        find_pos(b,r2,c2);

        if(r1==r2)
        {
            result+=matrix[r1][(c1+4)%5];
            result+=matrix[r2][(c2+4)%5];
        }
        else if(c1==c2)
        {
            result+=matrix[(r1+4)%5][c1];
            result+=matrix[(r2+4)%5][c2];
        }
        else
        {
            result+=matrix[r1][c2];
            result+=matrix[r2][c1];
        }
    }

    return result;
}

int main()
{
    string key,text;
    int choice;

    cout<<"1. Encrypt\n2. Decrypt\nChoice: ";
    cin>>choice;
    cin.ignore();

    cout<<"Enter key: ";
    getline(cin,key);

    create_matrix(key);
    print_matrix();

    if(choice==1)
    {
        cout<<"Enter plaintext: ";
        getline(cin,text);

        text=prepare_text(text);

        cout<<"Prepared text: "<<text<<endl;

        cout<<"Ciphertext: "<<encrypt(text)<<endl;
    }
    else
    {
        cout<<"Enter ciphertext: ";
        getline(cin,text);

        cout<<"Plaintext: "<<decrypt(text)<<endl;
    }
}