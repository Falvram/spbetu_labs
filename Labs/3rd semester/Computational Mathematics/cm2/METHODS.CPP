#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "METHODS.H"
using namespace std;
double Round (double X,double Delta)
{
 if (Delta<=1E-9) {puts("3 \n");exit(1);}
 if (X>0.0) return (Delta*(long((X/Delta)+0.5)));
    else    return (Delta*(long((X/Delta)-0.5)));
}

double F(double x)
{
extern double delta;
double s;
long int S;
s = 2*pow(x, 2)-pow(x, 4)-1-log(x);
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
  double E = fabs(Eps)*2.0;
  double FLeft = F(Left);
  double FRight = F(Right);
  double X = (Left+Right)/2.0;
  double Y;

  if (FLeft*FRight>0.0) {puts("2\n");exit(1);}
  if (Eps<=0.0) {puts("1\n");exit(1);}

  N=0;
  if (FLeft==0.0)  return Left;
  if (FRight==0.0) return Right;

  while ((Right-Left)>=E)
  {
    X = 0.5*(Right + Left);
	Y = F(X);
	if (Y == 0.0) return (X);
	if (Y*FLeft < 0.0)
	   Right=X;
	else
	 { Left=X; FLeft=Y; }
	N++;
  };
  return X;
}
