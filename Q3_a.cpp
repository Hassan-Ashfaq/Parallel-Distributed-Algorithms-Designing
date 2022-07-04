#include<iostream>
#include<string.h>
using namespace std;

int count_occurrence(string text_string, string pattern)
{
    int count = 0;
    for(int i=0; i<text_string.length()-pattern.length()+1; i++)
    {
        string temp = "";
        for(int j=0; j<pattern.length(); j++)
        {
            temp += text_string[i+j];
        }
        // cout<<temp<<endl;
        if(temp==pattern){
            count++;
        }
    }
    cout<<"Text: "<<text_string<<endl;
    cout<<"Pattern: "<<pattern<<endl;
    cout<<"Count: "<<count<<endl<<endl;
    return count;
}

int main()
{
    count_occurrence("ATTTGCGCAGACCTAAGCA", "GCA");
    count_occurrence("AAABCDEFGAAGEREAANMT", "AA");
    count_occurrence("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "BTTT");

    return 0;
}