#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "methods.h"
#include <cmath>
#define pi 3.14159265358979323846
#define e 2.71828182845904523536
using namespace std;
double l, r, fr, fl;

double Round (double X,double Delta)
{
 if (Delta<=1E-9) {puts("Invalid rounding precision\n");exit(1);}
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

double HORDA(double Left,double Right,double Eps,int &N)
{
  double FLeft = F(Left);
  double FRight = F(Right);
  double X,Y;
  r = Right;
  l = Left;

  if (FLeft*FRight>0.0) {puts("Invalid interval setting\n");exit(1);}
  if (Eps<=0.0) {puts("Invalid precision setting\n");exit(1);}

  N=0;
  if (FLeft==0.0)  return Left;
  if (FRight==0.0) return Right;
  X = 0;
  do
  {
	X = Left-(Right-Left)*FLeft/(FRight-FLeft);
	Y = F(X);
	if (Y == 0.0) return (X);
	if (Y*FLeft < 0.0)
     { Right=X;
        cout<< "right";
        FRight=Y;
        r = Right;

    }
	else
     { Left=X;
        FLeft=Y;
        l = Left;
    }
    fr = FRight;
    fl = FLeft;

	N++;
    speed(X, N);

  }
  while ( fabs(Y) >= Eps );

  return(X);

}

void speed(double X, int &N){
    cout << N << ": " << X << " "<< l << " " << r << endl;
}
