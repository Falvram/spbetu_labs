#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "methods.h"
using namespace std;
#define e 2,7182818284
double alpha, nu;

double Round (double X,double Delta)
{
 if (Delta<=1E-9) {puts("Incorrect accuracy");exit(1);}
 if (X>0.0) return (Delta*(long((X/Delta)+0.5)));
    else    return (Delta*(long((X/Delta)-0.5)));
}

double F(double x)
{
double pi = 3.1415926535;
extern double delta;
double s;
long int S;
s = (log(tan(x))+log(5))/pi;
if( s/delta < 0 )
S = s/delta - .5;
else
S = s/delta + .5;
s = S*delta;
s = Round( s,delta );
return(s);
}

double ITER(double X0,double Eps,int &N)
{
    double x[100];
  if (Eps<=0.0) {puts("Incorrect accuracy setting");exit(1);}
  double X1=F(X0);
  x[0]=X1;
  double X2=F(X1);
  x[1]=X2;
  N = 2;

  while( (X1 - X2)*(X1 - X2) > fabs((2*X1-X0-X2)*Eps) )
  {
	X0 = X1;
	X1 = X2;
	X2 = F(X1);
    x[N]=X2;
	N++;
  }
  cout << N << endl;
  alpha = 0.7;
  for(int i = 0; i<N-2; i++){
      alpha = (x[i+2]-x[i+1])/(x[i+1]-x[i]);
      cout << i << " nu: " << 1/(1-alpha) << " l: " << abs(x[i+2]-1) << " <= " << abs(alpha/(1-alpha)*(x[i+2]-x[i+1]));
      printf(" x=%f    k=%d\n",x[i],N);
  }

   return(X2);
}


