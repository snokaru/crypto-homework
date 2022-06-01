#include<iostream>
#include<math.h>

using namespace std;

//gasim cel mai mare divizor comun
int cmmdc(int a, int b)
{
    int var;
    while(1)
    {
        var = a%b;
        if(var==0)
            return b;
        a = b;
        b = var;
    }
}

int main()
{
    //2 random prime numbers
    double p ;
    double q ;
    double mesaj ;

    cout<<"Introdu prima valoare : ";
    cin>>p;
    cout<<"Introdu prima valoare : ";
    cin>>q;
    cout<<"Introdu mesaj : ";
    cin>>mesaj;
    //aflam prima parte a chei publice
    double n=p*q;
    double counter;
    double variabilaa = (p-1)*(q-1);
    double e=2;

    //cautam si cealalta parte a chei
    for(e;e<variabilaa;e++){

        counter = cmmdc(e,variabilaa);
        if(counter==1)
            break;
    }

    //private key
    double d;
    //k -  constanta
    double k = 2;

    //alegem d care sa satisfaca d*e = 1 + k * varibailaa
    d = (1 + (k*variabilaa))/e;

    double cryptat = pow(mesaj,e);
    double decryptat = pow(cryptat,d);
    cryptat=fmod(cryptat,n);
    decryptat=fmod(decryptat,n);
    cout<<"Mesajul = "<<mesaj;

    cout<<"\n"<<"Crypted:"<<cryptat;
    cout<<"\n"<<"Decrypted:"<<(int) decryptat << "\n";

    return 0;
}
