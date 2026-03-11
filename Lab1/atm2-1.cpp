#include <iostream>
#include <string>
#include <cmath>
using namespace std;

double englishFreq[26] = {
8.2,1.5,2.8,4.3,12.7,2.2,2.0,6.1,7.0,0.15,
0.77,4.0,2.4,6.7,7.5,1.9,0.095,6.0,6.3,9.1,
2.8,0.98,2.4,0.15,2.0,0.074
};

string encrypt(string text,int key){
    string result="";

    for(char c:text){
        if(isalpha(c)){
            char base = isupper(c)?'A':'a';
            result += (c-base+key)%26 + base;
        }
        else{
            result += c;
        }
    }

    return result;
}

string decrypt(string text,int key){
    return encrypt(text,26-key);
}

double scoreText(string text){

    int count[26]={0};
    int total=0;

    for(char c:text){
        if(isalpha(c)){
            c=toupper(c);
            count[c-'A']++;
            total++;
        }
    }

    if(total==0) return 1e9;

    double score=0;

    for(int i=0;i<26;i++){
        double observed = (double)count[i]/total*100;
        score += pow(observed-englishFreq[i],2);
    }

    return score;
}

void bruteForce(string cipher){

    double bestScore = 1e9;
    string bestPlain;
    int bestKey=0;

    for(int key=0; key<26; key++){

        string plain = decrypt(cipher,key);

        double s = scoreText(plain);

        if(s < bestScore){
            bestScore = s;
            bestPlain = plain;
            bestKey = key;
        }
    }

    cout << "\nBest plaintext: " << bestPlain << endl;
    cout << "Key: " << bestKey << endl;
}

int main(){

    int choice;
    string text;
    int key;

    cout<<"1. Encrypt\n";
    cout<<"2. Decrypt\n";
    cout<<"3. Brute Force\n";
    cout<<"Choose: ";
    cin>>choice;
    cin.ignore();

    if(choice==1){

        cout<<"Enter plaintext: ";
        getline(cin,text);

        cout<<"Enter key: ";
        cin>>key;

        cout<<"Ciphertext: "<<encrypt(text,key)<<endl;
    }

    else if(choice==2){

        cout<<"Enter ciphertext: ";
        getline(cin,text);

        cout<<"Enter key: ";
        cin>>key;

        cout<<"Plaintext: "<<decrypt(text,key)<<endl;
    }

    else if(choice==3){

        cout<<"Enter ciphertext: ";
        getline(cin,text);

        bruteForce(text);
    }

}