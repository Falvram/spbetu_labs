#include<cmath>
#include<iostream>
#include<cstdlib>
#include"METHODS.H"
double delta,c,d;
int main(){
    int k;
    float eps1,delta1;
    double a,b,eps,x;
    printf(" eps:");
    scanf("%f",&eps1);
    eps = eps1;
    b = 0.549;
    printf(" delta:");
    scanf("%f",&delta1);
    delta = delta1;
    x = ITER(b,eps,k);
    printf("x=%f    k=%d\n",x,k);
    return 0;
}
