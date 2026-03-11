#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

double englishFreq[26] = {
8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,
6.966,0.153,0.772,4.025,2.406,6.749,7.507,1.929,
0.095,5.987,6.327,9.056,2.758,0.978,2.360,0.150,
1.974,0.074};

string normalize(string text)
{
    string ans="";
    for(char c:text)
        if(isalpha(c))
            ans+=toupper(c);
    return ans;
}

double calcIC(string text)
{
    int freq[26]={0};
    int n=text.size();

    for(char c:text)
        freq[c-'A']++;

    double sum=0;

    for(int i=0;i<26;i++)
        sum+=freq[i]*(freq[i]-1);

    return sum/(n*(n-1.0));
}

int findKeyLength(string cipher)
{
    int bestLen=1;
    double bestIC=0;

    for(int k=1;k<=20;k++)
    {
        double icSum=0;

        for(int i=0;i<k;i++)
        {
            string group="";

            for(int j=i;j<cipher.size();j+=k)
                group+=cipher[j];

            icSum+=calcIC(group);
        }

        double avg=icSum/k;

        cout<<"k="<<k<<" IC="<<avg<<endl;

        if(avg>bestIC)
        {
            bestIC=avg;
            bestLen=k;
        }
    }

    return bestLen;
}

char findShift(string group)
{
    int freq[26]={0};
    int n=group.size();

    for(char c:group)
        freq[c-'A']++;

    double bestScore=1e18;
    int bestShift=0;

    for(int s=0;s<26;s++)
    {
        double chi=0;

        for(int i=0;i<26;i++)
        {
            int idx=(i+s)%26;
            double expected=englishFreq[i]*n/100.0;

            chi+=(freq[idx]-expected)*(freq[idx]-expected)/expected;
        }

        if(chi<bestScore)
        {
            bestScore=chi;
            bestShift=s;
        }
    }

    return 'A'+bestShift;
}

string findKey(string cipher,int k)
{
    string key="";

    for(int i=0;i<k;i++)
    {
        string group="";

        for(int j=i;j<cipher.size();j+=k)
            group+=cipher[j];

        char shift=findShift(group);

        key+=shift;
    }

    return key;
}

string decrypt(string cipher,string key)
{
    string plain="";
    int k=key.size();

    for(int i=0;i<cipher.size();i++)
    {
        int c=cipher[i]-'A';
        int s=key[i%k]-'A';

        char p=(c-s+26)%26+'A';

        plain+=p;
    }

    return plain;
}

int main()
{
    string cipher;

    cout<<"Ciphertext: ";
    getline(cin,cipher);

    cipher=normalize(cipher);

    cout<<"\nFinding key length...\n";

    int k=findKeyLength(cipher);

    cout<<"\nBest key length: "<<k<<endl;

    string key=findKey(cipher,k);

    cout<<"Key: "<<key<<endl;

    string plain=decrypt(cipher,key);

    cout<<"\nPlaintext:\n"<<plain<<endl;

    return 0;
}