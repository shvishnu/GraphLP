#include<bits/stdc++.h>
using namespace std;
#define ll long long
int main(int argc, char** argv) {

    int n = stoi(argv[1]);
    int i;
    cout << "@PartitionA" << endl;
    for(i=1;i<n;i++) {
        cout << "r" << i << " , ";
    }
    cout << "r" << i << " ;"<<endl;
    cout << "@End" << endl;
    cout << endl;
    cout << "@PartitionB" << endl;
    cout << "h0 (0), h1 (1), h2 (" << n << ") ;" <<endl;
    cout << "@End" << endl << endl;

    cout << "@PreferenceListsA" << endl;
    
    for(int i = 1; i < n; i ++) {
        cout << "r" << i << " : h1, h0 ;" << endl;
    }
    cout << "r" << n << " : h1, h2 ;" << endl;
    cout << "@End" << endl << endl;

    cout << "@PreferenceListsB" << endl;
    
    cout << "h0" << " : ";
    for(i=1;i<n-1;i++) {
        cout << "r" << i << ", ";
    }
    cout << "r" << n-1 << " ;" << endl;
    
    cout << "h1" << " : ";
    for(i=1;i<n;i++) {
        cout << "r" << i << ", ";
    }
        cout << "r" << n << " ;" << endl;

    cout << "h2" << " : " <<"r"<<n << " ;"<<endl;
    cout << "@End" << endl;
}