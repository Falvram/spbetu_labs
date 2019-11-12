#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "METHODS.H"
double delta,c,d;
int main()
{
    int k;
    float eps1,delta1;
    double a,b,eps,x;
    printf("Enter eps:");
    scanf("%f",&eps1);
    eps = eps1;
    a = 0.1;
    b = 1.5;
    printf("Enter delta:");
    scanf("%f",&delta1);
    delta = delta1;
    x = BISECT(a,b,eps,k);
    printf("x=%f    k=%d\n",x,k);
    return 0;

}

