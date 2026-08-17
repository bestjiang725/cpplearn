#include"cplib/num.h"

ll gcd(ll a,ll b){return b?gcd(b,a%b):a;}
ll lcm(ll a,ll b){return a/gcd(a,b)*b;}

ll qpow(ll a,ll b,ll mod){
    ll res=1;
    while(b){
        if(b&1) res=res*a%mod;
        a=a*a%mod;
        b>>=1;
    }
    return res;
}

bool is_prime(ll x){
    for(int i=2;i*i<=x;++i){
        if(x%i==0)return false;
    }
    return true;

}