#include <iostream>
#include <cmath>
using namespace std;
double rectangle(double a, double b, int n);
double trapezium(double a, double b, int n);
double Simpson(double a, double b, int n);
double F(double x);

int main()
{
    double exact = 0.44302828299;
    double a = 0;
    double b = 3.1415926535;
    double eps, check, rect, tr, sm;
    int n = 1;
    cout << "Enter Eps" << endl;
    cin >> eps;
    do{
        check = abs(rectangle(a,b,2*n)-rectangle(a,b,n))/3;
        rect = rectangle(a,b,2*n) + check;
        n *= 2;
    }while(abs(rect - exact) > eps);
    cout <<"rectangle: " << rect << " n: " << n/2 << endl;
    n = 1;

    do{
        check = abs(trapezium(a,b,2*n)-trapezium(a,b,n))/3;
        tr = trapezium(a,b,2*n) - check;
        n *= 2;
    }while (abs(tr - exact) > eps);
    cout <<"trapezium: " << tr << " n: " << n/2 << endl;
    n = 1;

    do{
        check = abs(Simpson(a,b,2*n)-Simpson(a,b,n))/15;
        sm = Simpson(a,b,2*n) - check;
        n *= 2;
        cout << sm - exact << " " << n << endl;
    }while (abs(sm - exact) > eps);
    cout <<"Simpson: " << sm << " n: " << n/2 << endl;
    return 0;
}

double F(double x){
    return pow(x,2)*exp(-pow(x,2));
}

double rectangle(double a, double b, int n){
    double rect = 0;
    double h = (b-a)/n;
    for(int i = 0; i < n; i++){
        rect += F(a+i*h+h/2);
    }
    rect *= h;
    return rect;
}

double trapezium(double a, double b, int n){
    double tr = 0;
    double h = (b-a)/n;
    for(int i = 0; i < n; i++){
        tr += F(a+i*h) + F(a+(i+1)*h);
    }
    tr *= h/2;
    return tr;
}

double Simpson(double a, double b, int n){
    double sm = 0;
    double h = (b-a)/n;
    for(int i = 0; i < n; i++){
        sm += F(a+2*i*h) + 4*F(a+(2*i+1)*h) + F(a+(2*i+2)*h);
    }
    sm *= h/3;
    return sm;
}
