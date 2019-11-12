#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "methods.h"
void speed(double X, int &N);
#define pi 3.14159265358979323846
#define e 2.71828182845904523536
using namespace std;
double l, r, fr, fl;

double Round (double X,double Delta)
{
 if (Delta<=1E-9) {puts("Ќеверное задание точности округлени€\n");exit(1);}
 if (X>0.0) return (Delta*(long((X/Delta)+0.5)));
    else    return (Delta*(long((X/Delta)-0.5)));
}

double F(double x)
{
extern double delta;
double s;
long int S;
s = pow(pi, x)-e;
if( s/delta < 0 )
S = s/delta - .5;
else
S = s/delta + .5;
s = S*delta;
s = Round( s,delta );
return(s);
}

double BISECT(double Left,double Right,double Eps,int &N)
{
    r = Right;
    l = Left;
  double E = fabs(Eps)*2.0;
  double FLeft = F(Left);
  double FRight = F(Right);
  double X = (Left+Right)/2.0;
  double Y;

  if (FLeft*FRight>0.0) {puts("Ќеверное задание интервала\n");exit(1);}
  if (Eps<=0.0) {puts("Ќеверное задание точности\n");exit(1);}

  N=0;
  if (FLeft==0.0)  return Left;
  if (FRight==0.0) return Right;

  while ((Right-Left)>=E)
  {
	X = 0.5*(Right + Left);     /* вычисление середины отрезка       */
	Y = F(X);
	if (Y == 0.0) return (X);
    if (Y*FLeft < 0.0){
	   Right=X;
       r = Right;
    }
	else
     {
        Left=X;
        FLeft=Y;
        l = Left;
    }
	N++;
    speed(X, N);
  };
  return X;
}

void speed(double X, int &N){
    cout << N << ": " << X << " "<< l << " " << r << endl;
}
