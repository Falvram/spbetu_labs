#include <iostream>
#include <cmath>
using namespace std;
double X[11] = {0.3120,  0.4990, 0.6870,  0.8740, 1.0620, 1.2490, 1.4370, 1.6240, 1.8120, 1.9990, 2.1870};
double Y[11] = {-0.3060, -0.0760, 0.0180, 0.0150, -0.0440, -0.1210, -0.1770, -0.1720, -0.0650, 0.1800, 0.6080};

double F1(int r, int k)
{
    return (r == 0)?(Y[k+1] - Y[k]):(F1(r-1,k+1)-F1(r-1,k));
}
double F2(int r, int k)
{
    return (r == 0)?(Y[k] - Y[k-1]):(F2(r-1,k)-F2(r-1,k-1));
}

double forward_int(double x)
{
     double q = (x-X[0])/0.1875;
     double res = Y[0];
     double p = 1;
     for(int i=0; i<11;i++){
         p*=(q-i)/(i+1);
         res+=p*F1(i,0);
     }
     return res;
}

double back_int(double x)
{
     double q = (x-X[10])/0.1875;
     double res = Y[10];
     double p = 1;
     for(int i=0; i<11;i++){
         p*=(q+i)/(i+1);
         res+=p*F2(i,10);
     }
     return res;
}

double Stirling(double x)
{
    double q = (-x+X[7])/0.1875;
    double res = Y[7];
    double p = 1;
    for(int i=1; i<4;i++){
        p*=(pow(q,2)-pow(i-1,2))/(2*i-1);
        res+=p*(F1(2*i-1,7-i)-F1(2*i-1,8-i))/2/q;
        p=p/(2*i);
        res+=p*F1(2*i, 7-i);
    }
    return res;
}

int main()
{
    cout << forward_int(1.3020) << " " << back_int(1.8350) << " " << Stirling(1.5690) << endl;
    return 0;
}
