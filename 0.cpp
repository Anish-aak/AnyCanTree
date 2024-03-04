#include<bits/stdc++.h>
using namespace std;

int main(){
    unordered_map<int,int> mp;
    mp[1] = 1;
    mp[2] = 3;
    for(auto p : mp){
        cout<<p.first<<" "<<p.second<<endl;
    }
}
