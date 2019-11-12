#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "methods.h"
using namespace std;
double n, n1;
void speed(double X, int &N);
double Round (double X,double Delta)
{
 if (Delta<=1E-9) {puts("nevok\n");exit(1);}
 if (X>0.0) return (Delta*(long((X/Delta)+0.5)));
    else    return (Delta*(long((X/Delta)-0.5)));
}

double F(double x)
{
extern double delta;
double s;
long int S;
s = 2*pow(x, 2)-pow(x, 4) - 1 - log(x);
if( s/delta < 0 )
S = s/delta - .5;
else
S = s/delta + .5;
s = S*delta;
s = Round( s,delta );
return(s);
}

double F1(double x)
{
extern double delta;
double s;
long int S;
s = 4*x-4*pow(x, 3) - 1/x;
if( s/delta < 0 )
S = s/delta - .5;
else
S = s/delta + .5;
s = S*delta;
s = Round( s,delta );
return(s);
}

double NEWTON (double X,double Eps,int &N)
{
  extern double F1 (double);
  double Y,Y1,DX;
  N=0;
  do
  {
      n1 = X;
    Y  = F(X);
    if (Y==0.0) return (X);

    Y1 = F1(X);
    if (Y1==0.0) {puts("prjo0\n");exit(1);}

    DX=Y/Y1; X=X-DX; N++;
    speed(X, N);
  }
  while (fabs(DX)>Eps);
  return (X);
}

void speed(double X, int &N){
    cout << N << ": " << X << " " << abs(X-1) << " <= " << abs(X - n1) << endl;
}
