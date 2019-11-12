#include <iostream>
using namespace std;
double X[11] = {0.3120,  0.4990, 0.6870,  0.8740, 1.0620, 1.2490, 1.4370, 1.6240, 1.8120, 1.9990, 2.1870};
double Y[11] = {-0.3060, -0.0760, 0.0180, 0.0150, -0.0440, -0.1210, -0.1770, -0.1720, -0.0650, 0.1800, 0.6080};
//double Y[11] = {-3.3559,0.2121,-0.1106,-0.2500,-0.3531,0.6870,1.4312,2.7091,37.6553,53.9326,57.3689};
//double X[11] = {0.2816,1.7768,2.1120 , 2.2696,2.4360,3.2456,3.4176,3.6312,5.4512,5.8664,5.9432};
double x = 1.5690;
double Aitken(int i, int j)
{
    if(i == j-1)
        return 1/(X[j]-X[i])*((x-X[i])*Y[j] - (x-X[j])*Y[i]);
    else
        return 1/(X[j]-X[i])*((x-X[i])*Aitken(i+1,j)-(x-X[j])*Aitken(i,j-1));
}
double F(int i, int j)
{
    if(i == j)
        return Y[i];
    if(i == j-1)
        return (Y[j] - Y[i])/(X[j] - X[i]);
    else
        return (F(i+1,j) - F(i,j-1))/(X[j] - X[i]);
}
double Newton()
{
    double sum = 0;
    double p = 1;
    for(int i = 0; i<=10; i++)
    {
        sum= sum+p*F(0, i);
        p*=(x-X[i]);
    }
    return sum;
}
int main(){
    cout<<"Aitken:\n"<<Aitken(0,10)<<endl;
    cout<<"Newton:\n"<<Newton()<<endl;
    return 0;
}
