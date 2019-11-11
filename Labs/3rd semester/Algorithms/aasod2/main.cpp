#include "l_intrfc.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdlib>
#include <strstream>
#define N 1000

using namespace h_list;
using namespace std;

int main()
{
    base x, y;
    lisp s = NULL;
    int k = 0;
    char str[N];

    while(k != 3){
        strstream st;
        cout << endl << "1 - Reading from file, 2 - Keyboard input, 3 - Exit from the program." << endl;
        cin >> k;
        switch (k) {
            case 1:{   
                cout << "Enter file name:" << endl;
                cin >> str;
                ifstream outfile(str);
                if (!outfile) { cout << "Input file not open!" << endl; break; }
                outfile.read(str, N);
                outfile.close();
                st << str;
                break;
            }
            case 2:{
                cout << "Enter a list:" << endl;
                cin.get();
                cin.getline(str, N);

                st << str;
                break;
            }
            case 3:{ cout << "Press Enter\n"; return 0; }
        }

        try { read_lisp(s, st); }
        catch(int a){
            Error(a);
            continue;
        }
        cout << "List entered: " << endl;
        write_lisp (s);
        cout << endl;
        cout << "Enter x y\n";
        cin >> x >> y;
        if(rep(s, x, y)){
            cout << "Replaced successfully." << endl;
            cout << "Processed list:" << endl;
            write_lisp (s);
        }
        else cout << "No replacement items found." << endl;
		cout << endl;
        destroy(s);
        s = NULL;
    }
    return 0;
}

