#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;
double F(double x);
double definite_integral(double a, double b);

int main()
{
    cout << setprecision(17) << definite_integral(2, 3) << endl;
    return 0;
}

double F(double x){
    //return cos(pow(x,2) + x + 1);
    return (atan(x))/x;
}

double definite_integral(double a, double b){
    //double Lezh[8] = {-0.96028986, -0.79666648, -0.52553242, -0.18343464, 0.18343464, 0.52553242, 0.79666648, 0.96028986};
    //double A[8] = {0.10122854, 0.22238103, 0.31370664, 0.36268378, 0.36268378, 0.31370664, 0.22238103, 0.10122854};
    double Lezh[4] = {0.33998, 0.86114, -0.86114, -0.33998};
    double A[4] = {0.652146, 0.347846, 0.347846, 0.652146};
    double res = 0;
    for(int i = 0; i < 8; i++){
        res += A[i]*F(0.5*(a+b+(b-a)*Lezh[i]));
    }
    return (b-a)/2*res;
}
